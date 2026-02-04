// (c) Simon Kolciter

#include "BzDamageDealerComponent.h"

UBzDamageDealerComponent* UBzDamageDealerComponent::Get(AActor* Actor)
{
	return Actor ? Actor->FindComponentByClass<UBzDamageDealerComponent>() : nullptr;
}

void UBzDamageDealerComponent::CallDamageDealt()
{
	OnDamageDealt.Broadcast();
}
