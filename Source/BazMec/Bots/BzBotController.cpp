// (c) Simon Kolciter


#include "BzBotController.h"
#include "Combat/BzHealthComponent.h"

void ABzBotController::SetAttackTarget(AActor* InActor)
{
	AttackTarget = InActor;

	if (UBzHealthComponent* Health = UBzHealthComponent::Get(InActor))
	{
		Health->OnStateChanged.AddUniqueDynamic(this, &ABzBotController::OnTargetHealthChanged);
		OnTargetHealthChanged(Health);
	}
}

void ABzBotController::RequestNewTarget()
{
	BP_RequestedNewTarget();
}

void ABzBotController::OnTargetHealthChanged(UBzHealthComponent* Sender)
{
	check(Sender);
	if (!Sender->IsAlive())
	{
		Sender->OnStateChanged.RemoveDynamic(this, &ABzBotController::OnTargetHealthChanged);
		SetAttackTarget(nullptr);
	}
}
