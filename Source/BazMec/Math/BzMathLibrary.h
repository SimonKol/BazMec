// (c) Simon Kolciter

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BzMathLibrary.generated.h"

/**
 * 
 */
UCLASS()
class BAZMEC_API UBzMathLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintPure, Category=Bz)
	static FVector PredictTargetPosition(const FVector& TargetPos, const FVector& TargetSpeed, const FVector& MuzzlePos, float ProjectileSpeed);

	UFUNCTION(BlueprintCallable, Category=Bz)
	static FTransform UpdateSpringPoint(UPARAM(ref) struct FBzSpringPointState& State, const FTransform& Target, const struct FBzSpringPointUpdateParams& Params, float DeltaSeconds);
};
