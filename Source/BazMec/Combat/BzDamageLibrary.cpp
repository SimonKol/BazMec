// (c) Simon Kolciter

#include "BzDamageLibrary.h"

#include "BzHealthComponent.h"

#include "Engine/OverlapResult.h"

FBzDamageResult UBzDamageLibrary::TryDealDamageSimple(AActor* Target, float Damage)
{
	if (UBzHealthComponent* HealthComp = UBzHealthComponent::Get(Target))
	{
		HealthComp->DealDamage(Damage);
		return FBzDamageResult { .bHasAny = true };
	}

	return FBzDamageResult();
}

FBzDamageResult UBzDamageLibrary::TryDealSplashDamageSimple(UWorld* World, FVector Location, float Range, float Damage, ECollisionChannel TraceChannel, AActor* Exclude)
{
	FBzDamageResult Result;
	TArray<FOverlapResult> Overlaps;
	World->OverlapMultiByChannel(Overlaps, Location, FQuat::Identity, TraceChannel, FCollisionShape::MakeSphere(Range));

	for (const FOverlapResult& Overlap : Overlaps)
	{
		UPrimitiveComponent* OverlapPrimitive = Overlap.GetComponent();

		if (OverlapPrimitive && OverlapPrimitive->GetOwner() != Exclude)
		{
			FVector ApproxHitPoint;
			const float Distance = OverlapPrimitive->GetClosestPointOnCollision(Location, ApproxHitPoint);
			if (Distance >= 0.f)
			{
				const float RelDistance = 1 - FMath::Min(Distance / Range, 1.f);
				if (TryDealDamageSimple(Overlap.GetActor(), Damage * RelDistance).bHasAny)
				{
					Result.bHasAny = true;
				}
			}
		}
	}

	return Result;
}
