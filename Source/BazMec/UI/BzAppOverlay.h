// (c) Simon Kolciter

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "BzAppOverlay.generated.h"

/**
 * 
 */
UCLASS()
class BAZMEC_API UBzAppOverlay : public UWidget
{
	GENERATED_BODY()

public:

	UBzAppOverlay();
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

protected:

	virtual TSharedRef<SWidget> RebuildWidget() override;
	
private:

	TSharedPtr<SWidget> MyWidget;
};
