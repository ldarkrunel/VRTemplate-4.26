#pragma once
#include "IInteractable.generated.h"

class UMotionControllerComponent;

UINTERFACE(MinimalAPI, Blueprintable)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

class IInteractable {

	GENERATED_IINTERFACE_BODY()

public:

	float MaxRaycastDistance;

	//this is a comment just to add something to commit
		//this is a comment just to add something to commit
		//this is a comment just to add something to commit
		//this is a comment just to add something to commit

	UFUNCTION()
	virtual void OnRaycastStart();
	UFUNCTION()
	virtual void OnRaycastEnd();
	UFUNCTION()
	virtual void OnSelectStart(USceneComponent* OwnerPosition);
	UFUNCTION()
	virtual void OnSelectEnd();
	UFUNCTION()
	virtual void OnOverlapStart();
	UFUNCTION()
	virtual void OnOverlapEnd();
	UFUNCTION()
	virtual void Highlight(bool bHighlight);
};
