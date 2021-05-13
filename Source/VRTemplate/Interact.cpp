// Fill out your copyright notice in the Description page of Project Settings.


#include "Interact.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"
#include "Components/InputComponent.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "MotionControllerComponent.h"
#include "IInteractable.h"


// Sets default values for this component's properties
UInteract::UInteract()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UInteract::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UInteract::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UInteract::RenderLine(const FVector& Start, const FVector& End, TArray<FHitResult>& HitResults)
{
	if (HitResults.Num() > 0)
		DrawDebugLine(GetWorld(), Start, HitResults[HitResults.Num()-1].Location, FColor::Red, false, -1.f, 0, 1.f);
	else
		DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, -1.f, 0, 1.f);
}


void UInteract::GrabInteractable(USceneComponent* Controller)
{
	if (CurrentlyHighlightedInteractable) {
		CurrentlySelectedInteractable = CurrentlyHighlightedInteractable;
		CurrentlySelectedInteractable->OnSelectStart(Controller);
	}
}

void UInteract::ReleaseInteractable()
{
	if (CurrentlySelectedInteractable) {
		CurrentlySelectedInteractable->OnSelectEnd();
		CurrentlySelectedInteractable = nullptr;
	}
}
void UInteract::HandleOverlappingComponent(UMotionControllerComponent* OverlappingController, UStaticMeshComponent* ControllerMesh)
{
	TArray<UPrimitiveComponent*> OverlappingComponents;
	ControllerMesh->GetOverlappingComponents(OverlappingComponents);

	if (OverlappingComponents.Num() > 0)
	{
		IInteractable* Interactable = Cast<IInteractable>(OverlappingComponents[0]->GetOwner());

		if (Interactable) {

			bIsOverlapping = true;

			if (Interactable != CurrentlyHighlightedInteractable) {

				if (CurrentlyHighlightedInteractable != nullptr)
					CurrentlyHighlightedInteractable->OnOverlapEnd();

				UE_LOG(LogTemp, Warning, TEXT("Overlapping interactable"));

				Interactable->OnOverlapStart();
				CurrentlyHighlightedInteractable = Interactable;
			}
		}
		else
			EndOverlap();
	}
}

void UInteract::EndOverlap()
{
	if (bIsOverlapping && CurrentlyHighlightedInteractable != nullptr) {
		bIsOverlapping = false;
		CurrentlyHighlightedInteractable->OnOverlapEnd();
		CurrentlyHighlightedInteractable = nullptr;
	}
}

void UInteract::PerformRayCastFromPoint(const FVector& Start, const FVector& End)
{
	TArray<FHitResult> HitResults;
	GetWorld()->LineTraceMultiByChannel(HitResults, Start, End, ECC_Visibility);

	RenderLine(Start, End, HitResults);

	if (bIsOverlapping) return;

	IInteractable* ClosestInteractable = ReturnClosestInteractableToPlayer(HitResults);

	if (ClosestInteractable)
		HighlightActor(ClosestInteractable);
	else
		StopHighlightingActor();
}

void UInteract::HighlightActor(IInteractable* Interactable)
{
	//FVector_NetQuantize HitPoint = HitResult.ImpactPoint;

	if (CurrentlyHighlightedInteractable == nullptr) {
		Interactable->OnRaycastStart();
		CurrentlyHighlightedInteractable = Interactable;
	}
	else if (Interactable != CurrentlyHighlightedInteractable) {
		CurrentlyHighlightedInteractable->OnRaycastEnd();
		CurrentlyHighlightedInteractable = Interactable;
		Interactable->OnRaycastStart();
	}
}

void UInteract::StopHighlightingActor()
{
	if (CurrentlyHighlightedInteractable) {
		CurrentlyHighlightedInteractable->OnRaycastEnd();
		CurrentlyHighlightedInteractable = nullptr;
	}
}

IInteractable* UInteract::ReturnClosestInteractableToPlayer(TArray<FHitResult> HitResults)
{
	IInteractable* ClosestInteractableToPlayer = nullptr;
	float ClosestInteractableDistance = 10000000.f; //return max int value distance if possible

	for (FHitResult Result : HitResults) {
		IInteractable* Interactable = Cast<IInteractable>(Result.Actor);
		if (Interactable) {
			if (Result.Distance < ClosestInteractableDistance){
				ClosestInteractableDistance = Result.Distance;
				ClosestInteractableToPlayer = Interactable;
			}
		}
	}

	return ClosestInteractableToPlayer;
}