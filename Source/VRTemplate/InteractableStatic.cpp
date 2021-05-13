// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableStatic.h"

// Sets default values
AInteractableStatic::AInteractableStatic()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InteractableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Interactable Mesh"));
	InteractableMesh->SetupAttachment(InteractableParent);
}

// Called when the game starts or when spawned
void AInteractableStatic::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractableStatic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractableStatic::OnSelectStart(USceneComponent* ControllerPosition)
{

}

void AInteractableStatic::OnSelectEnd()
{

}

void AInteractableStatic::OnOverlapStart()
{

}

void AInteractableStatic::OnOverlapEnd()
{

}

void AInteractableStatic::OnRaycastStart()
{

}

void AInteractableStatic::OnRaycastEnd()
{

}

void AInteractableStatic::Highlight(bool bHighlight)
{

}

