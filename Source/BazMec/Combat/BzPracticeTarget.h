// (c) Simon Kolciter

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BzPracticeTarget.generated.h"

UCLASS()
class BAZMEC_API ABzPracticeTarget : public AActor
{
	GENERATED_BODY()
	
public:	
	ABzPracticeTarget();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category=Bz)
	void SetMoveTarget(const FVector& Location);

	UPROPERTY(EditDefaultsOnly, Category=Bz)
	float MovementSpeed = 1000;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBzPracticeTargetDelegate, ABzPracticeTarget*, Sender);

	UPROPERTY(BlueprintAssignable, Category=Bz)
	FBzPracticeTargetDelegate OnReachedTarget;

private:
	
	FVector MoveTarget = FVector::ZeroVector;
	bool bIsMoving = false;
};
