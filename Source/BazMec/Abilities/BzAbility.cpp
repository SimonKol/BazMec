// (c) Simon Kolciter


#include "BzAbility.h"
#include "BzAbilityComponent.h"

UWorld* UBzAbility::GetWorld() const
{
	if (HasAllFlags(RF_ClassDefaultObject))
	{
		return nullptr;
	}

	return GetOuter()->GetWorld();
}

UBzAbilityComponent* UBzAbility::GetOwningComponent() const
{
	return Cast<UBzAbilityComponent>(GetOuter());
}

AActor* UBzAbility::GetOwningActor() const
{
	return GetOwningComponent()->GetOwner();
}
