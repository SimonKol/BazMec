// (c) Simon Kolciter


#include "BzMathLibrary.h"
#include "BzMathTypes.h"

FVector UBzMathLibrary::PredictTargetPosition(const FVector& TargetPos, const FVector& TargetSpeed, const FVector& MuzzlePos, float ProjectileSpeed)
{
	if (ProjectileSpeed < SMALL_NUMBER)
	{
		return TargetPos;
	}

	auto Root_a2_b2 = [](float a, float b)
		{
			return FMath::Sqrt(FMath::Abs(a * a - b * b));
		};

	const float Dist = FVector::Dist(MuzzlePos, TargetPos);
	const float Mult = Dist / Root_a2_b2(TargetSpeed.Size(), ProjectileSpeed);
	return TargetPos + TargetSpeed * Mult;
}

FTransform UBzMathLibrary::UpdateSpringPoint(FBzSpringPointState& State, const FTransform& Target, const FBzSpringPointUpdateParams& Params, float DeltaSeconds)
{
	const FVector Difference = Target.GetLocation() - State.LastTargetPos;
	State.LastTargetPos = Target.GetLocation();

	const FVector DifferenceSpeed = (DeltaSeconds > KINDA_SMALL_NUMBER) ? (Difference/DeltaSeconds) : FVector::ZeroVector;

	constexpr float Sign = -1;
	const FVector TargetOffset = DifferenceSpeed * Params.SpeedToOffset * Sign;

	State.Offset = FMath::VInterpTo(State.Offset, TargetOffset, DeltaSeconds, Params.OffsetInterpSpeed);
	State.WorldRotation = FMath::RInterpTo(State.WorldRotation, Target.Rotator(), DeltaSeconds, Params.RotInterpSpeed);

	return FTransform
	(
		State.WorldRotation,
		Target.GetLocation() + State.Offset
	);
}
