// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectableBase.h"
#include "InteractableBase.h"
#include "AnimationComponent.h"

// Sets default values for this component's properties
USelectableBase::USelectableBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void USelectableBase::BeginPlay()
{
	Super::BeginPlay();

	Interactable = Cast<AInteractableBase>(GetOwner());

	AnimationComponent = Interactable->FindComponentByClass<UAnimationComponent>();
}


// Called every frame
void USelectableBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USelectableBase::OnInteractPress(USceneComponent* Controller)
{

}

void USelectableBase::OnInteractRelease()
{

}
