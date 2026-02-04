// (c) Simon Kolciter


#include "BzEnvQueryContext_AttackTarget.h"

#include "../BzBotCharacter.h"
#include "../BzBotController.h"

#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

void UBzEnvQueryContext_AttackTarget::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	const ABzBotCharacter* BotCharacter = Cast<ABzBotCharacter>(QueryInstance.Owner.Get());
	if (!BotCharacter)
	{
		return;
	}

	const ABzBotController* BotController = BotCharacter->GetController<ABzBotController>();
	if (!(BotController && BotController->GetAttackTarget()))
	{
		return;
	}

	UEnvQueryItemType_Actor::SetContextHelper(ContextData, BotController->GetAttackTarget());
}