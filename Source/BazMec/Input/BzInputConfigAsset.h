// (c) Simon Kolciter

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BzInputConfigAsset.generated.h"

class UInputAction;
/**
 * 
 */
UCLASS()
class BAZMEC_API UBzInputConfigAsset : public UDataAsset
{
	GENERATED_BODY()

public:

#if WITH_EDITOR
	EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif

	UPROPERTY(EditDefaultsOnly, Category=Bz, meta = (DisplayThumbnail = false))
	TObjectPtr<const UInputAction> PrimaryAction;
	
	UPROPERTY(EditDefaultsOnly, Category=Bz, meta = (DisplayThumbnail = false))
	TObjectPtr<const UInputAction> SecondaryAction;
	
	UPROPERTY(EditDefaultsOnly, Category=Bz, meta = (DisplayThumbnail = false))
	TObjectPtr<const UInputAction> PrimaryAbility;

	UPROPERTY(EditDefaultsOnly, Category=Bz, meta = (DisplayThumbnail = false))
	TObjectPtr<const UInputAction> SecondaryAbility;
};
