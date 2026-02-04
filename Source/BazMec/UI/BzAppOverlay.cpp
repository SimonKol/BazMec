// (c) Simon Kolciter


#include "BzAppOverlay.h"
#include "BzBuildInfo.h"

#include "Styling/SlateStyleMacros.h"

class SBzAppOverlay : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SBzAppOverlay) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

private:

	TSharedPtr<STextBlock> TickRateText;
	double LastMeasureTime = FPlatformTime::Seconds();
	int32 TickCounter = 0;
};

void SBzAppOverlay::Construct(const FArguments& InArgs)
{
	FSlateFontInfo Font = DEFAULT_FONT("Mono", 10);
	Font.OutlineSettings.OutlineSize = 1;

	ChildSlot
		[
			SNew(SOverlay)
				+ SOverlay::Slot()
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Top)
				.Padding(8)
				[
					SNew(STextBlock)
						.Font(Font)
						.ColorAndOpacity(FColor::White)
						.Text(FText::Format(INVTEXT("{0}.{1}@UE{2}")
							, FBzBuildInfo::GetText()
							, FText::FromString(FBzBuildInfo::GetConfiguration())
							, FText::FromString(ENGINE_VERSION_STRING)))
						.ToolTipText(FBzBuildInfo::GetBuildTime())
				]
				+ SOverlay::Slot()
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Top)
				.Padding(8)
				[
					SAssignNew(TickRateText, STextBlock)
						.Font(Font)
						.ColorAndOpacity(FColor::White)
#if WITH_EDITOR
						.Text(INVTEXT("123"))
#endif
				]
		];
}

void SBzAppOverlay::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	++TickCounter;

	const double Now = FPlatformTime::Seconds();
	const double Diff = Now - LastMeasureTime;

	if (Diff >= 1.0)
	{
		LastMeasureTime = Now;
		TickRateText->SetText(FText::AsNumber((int32)(double(TickCounter) / Diff)));
		TickCounter = 0;
	}
}

UBzAppOverlay::UBzAppOverlay()
{
	SetVisibilityInternal(ESlateVisibility::SelfHitTestInvisible);
}

void UBzAppOverlay::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyWidget.Reset();
}

TSharedRef<SWidget> UBzAppOverlay::RebuildWidget()
{
	MyWidget = SNew(SBzAppOverlay);
#if WITH_EDITOR
	MyWidget->SetCanTick(!IsDesignTime());
#endif
	return MyWidget.ToSharedRef();
}
