// (c) Simon Kolciter


#include "BzSystemLibrary.h"

#include "ShaderPipelineCache.h"

int32 UBzSystemLibrary::GetShaderPrecompilesRemaining()
{
	return FMath::Max((int)FShaderPipelineCache::NumPrecompilesRemaining(), 0);
}

void UBzSystemLibrary::ExploreFolder(const FString& Path)
{
	FPlatformProcess::ExploreFolder(*Path);
}
