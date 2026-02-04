// (c) Simon Kolciter

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BzAbility.generated.h"

USTRUCT(BlueprintType)
struct FBzStartAbilityParams
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FHitResult Hit;
};
/**
 * 
 */
UCLASS(Blueprintable, Config = BzGame, DefaultConfig)
class BAZMEC_API UBzAbility : public UObject
{
	GENERATED_BODY()
	
public:

	virtual UWorld* GetWorld() const override;

	UFUNCTION(BlueprintNativeEvent, Category=Bz)
	bool CanExecute() const;
	virtual bool CanExecute_Implementation() const { return true; }

	UFUNCTION(BlueprintNativeEvent, Category=Bz)
	void Execute(const FBzStartAbilityParams& Params);
	virtual void Execute_Implementation(const FBzStartAbilityParams& Params) {}

	UPROPERTY(EditDefaultsOnly, Category=Bz)
	bool bIsTargeted = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Bz, Config)
	float EnergyCost = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Bz, Config)
	float CooldownDuration = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Bz, Config)
	FText DisplayLabel;

protected:

	class UBzAbilityComponent* GetOwningComponent() const;
	class AActor* GetOwningActor() const;

	template<typename TActor>
	TActor* GetOwningActor() const { return Cast<TActor>(GetOwningActor()); }

	template<typename TActor>
	TActor* GetOwningActorChecked() const { return CastChecked<TActor>(GetOwningActor()); }
};
