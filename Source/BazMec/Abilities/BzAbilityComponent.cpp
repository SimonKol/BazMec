// (c) Simon Kolciter


#include "BzAbilityComponent.h"

#include "BzAbility.h"
#include "BzEnergyComponent.h"

#define BZ_ABILITY_DIAG !UE_BUILD_SHIPPING

#if BZ_ABILITY_DIAG
#include "UObject/UObjectIterator.h"
#endif

UBzAbilityComponent::UBzAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

UBzAbilityComponent* UBzAbilityComponent::Get(AActor* Actor)
{
	return Actor ? Actor->FindComponentByClass<UBzAbilityComponent>() : nullptr;
}

FBzAbilityHandle UBzAbilityComponent::AddAbility(TSubclassOf<UBzAbility> AbilityType)
{
	OwnedAbilities.Add(FBzAbilityState{ .Instance = NewObject<UBzAbility>(this, AbilityType.Get()) });
	return FBzAbilityHandle { .Index = OwnedAbilities.Num() - 1 };
}

#if BZ_ABILITY_DIAG

class FBzAbilityCooldownWatch
{
public:
	static void Callback(IConsoleVariable* Variable)
	{
		const float Value = Variable->GetFloat();
		for (TObjectIterator<UBzAbilityComponent> It; It; ++It)
		{
			for (FBzAbilityState& State : It->OwnedAbilities)
			{
				State.Cooldown = FMath::Min(Value, State.Cooldown);
			}
		}
	}
};

static TAutoConsoleVariable<float> CVarBzAbilityCooldown(
	TEXT("Bz.ability.cooldown")
	, 0.f
	, TEXT("Non-zero value overrides ability cooldown durations")
	, FConsoleVariableDelegate::CreateStatic(&FBzAbilityCooldownWatch::Callback)
	, ECVF_Cheat);

static float GetAbilityCooldown(float Original)
{
	float Override = CVarBzAbilityCooldown.GetValueOnGameThread();
	return (Override > SMALL_NUMBER) ? Override : Original;
}
#else
#define GetAbilityCooldown(x) (x)
#endif

void UBzAbilityComponent::TryExecute(const FBzAbilityHandle& Handle, const FHitResult& Hit)
{
	if (CanExecute(Handle))
	{
		FBzAbilityState& State = OwnedAbilities[Handle.Index];
		if (State.Instance->bIsTargeted && TargetedAbility != Handle)
		{
			TargetedAbility = Handle;
			OnTargetingAbility.Broadcast(this, TargetedAbility);
		}
		else
		{
			EnergyComponent->Spend(State.Instance->EnergyCost);
			State.Instance->Execute(FBzStartAbilityParams {.Hit = Hit});
			State.Cooldown = GetAbilityCooldown(State.Instance->CooldownDuration);
			OnAbilityExecuted.Broadcast(this, Handle);
		}
	}
}

bool UBzAbilityComponent::CanExecute(const FBzAbilityHandle& Handle) const
{
	check(OwnedAbilities.IsValidIndex(Handle.Index));

	const FBzAbilityState& AbilityState = OwnedAbilities[Handle.Index];
	UBzAbility* AbilityInstance = AbilityState.Instance;
	check(AbilityInstance);
	check(AbilityInstance->GetOuter() == this);
	check(EnergyComponent);
	
	return (AbilityState.Cooldown <= 0.f) && EnergyComponent->CanSpend(AbilityInstance->EnergyCost) && AbilityInstance->CanExecute();
}

void UBzAbilityComponent::ConfirmTarget(const FHitResult& Hit)
{
	TryExecute(TargetedAbility, Hit);
	TargetedAbility.Reset();
	OnTargetingAbility.Broadcast(this, TargetedAbility);
}

void UBzAbilityComponent::StopTargeting()
{
	TargetedAbility.Reset();
	OnTargetingAbility.Broadcast(this, TargetedAbility);
}

void UBzAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (FBzAbilityState& State : OwnedAbilities)
	{
		if (State.Cooldown > 0.f)
		{
			State.Cooldown -= DeltaTime;
		}
	}
}

bool UBzAbilityComponent::GetAbilityState(const FBzAbilityHandle& Handle, FBzAbilityState& State) const
{
	if (OwnedAbilities.IsValidIndex(Handle.Index))
	{
		State = OwnedAbilities[Handle.Index];
		return true;
	}

	return false;
}

void UBzAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
	
	EnergyComponent = UBzEnergyComponent::Get(GetOwner());
}
