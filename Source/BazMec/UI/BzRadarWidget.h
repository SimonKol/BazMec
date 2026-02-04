// (c) Simon Kolciter

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "BzRadarWidget.generated.h"

class APawn;
/**
 * 
 */
UCLASS()
class BAZMEC_API UBzRadarWidget : public UWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category=Bz)
	void SetPawn(APawn* Pawn);

	virtual void SynchronizeProperties() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	UPROPERTY(EditAnywhere, Category=Bz)
	TObjectPtr<const class UBzRadarConfigAsset> Config;

protected:

	virtual TSharedRef<SWidget> RebuildWidget() override;

private:

	TSharedPtr<class SBzRadarMap> MyWidget;

	UPROPERTY()
	TObjectPtr<APawn> OwningPawn;
};
