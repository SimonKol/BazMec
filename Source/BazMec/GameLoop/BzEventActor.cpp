// (c) Simon Kolciter


#include "BzEventActor.h"
 
ABzEventActor::ABzEventActor()
{
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void ABzEventActor::Trigger()
{
	OnTriggered.ExecuteIfBound(this);
}

void ABzEventActor::SetEnabled(bool bValue)
{
	if (bValue != bEnabled)
	{
		BP_OnSetEnabled(bEnabled = bValue);
	}
}

void ABzEventActor::BeginPlay()
{
	Super::BeginPlay();

	BP_OnSetEnabled(bEnabled);
}
