// Fill out your copyright notice in the Description page of Project Settings.


#include "PullSelectable.h"
#include "InteractableBase.h"
#include "AnimationComponent.h"

// Sets default values for this component's properties
UPullSelectable::UPullSelectable()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UPullSelectable::BeginPlay()
{
	Super::BeginPlay();

	SpawnTransform = Interactable->GetTransform();

	if (AnimationComponent) 
		AnimationComponent->OnAnimFinished.AddDynamic(this, &UPullSelectable::AttachToHand);
}

void UPullSelectable::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPullSelectable::OnInteractPress(USceneComponent* Controller)
{
	AttachPoint = Controller;

	Interactable->EnablePhysics(false);

	AnimationComponent->CreateAnimation(Interactable->GetAttachPoint(), AttachPoint, EAnimType::NONE);
	AnimationComponent->Start();
}

void UPullSelectable::OnInteractRelease()
{
	Interactable->DetachRootComponentFromParent();
	

	if (bReturnToInitialLocation) {

		AnimationComponent->CreateAnimation(Interactable->GetActorTransform(), SpawnTransform, EAnimType::NONE);
		AnimationComponent->Start();
	}
	else {
		Interactable->EnablePhysics(true);
	}

	Interactable->EnableCollisions(ECollisionEnabled::QueryAndPhysics);
	AttachPoint = nullptr;
}

void UPullSelectable::AttachToHand()
{
	if (FVector::Distance(Interactable->GetActorLocation(), SpawnTransform.GetLocation()) <= 2.f)
		return;

	Interactable->AttachToHand(AttachPoint);
}

