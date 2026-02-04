// (c) Simon Kolciter


#include "BzRadarComponent.h"
#include "BzRadarSubsystem.h"

UBzRadarComponent::UBzRadarComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}


// Called when the game starts
void UBzRadarComponent::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetSubsystem<UBzRadarSubsystem>()->Register(this);
}


void UBzRadarComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetSubsystem<UBzRadarSubsystem>()->Unregister(this);
}

