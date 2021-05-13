// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemHUD.h"
#include "InteractableBase.h"
#include "Components/TextRenderComponent.h"

// Sets default values
AItemHUD::AItemHUD()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DisplayTextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("DisplayText"));
	DisplayTextComponent->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void AItemHUD::BeginPlay()
{
	Super::BeginPlay();


}

// Called every frame
void AItemHUD::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Interactable) {
		FVector ActorWorldLocation = Interactable->GetActorLocation();
		ActorWorldLocation.Z += 50.f; // applies vertical padding
		DisplayTextComponent->SetWorldLocation(ActorWorldLocation);

		FVector vecToPlayer = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation() - ActorWorldLocation;
		FRotator RotationToPlayer = FRotationMatrix::MakeFromX((vecToPlayer)).Rotator();
		DisplayTextComponent->SetWorldRotation(RotationToPlayer);
	}
}

void AItemHUD::SetVisible(bool BVisible)
{
	DisplayTextComponent->SetVisibility(BVisible);
}

void AItemHUD::SetInteractable(AInteractableBase* InteractableParent)
{
	Interactable = InteractableParent;
}

void AItemHUD::Initialize(FString Name, float textSize)
{
	DisplayTextComponent->SetWorldSize(textSize);
	DisplayTextComponent->SetText(Name);
}


