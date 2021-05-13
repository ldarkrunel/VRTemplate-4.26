// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableBase.h"
#include "Interact.generated.h"

class APlayerController;
class UMotionControllerComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VRTEMPLATE_API UInteract : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteract();
	void PerformRayCastFromPoint(const FVector& Start, const FVector& End);
	void GrabInteractable(USceneComponent* Controller);
	void ReleaseInteractable();

	void HandleOverlappingComponent(UMotionControllerComponent* OverlappingComponent, UStaticMeshComponent* ControllerMesh);
	void EndOverlap();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	IInteractable* CurrentlyHighlightedInteractable = nullptr;
	IInteractable* CurrentlySelectedInteractable = nullptr;

	bool bIsOverlapping = false;

	UPROPERTY(BlueprintReadOnly, BlueprintReadOnly, Category = "PlayerController", meta = (AllowPrivateAccess = "true"))
	APlayerController* PC;

	UPROPERTY(BlueprintReadOnly, BlueprintReadOnly, Category = "PlayerController", meta = (AllowPrivateAccess = "true"))
	UStaticMesh* LineRender;

	void RenderLine(const FVector& Start, const FVector& End, TArray<FHitResult>& HitResults);

	void HighlightActor(IInteractable* CurrentInteractable);
	void StopHighlightingActor();

	IInteractable* ReturnClosestInteractableToPlayer(TArray<FHitResult> HitResults);

};
