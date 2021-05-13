// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SelectableBase.generated.h"

class UAnimationComponent;
class AInteractableBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VRTEMPLATE_API USelectableBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USelectableBase();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	AInteractableBase* Interactable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Scripts", meta = (AllowPrivateAccess = "true"))
	UAnimationComponent* AnimationComponent;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void OnInteractPress(USceneComponent* Controller);
	virtual void OnInteractRelease();
};
