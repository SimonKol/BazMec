// (c) Simon Kolciter

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "BzEnvQueryContext_AttackTarget.generated.h"

/**
 * 
 */
UCLASS()
class BAZMEC_API UBzEnvQueryContext_AttackTarget : public UEnvQueryContext
{
	GENERATED_BODY()
	
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
};
