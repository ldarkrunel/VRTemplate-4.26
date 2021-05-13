// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VRCharacter.h"
#include "NPCController.generated.h"

UENUM(BlueprintType)
enum EDistanceState
{
	Close   UMETA(DisplayName = "Close"),
	Mediuem UMETA(DisplayName = "Mediuem"),
	Far		UMETA(DisplayName = "Far"),
	NONE	UMETA(DisplayName = "None")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VRTEMPLATE_API UNPCController : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UNPCController();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Distance State", meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<EDistanceState> DistanceState = EDistanceState::NONE;

	UPROPERTY(BlueprintReadOnly, BlueprintReadOnly, Category = "VR Characters", meta = (AllowPrivateAccess = "true"))
	AVRCharacter* VRPlayer = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Distance State", meta = (AllowPrivateAccess = "true"))
	float IncreaseDistanceValue = 250.f;

	float DistanceCalculator(FVector Vector_A, FVector Vector_B);

	void SetDistanceState(float DistanceToPlayer);

	void SetAnimation(EDistanceState CurrentState);


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsIdle = true;

	UFUNCTION(BlueprintCallable, Category = "Animation State")
	EDistanceState GetAnimState();

	UFUNCTION(BlueprintCallable, Category = "Animation State")
	void IncreaseDistanceRange(EDistanceState DistanceValue);

	// used to reset the distance values.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation State")
	float MediuemDistanceDefault = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation State")
	float CloseDistanceDefault = 400.f;

	// Used to check the distance between the player and the NPC. This value is updated in the blueprint, and reset to default using the values above.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation State")
	float MediumDistanceValue = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation State")
	float CloseDistanceValue = 400.f;
		
};
