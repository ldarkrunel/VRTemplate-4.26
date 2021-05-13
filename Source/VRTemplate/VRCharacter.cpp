// Fill out your copyright notice in the Description page of Project Settings.

#include "VRCharacter.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "NavigationSystem.h"
#include "AI/NavigationSystemBase.h"
#include "Camera/CameraComponent.h"
#include "MotionControllerComponent.h"
#include "InputCoreTypes.h"
#include "Engine/StaticMeshActor.h"
#include "Interact.h"
#include "Components/CapsuleComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"

// Sets default values
AVRCharacter::AVRCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VRRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VR Root"));
	VRRoot->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(VRRoot);

	LeftController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Left Controller"));
	LeftController->SetupAttachment(VRRoot);
	LeftController->SetTrackingSource(EControllerHand::Left);

	SM_Left = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left Controller SM"));
	SM_Left->SetupAttachment(LeftController);

	RightController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Right Controller"));
	RightController->SetupAttachment(VRRoot);
	RightController->SetTrackingSource(EControllerHand::Right);

	SM_Right = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right Controller SM"));
	SM_Right->SetupAttachment(RightController);

	DestinationMarker = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DestinationMarker"));
	DestinationMarker->SetupAttachment(VRRoot);

	RightControllerAttach = CreateDefaultSubobject<USceneComponent>(TEXT("Right Controller Attach Point"));
	RightControllerAttach->SetupAttachment(RightController);
}

// Called when the game starts or when spawned
void AVRCharacter::BeginPlay()
{
	Super::BeginPlay();
	InteractComponent = FindComponentByClass<UInteract>();
	DestinationMarker->SetVisibility(false);

	PlayerCollider = Cast<UCapsuleComponent>(GetRootComponent());

	CheckVRDeviceType();

}

// Called every frame
void AVRCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//the code below lines up the position of the player model with where he actually is in space by creating an offset
	FVector NewCameraOffset = Camera->GetComponentLocation() - GetActorLocation();
	NewCameraOffset.Z = 0.f;
	AddActorWorldOffset(NewCameraOffset);
	VRRoot->AddWorldOffset(-NewCameraOffset);

	if (bShowTeleportMarker)
		UpdateTeleportDestinationMarker();

	//interactions only working with the right controller for now.

	FVector Start = RightController->GetComponentLocation();
	FVector End = Start + RightController->GetForwardVector() * 5000.f;

	InteractComponent->PerformRayCastFromPoint(Start, End);

	UMotionControllerComponent* OverlappingController = GetOverlappingHandComponent();

	if (OverlappingController) {
		if (OverlappingController->GetTrackingSource() == EControllerHand::Right)
			InteractComponent->HandleOverlappingComponent(OverlappingController, SM_Right);
		else
			InteractComponent->HandleOverlappingComponent(OverlappingController, SM_Left);
	}
	else
		InteractComponent->EndOverlap();
}

// Called to bind functionality to input
void AVRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Forward"), this, &AVRCharacter::Move);

	PlayerInputComponent->BindAction(TEXT("Right_Teleport"), IE_Released, this, &AVRCharacter::BeginTeleport);
	PlayerInputComponent->BindAction(TEXT("Right_Teleport"), IE_Pressed, this, &AVRCharacter::ShowTeleportMarker);

	PlayerInputComponent->BindAction(TEXT("Grab"), IE_Pressed, this, &AVRCharacter::Grab);
	//PlayerInputComponent->BindAction(TEXT("R_Grab"), IE_Pressed, this, &AVRCharacter::R_Grab);
	PlayerInputComponent->BindAction(TEXT("Release"), IE_Released, this, &AVRCharacter::Release);
}

void AVRCharacter::Move(float AxisValue)
{
	AddMovementInput(AxisValue * Camera->GetForwardVector());
}

void AVRCharacter::ShowTeleportMarker()
{
	bShowTeleportMarker = true;
}

void AVRCharacter::Grab()
{	
	InteractComponent->GrabInteractable(RightControllerAttach);
}

void AVRCharacter::Release()
{
	InteractComponent->ReleaseInteractable();
}

void AVRCharacter::Rotate(float RotateAxis)
{

}

void AVRCharacter::UpdateTeleportDestinationMarker()
{
	FVector TeleportLocation;
	bool bCanTeleport = FindTeleportLocation(TeleportLocation);

	DestinationMarker->SetVisibility(false);
	if (bCanTeleport) {
		DestinationMarker->SetVisibility(true);
		DestinationMarker->SetWorldLocation(TeleportLocation);
	}
}

bool AVRCharacter::FindTeleportLocation(FVector& OutLocation)
{
	FHitResult HitResult;
	FVector Start = RightController->GetComponentLocation();
	FVector Look = RightController->GetForwardVector();

	//modify look angle so controller doesn't need to be awkwardly pointed at something directly
	Look = Look.RotateAngleAxis(30, RightController->GetRightVector());

	FVector End = Start + Look * MaxTeleportDistance;
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);

	if (!bHit) return false;

	UNavigationSystemV1* NavSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());

	FNavLocation NavLocation;

	bool bOnNavMesh = NavSystem->ProjectPointToNavigation(HitResult.Location, NavLocation, TeleportExtent);

	OutLocation = NavLocation.Location;

	return bHit && bOnNavMesh;
}

void AVRCharacter::BeginTeleport()
{
	APlayerController* PC = Cast<APlayerController>(GetController());

	if (PC) 
		PC->PlayerCameraManager->StartCameraFade(0, 1, TeleportFadeTime, FLinearColor::Black);

	FTimerHandle Handle;
	GetWorldTimerManager().SetTimer(Handle, this, &AVRCharacter::FinishTeleport, TeleportFadeTime);

}

void AVRCharacter::FinishTeleport()
{
	FVector TeleportLocation = DestinationMarker->GetComponentLocation();

	float ColliderHeight = PlayerCollider->GetScaledCapsuleHalfHeight();
	TeleportLocation.Z = TeleportLocation.Z + ColliderHeight; //change later from hardcoded height value
	SetActorLocation(TeleportLocation);
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC) 
		PC->PlayerCameraManager->StartCameraFade(1, 0, TeleportFadeTime, FLinearColor::Black);

	bShowTeleportMarker = false;
	DestinationMarker->SetVisibility(false);


	//UE_LOG(LogTemp,Warning,TEXT("VR Root is: " + GetRootComponent()->))
}

void AVRCharacter::CheckVRDeviceType()
{
	FName hmd = UHeadMountedDisplayFunctionLibrary::GetHMDDeviceName();

	if (hmd == "OculusHMD") {
		ApplyCameraHeightOffset(-108); // -108 is the most accurate tested offset for oculus;
	}
}

void AVRCharacter::ApplyCameraHeightOffset(float offset)
{
	UE_LOG(LogTemp, Warning, TEXT("Oculus offset applied"));
	// Create a camera offset for the Oculus rift.
	FVector NewCameraOffset = Camera->GetComponentLocation() - GetActorLocation();
	NewCameraOffset.Z = offset;
	AddActorWorldOffset(NewCameraOffset);
	VRRoot->AddWorldOffset(-NewCameraOffset);
}

UMotionControllerComponent* AVRCharacter::GetOverlappingHandComponent()
{
	TArray<UPrimitiveComponent*> OverlappingComponents;
	SM_Right->GetOverlappingComponents(OverlappingComponents);

	if (OverlappingComponents.Num() > 0) {
		return RightController;
	}

	SM_Left->GetOverlappingComponents(OverlappingComponents);

	if (OverlappingComponents.Num() > 0)
		return LeftController;

	return nullptr;
}

