// (c) Simon Kolciter

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BzEnergyComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), PrioritizeCategories = ("Bz"))
class BAZMEC_API UBzEnergyComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBzEnergyComponent();

	UFUNCTION(BlueprintPure, Category=Bz, DisplayName="GetBzEnergyComponent")
	static UBzEnergyComponent* Get(AActor* Actor);

	UFUNCTION(BlueprintPure, Category=Bz)
	float GetCurrentValue() const { return CurrentValue; }

	bool CanSpend(float Amount) const { return CurrentValue >= Amount; }
	bool Spend(float Amount);

	UFUNCTION(BlueprintPure, Category=Bz)
	bool NeedsRestore() const { return CurrentValue < MaxEnergy; }

	UFUNCTION(BlueprintCallable, Category=Bz)
	bool Restore(float Amount);

	UPROPERTY(EditDefaultsOnly, Category=Bz)
	float RechargeRate = 10;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	static constexpr float MaxEnergy = 100.f;
	float CurrentValue = MaxEnergy;
};
