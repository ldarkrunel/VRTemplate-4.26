// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCController.h"
#include "Kismet/GameplayStatics.h"
#include "VRCharacter.h"




// Sets default values for this component's properties
UNPCController::UNPCController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UNPCController::BeginPlay()
{
	Super::BeginPlay();

	// ...
	VRPlayer = Cast<AVRCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

// Called every frame
void UNPCController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float distance = DistanceCalculator(VRPlayer->GetActorLocation(), GetOwner()->GetActorLocation());

	SetDistanceState(distance);
}


EDistanceState UNPCController::GetAnimState()
{
	return DistanceState;
}

void UNPCController::IncreaseDistanceRange(EDistanceState DistanceValue)
{
	MediumDistanceValue = MediuemDistanceDefault;
	CloseDistanceValue = CloseDistanceDefault;

	switch (DistanceState)
	{
	case EDistanceState::Close:
		CloseDistanceValue += IncreaseDistanceValue;
		break;

	case EDistanceState::Mediuem:
		MediumDistanceValue += IncreaseDistanceValue;
		break;
	}
}

float UNPCController::DistanceCalculator(FVector Vector_A, FVector Vector_B)
{
	return (Vector_A - Vector_B).Size();
}

void UNPCController::SetDistanceState(float DistanceToPlayer)
{
	if (DistanceToPlayer <= CloseDistanceValue)
		SetAnimation(EDistanceState::Close);

	else if (DistanceToPlayer <= MediumDistanceValue)
		SetAnimation(EDistanceState::Mediuem);

	else
		SetAnimation(EDistanceState::Far);
}

void UNPCController::SetAnimation(EDistanceState CurrentState)
{
	// No change in animation state needed
	if (CurrentState == DistanceState)
		return;

	DistanceState = CurrentState;

	if (CurrentState == EDistanceState::Close)
	{
		UE_LOG(LogTemp, Warning, TEXT("Close distance"));
		bIsIdle = false;
	}
	else if (CurrentState == EDistanceState::Mediuem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Medium distance"));
		bIsIdle = true;
	}
	else if (CurrentState == EDistanceState::Far)
	{
		UE_LOG(LogTemp, Warning, TEXT("Far distance"));
		bIsIdle = true;
	}
}
