// (c) Simon Kolciter

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BzGameLoopConfigAsset.generated.h"

class ABzPickup;

/**
 * 
 */
UCLASS(BlueprintType)
class BAZMEC_API UBzGameLoopConfigAsset : public UDataAsset
{
	GENERATED_BODY()

public:

#if WITH_EDITOR
	EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif
	
	UPROPERTY(EditDefaultsOnly, Category=Bz, meta = (ShowTreeView = true))
	TSubclassOf<class ABzEventActor> EventActorClass;
	
	UPROPERTY(EditDefaultsOnly, Category=Bz, meta = (ShowTreeView = true))
	TSubclassOf<class APawn> BotClass;

	UPROPERTY(EditDefaultsOnly, Category=Bz, meta = (ShowTreeView = true))
	TSet<TSubclassOf<ABzPickup>> PickupClasses;

	UFUNCTION(BlueprintPure, Category=Bz)
	TSubclassOf<ABzPickup> GetRandomPickup() const;
};
