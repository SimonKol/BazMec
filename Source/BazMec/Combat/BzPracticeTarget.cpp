// (c) Simon Kolciter


#include "BzPracticeTarget.h"

#include "Components/SceneComponent.h"

ABzPracticeTarget::ABzPracticeTarget()
{
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("Root"));
}

void ABzPracticeTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsMoving)
	{
		return;
	}

	const FVector CurrentLocation = GetActorLocation();
	const FVector Diff = MoveTarget - CurrentLocation;
	const float Dist2 = Diff.SquaredLength();

	if (Dist2 <= FMath::Square(DeltaTime * MovementSpeed))
	{
		SetActorLocation(MoveTarget);
		RootComponent->ComponentVelocity = FVector::ZeroVector;
		bIsMoving = false;

		OnReachedTarget.Broadcast(this);
	}
	else
	{
		const FVector Speed = MovementSpeed * Diff / FMath::Sqrt(Dist2);
		SetActorLocation(CurrentLocation + Speed * DeltaTime);
		RootComponent->ComponentVelocity = Speed;
	}
}

void ABzPracticeTarget::SetMoveTarget(const FVector& Location)
{
	MoveTarget = Location;
	bIsMoving = true;
}
