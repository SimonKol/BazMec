// (c) Simon Kolciter

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BzBotController.generated.h"

/**
 * 
 */
UCLASS()
class BAZMEC_API ABzBotController : public AAIController
{
	GENERATED_BODY()

public:

	virtual void UpdateControlRotation(float DeltaTime, bool bUpdatePawn) override {}

	AActor* GetAttackTarget() const { return AttackTarget; }

	UFUNCTION(BlueprintCallable, Category=Bz)
	void SetAttackTarget(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category=Bz)
	AActor* GetAttackTarget() { return AttackTarget; }

	UFUNCTION(BlueprintCallable, Category=Bz)
	void RequestNewTarget();

protected:

	UFUNCTION(BlueprintImplementableEvent, Category=Bz)
	void BP_RequestedNewTarget();
private:

	UFUNCTION()
	void OnTargetHealthChanged(class UBzHealthComponent* Sender);

	UPROPERTY()
	TObjectPtr<AActor> AttackTarget;
	
};
