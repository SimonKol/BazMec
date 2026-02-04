// (c) Simon Kolciter

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BzHealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), PrioritizeCategories = ("Bz"))
class BAZMEC_API UBzHealthComponent : public UActorComponent
{
	GENERATED_BODY()


public:

	UFUNCTION(BlueprintPure, Category=Bz, DisplayName="GetBzHealthComponent")
	static UBzHealthComponent* Get(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category=Bz)
	void DealDamage(float Amount);

	UFUNCTION(BlueprintCallable, Category=Bz)
	void RestoreHealth(float Amount);

	UFUNCTION(BlueprintPure, Category=Bz)
	float GetHealth() const { return CurrentHealth; }

	UFUNCTION(BlueprintPure, Category=Bz)
	bool IsAlive() const { return bIsAlive; }

	UFUNCTION(BlueprintPure, Category=Bz)
	bool NeedsHeal() const { return CurrentHealth < InitialHealth; }

	UFUNCTION(BlueprintPure, Category=Bz)
	float GetInitialHealth() const { return InitialHealth; }

	// returns 0..3 (0: below 25%; 3: above 75%)
	UFUNCTION(BlueprintPure, Category=Bz)
	int32 GetHealthQuadrant() const;

	UPROPERTY(EditDefaultsOnly, Category=Bz)
	float InitialHealth = 100;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBzHealthComponentDelegate, UBzHealthComponent*, Sender);

	UPROPERTY(BlueprintAssignable, Category=Bz)
	FBzHealthComponentDelegate OnStateChanged;

protected:

	virtual void BeginPlay() override;

private:

	bool bIsAlive = true;
	float CurrentHealth = 0;
};
