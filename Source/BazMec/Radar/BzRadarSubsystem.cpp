// (c) Simon Kolciter


#include "Radar/BzRadarSubsystem.h"

void UBzRadarSubsystem::Register(UBzRadarComponent* Item)
{
	ActiveComponents.AddUnique(Item);
	OnItemRegistered.Broadcast(Item);
}

void UBzRadarSubsystem::Unregister(UBzRadarComponent* Item)
{
	ActiveComponents.Remove(Item);
	OnItemUnregistered.Broadcast(Item);
}
