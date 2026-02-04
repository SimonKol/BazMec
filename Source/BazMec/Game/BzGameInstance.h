// (c) Simon Kolciter

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BzGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class BAZMEC_API UBzGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

#if WITH_EDITOR
	virtual void BeginCacheForCookedPlatformData(const class ITargetPlatform* TargetPlatform) override;
#endif
	
	virtual void Init() override;

private:

	UPROPERTY()
	FString CookedVersion = TEXT("-");
};
