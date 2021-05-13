// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableHUD.generated.h"

class UTextRenderComponent;
class AInteractableBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VRTEMPLATE_API UInteractableHUD : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractableHUD();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Initialize(UTextRenderComponent* TextRenderComponent); //add any additional components later

	void SetVisibility(bool bVisible);

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
	FString ItemName = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
	float TextSize = 20.f;

	UTextRenderComponent* DisplayTextComponent;

	AInteractableBase* Interactable;
};
