// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimationComponent.h"
#include "Components/ActorComponent.h"

// Sets default values for this component's properties
UAnimationComponent::UAnimationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UAnimationComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UAnimationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsAnimating)
		AnimateComponent(DeltaTime);
}

void UAnimationComponent::CreateAnimation(FTransform destination, float Speed, EAnimType Type)
{
	StartTransform = GetOwner()->GetTransform();
	DestinationTransform = destination;
	AnimSpeed = Speed;
	ReverseAnim = false;
	Alpha = 0.f;
	AnimType = Type;
}

void UAnimationComponent::CreateAnimation(FTransform InitialTransform, FTransform Destination, EAnimType Type)
{
	Stop();

	StartTransform = InitialTransform;
	DestinationTransform = Destination;
	ReverseAnim = false;
	Alpha = 0.f;
	AnimType = Type;

	if (ConstantSpeedIgnoringDistance) {
		float distance = FVector::Distance(InitialTransform.GetLocation(), Destination.GetLocation());
		TotalTime = distance / AnimSpeed;
	}
}

void UAnimationComponent::CreateAnimation(FTransform InitialTransform, USceneComponent* PlayerHand, EAnimType Type)
{
	Stop();

	StartTransform = InitialTransform;
	DestObject = PlayerHand;
	ReverseAnim = false;
	Alpha = 0.f;
	AnimType = Type;

	if (ConstantSpeedIgnoringDistance) {
		float distance = FVector::Distance(InitialTransform.GetLocation(), DestObject->GetComponentLocation());
		TotalTime = distance / AnimSpeed;
	}
}

void UAnimationComponent::Start()
{
	OnAnimStarted.Broadcast();
	Alpha = 0.f;
	ReverseAnim = false;
	IsAnimating = true;
}

void UAnimationComponent::Reverse()
{
	OnAnimStarted.Broadcast();
	ReverseAnim = true;
	IsAnimating = true;
}

void UAnimationComponent::Stop()
{
	IsAnimating = false;
	Alpha = 0.f;
	DestObject = nullptr;
}

FTransform UAnimationComponent::GetAlphaValue(FTransform CurrentTransform)
{
	FVector CurrentLoc = CurrentTransform.GetLocation();
	FQuat Rot = CurrentTransform.GetRotation();
	FVector Scale = CurrentTransform.GetScale3D();

	if (AnimType == EAnimType::NONE) {
		CurrentLoc = FMath::Lerp(StartTransform.GetLocation(), DestinationTransform.GetLocation(), Alpha);
		Rot = FMath::Lerp(StartTransform.GetRotation(), DestinationTransform.GetRotation(), Alpha);
		Scale = FMath::Lerp(StartTransform.GetScale3D(), DestinationTransform.GetScale3D(), Alpha);
	}
	else if (AnimType == EAnimType::EASEIN) {
		CurrentLoc = FMath::Lerp(StartTransform.GetLocation(), DestinationTransform.GetLocation(), EaseIn(Alpha));
		Rot = FMath::Lerp(StartTransform.GetRotation(), DestinationTransform.GetRotation(), EaseIn(Alpha));
	}
	else if (AnimType == EAnimType::EASEOUT) {
		CurrentLoc = FMath::Lerp(StartTransform.GetLocation(), DestinationTransform.GetLocation(), EaseOut(Alpha));
		Rot = FMath::Lerp(StartTransform.GetRotation(), DestinationTransform.GetRotation(), EaseOut(Alpha));
	}
	else {
		CurrentLoc = FMath::Lerp(StartTransform.GetLocation(), DestinationTransform.GetLocation(), EaseInOut(Alpha));
		Rot = FMath::Lerp(StartTransform.GetRotation(), DestinationTransform.GetRotation(), EaseInOut(Alpha));
	}

	FTransform NewTransform = FTransform(Rot,CurrentLoc, Scale);

	return NewTransform;
}

void UAnimationComponent::AnimateComponent(float DeltaTime)
{
	FTransform CurrentLocation;

	if (DestObject != nullptr) {
		
		FTransform DestTransform = FTransform(DestObject->GetComponentRotation(), DestObject->GetComponentLocation(), DestinationScale);
		//UE_LOG(LogTemp, Warning, TEXT("Start Transform %s"), *StartTransform.GetScale3D().ToString());
		DestinationTransform = DestTransform;
	}

	if (!ReverseAnim) {
		ConstantSpeedIgnoringDistance ? Alpha += DeltaTime / TotalTime : Alpha += DeltaTime * AnimSpeed;
	}
	else {
		ConstantSpeedIgnoringDistance ? Alpha -= DeltaTime / TotalTime : Alpha -= DeltaTime * AnimSpeed;
	}

	if (Alpha > 1)
		Alpha = 1.0f;
	else if (Alpha < 0)
		Alpha = 0.f;

	CurrentLocation = GetAlphaValue(CurrentLocation);

	GetOwner()->SetActorTransform(CurrentLocation);

	if (Alpha >= 1.f || Alpha <= 0.f) {
		IsAnimating = false;
		DestObject = nullptr;
		OnAnimFinished.Broadcast();
	}
}

float UAnimationComponent::EaseInOut(float t)
{
	return t < .5f ? 4 * t * t * t : (t - 1) * (2 * t - 2) * (2 * t - 2) + 1;
}

float UAnimationComponent::EaseOut(float t)
{
	return 1 - pow(1 - t, 3);
}

float UAnimationComponent::EaseIn(float t)
{
	return t * t * t;
}



