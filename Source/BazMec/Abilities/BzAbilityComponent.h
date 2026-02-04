// (c) Simon Kolciter

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BzAbilityTypes.h"
#include "BzAbilityComponent.generated.h"

class UBzAbility;

USTRUCT(BlueprintType)
struct FBzAbilityState
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UBzAbility> Instance;

	UPROPERTY(BlueprintReadOnly)
	float Cooldown = -1;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BAZMEC_API UBzAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UBzAbilityComponent();

	UFUNCTION(BlueprintPure, Category=Bz, DisplayName="GetBzAbilityComponent")
	static UBzAbilityComponent* Get(AActor* Actor);

	UFUNCTION(BlueprintPure, Category=Bz)
	static bool IsValid(const FBzAbilityHandle& Handle) { return Handle.IsValid(); }

	UFUNCTION(BlueprintPure, Category=Bz)
	static bool Equals(const FBzAbilityHandle& A, const FBzAbilityHandle& B) { return A == B; }

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintPure, Category=Bz)
	bool GetAbilityState(const FBzAbilityHandle& Handle, FBzAbilityState& State) const;

	template<typename TAbility>
	FBzAbilityHandle AddAbility() { return AddAbility(TAbility::StaticClass()); }

	FBzAbilityHandle AddAbility (TSubclassOf<UBzAbility> AbilityType);

	void TryExecute(const FBzAbilityHandle& Handle, const FHitResult& Hit = FHitResult());
	bool CanExecute(const FBzAbilityHandle& Handle) const;

	bool IsTargetingAbility() const { return TargetedAbility.IsValid(); }
	const FBzAbilityHandle& GetTargetedAbility() const { return TargetedAbility; }
	void ConfirmTarget(const FHitResult& Hit);
	void StopTargeting();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBzAbilityHandleDelegate, UBzAbilityComponent*, Sender, FBzAbilityHandle, Handle);

	UPROPERTY(BlueprintAssignable, Category=Bz)
	FBzAbilityHandleDelegate OnTargetingAbility;

	UPROPERTY(BlueprintAssignable, Category=Bz)
	FBzAbilityHandleDelegate OnAbilityExecuted;

protected:

	virtual void BeginPlay() override;

private:

	friend class FBzAbilityCooldownWatch;

	UPROPERTY()
	TObjectPtr<class UBzEnergyComponent> EnergyComponent;

	UPROPERTY()
	TArray<FBzAbilityState> OwnedAbilities;

	FBzAbilityHandle TargetedAbility;
};
