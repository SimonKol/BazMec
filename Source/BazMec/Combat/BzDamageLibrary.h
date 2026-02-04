// (c) Simon Kolciter

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BzDamageLibrary.generated.h"

struct FBzDamageResult
{
	bool bHasAny = false;
};

UCLASS()
class BAZMEC_API UBzDamageLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	static FBzDamageResult TryDealDamageSimple(AActor* Target, float Damage);
	static FBzDamageResult TryDealSplashDamageSimple(UWorld* World, FVector Location, float Range, float Damage, ECollisionChannel TraceChannel, AActor* Exclude);
};