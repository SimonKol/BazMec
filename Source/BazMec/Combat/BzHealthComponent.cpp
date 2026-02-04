// (c) Simon Kolciter


#include "BzHealthComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBzHealth, Log, All);

void UBzHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = InitialHealth;
	OnStateChanged.Broadcast(this);
}

UBzHealthComponent* UBzHealthComponent::Get(AActor* Actor)
{
	return Actor ? Actor->FindComponentByClass<UBzHealthComponent>() : nullptr;
}

namespace { constexpr float HealthThreshold = SMALL_NUMBER; }

void UBzHealthComponent::DealDamage(float Amount)
{
	if (!bIsAlive)
	{
		return;
	}

	CurrentHealth -= Amount;
	if (CurrentHealth <= 0.f)
	{
		CurrentHealth = 0;
		bIsAlive = false;
	}

	if (Amount >= HealthThreshold)
	{
		UE_LOG(LogBzHealth, Verbose, TEXT("Dealt damage [-%g ~> %g] to %s"), Amount, CurrentHealth, *GetOwner()->GetName());
		OnStateChanged.Broadcast(this);
	}
}

void UBzHealthComponent::RestoreHealth(float Amount)
{
	if (!bIsAlive)
	{
		return;
	}

	const float OldHealth = CurrentHealth;
	CurrentHealth = FMath::Min(CurrentHealth + Amount, InitialHealth);
	if (FMath::Abs(OldHealth - CurrentHealth) >= HealthThreshold)
	{
		UE_LOG(LogBzHealth, Verbose, TEXT("Restored health [+%g ~> %g] to %s"), Amount, CurrentHealth, *GetOwner()->GetName());
		OnStateChanged.Broadcast(this);
	}
}

int32 UBzHealthComponent::GetHealthQuadrant() const
{
	return FMath::Clamp(4 * CurrentHealth / InitialHealth, 0, 3);
}

