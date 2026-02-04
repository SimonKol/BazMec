// (c) Simon Kolciter


#include "BzEnergyComponent.h"

// Sets default values for this component's properties
UBzEnergyComponent::UBzEnergyComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

UBzEnergyComponent* UBzEnergyComponent::Get(AActor* Actor)
{
	return Actor ? Actor->FindComponentByClass<UBzEnergyComponent>() : nullptr;
}

bool UBzEnergyComponent::Spend(float Amount)
{
	if (CanSpend(Amount))
	{
		CurrentValue -= Amount;
		return true;
	}

	return false;
}

bool UBzEnergyComponent::Restore(float Amount)
{
	const float OldValue = CurrentValue;
	
	CurrentValue = FMath::Min(CurrentValue + Amount, MaxEnergy);
	return OldValue != CurrentValue;
}

// Called every frame
void UBzEnergyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CurrentValue = FMath::Min(CurrentValue + DeltaTime * RechargeRate, MaxEnergy);
}

