// (c) Simon Kolciter

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BzSystemLibrary.generated.h"

/**
 * 
 */
UCLASS()
class BAZMEC_API UBzSystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintPure, Category=Bz)
	static int32 GetShaderPrecompilesRemaining();

	UFUNCTION(BlueprintCallable, Category=Bz)
	static void ExploreFolder(const FString& Path);
};
