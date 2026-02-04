// (c) Simon Kolciter


#include "BzProjectileActor.h"

// Sets default values
ABzProjectileActor::ABzProjectileActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

}

void ABzProjectileActor::ConfirmRelease()
{
	bIsActive = false;
}

void ABzProjectileActor::Start(const FVector& Location, const FRotator& Rotation)
{
	bIsActive = true;
	SetActorLocationAndRotation(Location, Rotation);
	BP_OnStarted(Location, Rotation);

}

void ABzProjectileActor::OnImpact(const FHitResult& Hit)
{
	BP_OnImpact(Hit);
}

void ABzProjectileActor::Finish()
{
	BP_OnFinished();

	if (bReleaseOnFinish)
	{
		bIsActive = false;
	}
}

