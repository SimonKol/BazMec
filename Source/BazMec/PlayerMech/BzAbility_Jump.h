// (c) Simon Kolciter

#pragma once

#include "CoreMinimal.h"
#include "Abilities/BzAbility.h"
#include "BzAbility_Jump.generated.h"

/**
 * 
 */
UCLASS()
class BAZMEC_API UBzAbility_Jump : public UBzAbility
{
	GENERATED_BODY()

	virtual bool CanExecute_Implementation() const override;
	virtual void Execute_Implementation(const FBzStartAbilityParams& Params) override;
};
