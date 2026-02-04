// (c) Simon Kolciter

#pragma once

#include "CoreMinimal.h"
#include "BzAbilityTypes.generated.h"

USTRUCT(BlueprintType)
struct FBzAbilityHandle
{
	GENERATED_BODY()

	UPROPERTY()
	int32 Index = INDEX_NONE;

	bool IsValid() const { return Index != INDEX_NONE; }
	void Reset() { Index = INDEX_NONE; }

	bool operator==(const FBzAbilityHandle& Other) const
	{
		return Index == Other.Index;
	}
};
