// (c) Simon Kolciter


#include "BzGameInstance.h"
#include "BzBuildInfo.h"

DEFINE_LOG_CATEGORY_STATIC(LogBzGameInstance, Display, All);

void UBzGameInstance::Init()
{
	Super::Init();

#if !WITH_EDITOR
	UE_LOG(LogBzGameInstance, Display, TEXT("Initializing {%s} with version\n\t[%s] vs build\n\t[%s]"), *GetPathName(), *CookedVersion, FBzBuildInfo::GetShortString());
#endif
}

#if WITH_EDITOR

// Build identifier passed to the cooker
struct FBzCookedVersionHelper
{
	FBzCookedVersionHelper()
	{
		FParse::Value(FCommandLine::Get(), TEXT("buildInfo"), BuildInfo);
	}

	FString BuildInfo;
};

void UBzGameInstance::BeginCacheForCookedPlatformData(const ITargetPlatform* TargetPlatform)
{
	Super::BeginCacheForCookedPlatformData(TargetPlatform);

	static FBzCookedVersionHelper Helper;
	CookedVersion = Helper.BuildInfo;

	UE_LOG(LogBzGameInstance, Display, TEXT("Cooking {%s} with version [%s]"), *GetPathName(), *CookedVersion);
}
#endif
