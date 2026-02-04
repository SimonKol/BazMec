// (c) Simon Kolciter

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "BzCombatSettings.generated.h"

/**
 * 
 */
UCLASS(Config = BzCombat, DefaultConfig)
class BAZMEC_API UBzCombatSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:

	virtual FName GetCategoryName() const override { return "BazMec"; }
	static const UBzCombatSettings* Get();

#if WITH_EDITOR
	UFUNCTION()
	static TArray<FName> GetProjectileTypes();
#endif

	UPROPERTY(Config, EditAnywhere)
	TEnumAsByte<ECollisionChannel> ProjectileChannel = ECollisionChannel::ECC_MAX;

	UPROPERTY(config, EditAnywhere, meta = (RowType = "/Script/BazMec.BzProjectileType"))
	TSoftObjectPtr<class UDataTable> ProjectileTable;
};
