// (c) Simon Kolciter

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "BzRadarConfigAsset.generated.h"

/**
 * 
 */
UCLASS()
class BAZMEC_API UBzRadarConfigAsset : public UDataAsset
{
	GENERATED_BODY()

public:

#if WITH_EDITOR
	EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif

	UPROPERTY(EditDefaultsOnly, Category=Bz)
	float MinimumRange = 10000;

	UPROPERTY(EditDefaultsOnly, Category=Bz, meta = (Categories = Radar))
	TMap<FGameplayTag, FSlateBrush> Icons;
	
};
