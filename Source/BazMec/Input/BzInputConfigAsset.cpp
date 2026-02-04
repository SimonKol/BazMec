// (c) Simon Kolciter


#include "BzInputConfigAsset.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#if WITH_EDITOR
EDataValidationResult UBzInputConfigAsset::IsDataValid(class FDataValidationContext& Context) const
{
	if (Super::IsDataValid(Context) != EDataValidationResult::Invalid)
	{
		if (!(PrimaryAction && SecondaryAction && PrimaryAbility && SecondaryAbility))
		{
			Context.AddError(INVTEXT("All input actions must be set"));
			return EDataValidationResult::Invalid;
		}
	}

	return EDataValidationResult::Valid;
}
#endif
