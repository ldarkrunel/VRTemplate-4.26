// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableBase.h"
#include "InteractableStatic.generated.h"

UCLASS()
class VRTEMPLATE_API AInteractableStatic : public AInteractableBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableStatic();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* InteractableMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnOverlapStart() override;
	virtual void OnOverlapEnd() override;
	virtual void OnSelectStart(USceneComponent* ControllerPosition) override;
	virtual void OnSelectEnd() override;
	virtual void OnRaycastStart() override;
	virtual void OnRaycastEnd() override;
	virtual void Highlight(bool bHighlight) override;

};
