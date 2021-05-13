// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemHUD.generated.h"

class UTextRenderComponent;
class AInteractableBase;

UCLASS()
class VRTEMPLATE_API AItemHUD : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemHUD();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetVisible(bool BVisible);

	void SetInteractable(AInteractableBase* Interactable);

	void Initialize(FString Name, float textSize);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UTextRenderComponent* DisplayTextComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
	FString ItemName = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
	float TextSize = 20.f;

	FTransform DisplayTextSpawnLocation;

	AInteractableBase* Interactable;

};
