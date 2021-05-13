// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "VRCharacter.generated.h"

class UCameraComponent;
class USceneComponent;
class UMotionControllerComponent;
class UInteract;
class AStaticMeshActor;
class UCapsuleComponent;

UCLASS()
class VRTEMPLATE_API AVRCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVRCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(BlueprintReadOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UInteract* InteractComponent;

	UPROPERTY(VisibleAnywhere)
	UMotionControllerComponent* LeftController;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* SM_Left;

	UPROPERTY(VisibleAnywhere)
	UMotionControllerComponent* RightController;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* SM_Right;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* VRRoot;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* DestinationMarker;

	UPROPERTY(EditAnywhere)
	AStaticMeshActor* TestActor;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* RightControllerAttach;

	UCapsuleComponent* PlayerCollider;

	UPROPERTY(EditAnywhere)
	float MaxTeleportDistance = 1000.f;

	UPROPERTY(EditAnywhere)
	float TeleportFadeTime = 0.15f;

	UPROPERTY(EditAnywhere)
	FVector TeleportExtent = FVector(100, 100, 100);

	bool bShowTeleportMarker = false;

	void Move(float AxisValue);
	void Rotate(float RotateAxis);

	void UpdateTeleportDestinationMarker();

	void BeginTeleport();

	void FinishTeleport();

	bool FindTeleportLocation(FVector& OutLocation);

	void ShowTeleportMarker();

	void Grab();
	void Release();

	void CheckVRDeviceType();
	void ApplyCameraHeightOffset(float offset);

	UMotionControllerComponent* GetOverlappingHandComponent();

};
