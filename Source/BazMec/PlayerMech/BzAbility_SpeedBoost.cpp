// (c) Simon Kolciter

#include "BzAbility_SpeedBoost.h"

#include "BzPlayerMech.h"

void UBzAbility_SpeedBoost::Execute_Implementation(const FBzStartAbilityParams&)
{
	GetOwningActorChecked<ABzPlayerMech>()->SetSpeedBoostDuration(EffectDuration);
}
