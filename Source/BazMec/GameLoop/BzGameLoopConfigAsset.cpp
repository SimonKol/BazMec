// (c) Simon Kolciter


#include "BzGameLoopConfigAsset.h"

#if WITH_EDITOR
#include "BzEventActor.h"
#include "Abilities/BzPickup.h"

#include "Misc/DataValidation.h"
#endif

#if WITH_EDITOR
EDataValidationResult UBzGameLoopConfigAsset::IsDataValid(class FDataValidationContext& Context) const
{
	if (Super::IsDataValid(Context) != EDataValidationResult::Invalid)
	{
		if (!EventActorClass)
		{
			Context.AddError(INVTEXT("Event actor class not set"));
			return EDataValidationResult::Invalid;
		}

		if (!BotClass)
		{
			Context.AddError(INVTEXT("Bot class not set"));
			return EDataValidationResult::Invalid;
		}

		if (PickupClasses.IsEmpty())
		{
			Context.AddError(INVTEXT("No pickup classes set"));
			return EDataValidationResult::Invalid;
		}

		for (const auto& Pickup : PickupClasses)
		{
			if (!Pickup)
			{
				Context.AddError(INVTEXT("Pickup class is null"));
				return EDataValidationResult::Invalid;
			}
		}
	}

	return EDataValidationResult::Valid;
}
#endif

TSubclassOf<ABzPickup> UBzGameLoopConfigAsset::GetRandomPickup() const
{
	if (PickupClasses.IsEmpty())
	{
		return nullptr;
	}

	return PickupClasses.Array()[FMath::Rand() % PickupClasses.Num()];
}
