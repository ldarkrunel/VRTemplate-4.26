// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AnimationComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAnimationFinished);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAnimationStarted);

UENUM()
enum class EAnimType : uint8
{
	NONE,
	EASEIN,
	EASEOUT,
	EASEINOUT
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class VRTEMPLATE_API UAnimationComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAnimationComponent();

	bool IsAnimating = false;
	bool ReverseAnim = false;

	UPROPERTY(BlueprintReadOnly, BlueprintReadOnly, Category = "Location", meta = (AllowPrivateAccess = "true"))
	FTransform StartTransform;

	UPROPERTY(BlueprintReadOnly, BlueprintReadOnly, Category = "Location", meta = (AllowPrivateAccess = "true"))
	FTransform DestinationTransform;

	UPROPERTY(BlueprintReadOnly, BlueprintReadOnly, Category = "Location", meta = (AllowPrivateAccess = "true"))
	USceneComponent* DestObject = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	bool ConstantSpeedIgnoringDistance = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	float AnimSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	FVector DestinationScale;

	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnAnimationFinished OnAnimFinished;

	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnAnimationStarted OnAnimStarted;

	float TotalTime = 0.f;
	float Alpha = 0.f;
	EAnimType AnimType;

	void CreateAnimation(FTransform destination, float AnimSpeed, EAnimType Type);

	void CreateAnimation(FTransform InitalPosition, FTransform Destination, EAnimType Type);

	void CreateAnimation(FTransform InitalPosition, USceneComponent* PlayerHand, EAnimType Type);

	void Start();
	void Reverse();
	void Stop();

	FTransform GetAlphaValue(FTransform CurrentLocation);

private:
	void AnimateComponent(float DeltaTime);

	float EaseInOut(float t);
	float EaseIn(float t);
	float EaseOut(float t);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
