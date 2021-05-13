// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableBase.h"
#include "SelectableBase.h"
#include "InteractableHUD.h"
#include "Components/TextRenderComponent.h"
#include "Components/BoxComponent.h"

class UAnimationComponent;

// Sets default values
AInteractableBase::AInteractableBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root"));
	Root->SetupAttachment(GetRootComponent());

	InteractableParent = CreateDefaultSubobject<USceneComponent>(TEXT("Interactable Parent"));
	InteractableParent->SetupAttachment(Root);

	InteractableAttachPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Weapon Attach point"));
	InteractableAttachPoint->SetupAttachment(InteractableParent);

	InteractableCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	InteractableCollisionBox->SetupAttachment(Root);

	InteractableTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Interaction Collider"));
	InteractableTriggerBox->SetupAttachment(Root);

	DisplayTextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("DisplayText"));
	DisplayTextComponent->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void AInteractableBase::BeginPlay()
{
	Super::BeginPlay();

	SelectableInteraction = FindComponentByClass<USelectableBase>();

	HUD = FindComponentByClass<UInteractableHUD>();

	if (HUD) {
		HUD->Initialize(DisplayTextComponent);
	}
}

// Called every frame
void AInteractableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractableBase::OnSelectStart(USceneComponent* ControllerPosition)
{
	if (bIsHighlighting && SelectableInteraction)
		SelectableInteraction->OnInteractPress(ControllerPosition);
}

void AInteractableBase::OnSelectEnd()
{
	if (SelectableInteraction)
		SelectableInteraction->OnInteractRelease();
}

void AInteractableBase::OnOverlapStart() 
{
	bIsHighlighting = true;
}

void AInteractableBase::OnOverlapEnd()
{
	bIsHighlighting = false;
}

void AInteractableBase::OnRaycastStart()
{
	bIsHighlighting = true;
	Highlight(false);
	if (HUD)
		HUD->SetVisibility(true);
}

void AInteractableBase::OnRaycastEnd()
{
	bIsHighlighting = false;
	Highlight(false);
	if (HUD)
		HUD->SetVisibility(false);
}

void AInteractableBase::Highlight(bool bHighlight)
{

}

void AInteractableBase::EnablePhysics(bool bState)
{
	Root->SetSimulatePhysics(bState);
	//Root->SetEnableGravity(bState);
}

void AInteractableBase::EnableCollisions(ECollisionEnabled::Type CollisionState)
{
	InteractableTriggerBox->SetCollisionEnabled(CollisionState);
	InteractableCollisionBox->SetCollisionEnabled(CollisionState);
}

void AInteractableBase::AttachToHand(USceneComponent* AttachPoint)
{
	EnableCollisions(ECollisionEnabled::NoCollision);
	Highlight(false);

	UE_LOG(LogTemp, Warning, TEXT("Attached to hand"));

	EAttachmentRule AttachmentRule = EAttachmentRule::SnapToTarget;
	EAttachmentRule ScaleRule = EAttachmentRule::KeepRelative;
	FAttachmentTransformRules rules = FAttachmentTransformRules(AttachmentRule, AttachmentRule, ScaleRule, false);

	AttachToComponent(AttachPoint, rules);
}

FTransform AInteractableBase::GetAttachPoint()
{
	return InteractableAttachPoint->GetComponentTransform();
}