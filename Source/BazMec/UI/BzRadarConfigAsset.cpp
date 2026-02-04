// (c) Simon Kolciter


#include "BzRadarConfigAsset.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#if WITH_EDITOR
EDataValidationResult UBzRadarConfigAsset::IsDataValid(class FDataValidationContext& Context) const
{
	if (Super::IsDataValid(Context) != EDataValidationResult::Invalid)
	{
		if (MinimumRange < SMALL_NUMBER)
		{
			Context.AddError(INVTEXT("Minimum range must be >0"));
			return EDataValidationResult::Invalid;
		}
	}

	return EDataValidationResult::Valid;
}
#endif
