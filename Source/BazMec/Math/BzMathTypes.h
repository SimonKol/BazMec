// (c) Simon Kolciter

#pragma once

#include "CoreMinimal.h"
#include "BzMathTypes.generated.h"

USTRUCT(BlueprintType)
struct FBzSpringPointState
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FRotator WorldRotation = FRotator::ZeroRotator;

	UPROPERTY(BlueprintReadOnly)
	FVector Offset = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly)
	FVector LastTargetPos = FVector::ZeroVector;
};

USTRUCT(BlueprintType)
struct FBzSpringPointUpdateParams
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float SpeedToOffset = 0.01;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float OffsetInterpSpeed = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float RotInterpSpeed = 10;
};
