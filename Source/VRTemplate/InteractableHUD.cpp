// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableHUD.h"
#include "Components/TextRenderComponent.h"
#include "InteractableBase.h"

// Sets default values for this component's properties
UInteractableHUD::UInteractableHUD()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UInteractableHUD::BeginPlay()
{
	Super::BeginPlay();

	Interactable = Cast<AInteractableBase>(GetOwner());
}


// Called every frame
void UInteractableHUD::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (DisplayTextComponent) {
		// Set the Text FVector to the owners, with padding.
		FVector ActorWorldLocation = Interactable->GetActorLocation();
		ActorWorldLocation.Z += 50.f; // applies vertical padding
		DisplayTextComponent->SetWorldLocation(ActorWorldLocation);

		// Set the Text FRotator to face the player.
		FVector vecToPlayer = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation() - ActorWorldLocation;
		FRotator RotationToPlayer = FRotationMatrix::MakeFromX((vecToPlayer)).Rotator();
		DisplayTextComponent->SetWorldRotation(RotationToPlayer);
	}

}

void UInteractableHUD::Initialize(UTextRenderComponent* TextRenderComponent)
{
	DisplayTextComponent = TextRenderComponent;

	DisplayTextComponent->SetWorldSize(TextSize);
	DisplayTextComponent->SetText(ItemName);
	SetVisibility(false);
}

void UInteractableHUD::SetVisibility(bool bVisible)
{
	DisplayTextComponent->SetVisibility(bVisible);
}

