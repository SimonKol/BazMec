// (c) Simon Kolciter

#pragma once

#include "CoreMinimal.h"
#include "Abilities/BzPickup.h"
#include "BzPickup_Ammo.generated.h"

/**
 * 
 */
UCLASS()
class BAZMEC_API ABzPickup_Ammo : public ABzPickup
{
	GENERATED_BODY()

public:

	virtual bool CanPickup_Implementation(AActor* User) const override;
	virtual void DoPickup_Implementation(AActor* User) override;

	UPROPERTY(EditDefaultsOnly, Category=Bz, meta = (GetOptions = "BzCombatSettings.GetProjectileTypes"))
	FName ProjectileType;

	UPROPERTY(EditDefaultsOnly, Category=Bz)
	int32 Amount = 0;
};
