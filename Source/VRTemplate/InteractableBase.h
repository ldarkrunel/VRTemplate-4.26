// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IInteractable.h"
#include "InteractableBase.generated.h"

class UBoxComponent;
class UAnimationComponent;
class USelectableBase;
class UTextRenderComponent;
class UInteractableHUD;

UCLASS()
class VRTEMPLATE_API AInteractableBase : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* InteractableParent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* InteractableTriggerBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* InteractableCollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* InteractableAttachPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UTextRenderComponent* DisplayTextComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	bool CanOverlap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	bool CanRayCast;

	USelectableBase* SelectableInteraction;

	UInteractableHUD* HUD;

	FTransform SpawnTransform;

	bool bIsHighlighting = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnSelectStart(USceneComponent* ControllerPosition) override;
	UFUNCTION()
	virtual void OnSelectEnd() override;
	UFUNCTION()
	virtual void OnOverlapStart() override;
	UFUNCTION()
	virtual void OnOverlapEnd() override;
	UFUNCTION()
	virtual void OnRaycastStart() override;
	UFUNCTION()
	virtual void OnRaycastEnd() override;
	UFUNCTION()
	virtual void Highlight(bool bHighlight) override;

	void EnablePhysics(bool bState);
	void EnableCollisions(ECollisionEnabled::Type CollisionType);

	FTransform GetAttachPoint();

	UFUNCTION()
	void AttachToHand(USceneComponent* AttachPoint);
};
