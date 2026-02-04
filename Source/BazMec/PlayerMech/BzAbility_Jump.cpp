// (c) Simon Kolciter

#include "BzAbility_Jump.h"

#include "BzPlayerMech.h"

bool UBzAbility_Jump::CanExecute_Implementation() const
{
	return GetOwningActorChecked<ABzPlayerMech>()->CanJump();
}

void UBzAbility_Jump::Execute_Implementation(const FBzStartAbilityParams&)
{
	GetOwningActorChecked<ABzPlayerMech>()->Jump();
}
