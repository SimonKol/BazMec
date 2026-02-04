// (c) Simon Kolciter

#pragma once

#include "CoreMinimal.h"
#include "Abilities/BzAbility.h"
#include "BzAbility_SpeedBoost.generated.h"

/**
 * 
 */
UCLASS()
class BAZMEC_API UBzAbility_SpeedBoost : public UBzAbility
{
	GENERATED_BODY()
	
public:
	void Execute_Implementation(const FBzStartAbilityParams& Params) override;

	UPROPERTY(EditDefaultsOnly, Category = Bz)
	float EffectDuration = 1;
};
