// (c) Simon Kolciter

#pragma once

#include "CoreMinimal.h"
#include "BzProjectileType.generated.h"

class ABzProjectileActor;

USTRUCT(BlueprintType)
struct FBzProjectileType : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	float Speed = 1000;

	// Gravity multiplier. 0: no gravity, 1: ~Earth (9.8 m/s/s)
	UPROPERTY(EditDefaultsOnly)
	float Gravity = 0;

	UPROPERTY(EditDefaultsOnly)
	float DirectDamage = 0;

	UPROPERTY(EditDefaultsOnly)
	float SplashDamage = 0;

	UPROPERTY(EditDefaultsOnly)
	float SplashDamageRange = 0;

	UPROPERTY(EditDefaultsOnly, meta = (ShowTreeView = true))
	TSubclassOf<ABzProjectileActor> ProjectileActor;
};
