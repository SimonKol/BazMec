// (c) Simon Kolciter

#pragma once

#include "CoreMinimal.h"

class FBzBuildInfo
{
public:
	
	static FText GetText();
	static const TCHAR* GetShortString();

	static constexpr const TCHAR* GetConfiguration()
	{
#if UE_BUILD_SHIPPING
		return TEXT("Shipping");
#elif UE_BUILD_DEVELOPMENT
		return TEXT("Development");
#elif UE_BUILD_TEST
		return TEXT("Test");
#elif UE_BUILD_DEBUG
		return TEXT("Debug");
#else
		return TEXT("Unknown");
#endif
	}

	static FText GetBuildTime();
};
