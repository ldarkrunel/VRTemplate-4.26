// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableAnimated.h"
#include "SelectableBase.h"

// Sets default values
AInteractableAnimated::AInteractableAnimated()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	InteractableSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Interactable Skeletal Mesh"));
	InteractableSkeletalMesh->SetupAttachment(InteractableParent);
}

// Called when the game starts or when spawned
void AInteractableAnimated::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractableAnimated::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//tick inactive

}

void AInteractableAnimated::OnSelectStart(USceneComponent* ControllerPosition)
{
	AInteractableBase::OnSelectStart(ControllerPosition);
}

void AInteractableAnimated::OnSelectEnd()
{
	AInteractableBase::OnSelectEnd();
}

void AInteractableAnimated::OnOverlapStart()
{
	if (CanOverlap) {
		AInteractableBase::OnOverlapStart();
		Highlight(true);
	}
}

void AInteractableAnimated::OnOverlapEnd()
{
	if (CanOverlap) {
		AInteractableBase::OnOverlapEnd();
		Highlight(false);
	}
}

void AInteractableAnimated::OnRaycastStart()
{
	if (CanRayCast) {
		AInteractableBase::OnRaycastStart();
		Highlight(true);
	}

}

void AInteractableAnimated::OnRaycastEnd()
{
	if (CanRayCast) {
		AInteractableBase::OnRaycastEnd();
		Highlight(false);
	}
}

void AInteractableAnimated::Highlight(bool bHighlight)
{
	AInteractableBase::Highlight(bHighlight);

	InteractableSkeletalMesh->SetRenderCustomDepth(bHighlight);
}

