// (c) Simon Kolciter

#include "BzBuildInfo.h"

#if WITH_EDITOR
#define BZ_BUILDINFO "editor"
#else
#if __has_include("__buildinfo.h") // fingers crossed for this working on other compilers
#include "__buildinfo.h"
#endif
#ifndef BZ_BUILDINFO
#define BZ_BUILDINFO "..."
#endif
#pragma message ("*** BZ_BUILDINFO = " BZ_BUILDINFO)
#endif

FText FBzBuildInfo::GetText()
{
	return INVTEXT(BZ_BUILDINFO);
}

const TCHAR* FBzBuildInfo::GetShortString()
{
	return TEXT(BZ_BUILDINFO);
}

FText FBzBuildInfo::GetBuildTime()
{
	FDateTime DateTime;
#ifdef BZ_BUILDTIME
	#pragma message ("*** BZ_BUILDTIME = " BZ_BUILDTIME)
	FDateTime::ParseIso8601(TEXT(BZ_BUILDTIME), DateTime);
#else
	DateTime = FDateTime::MinValue();
#endif

	return FText::FromString(FString::Printf(TEXT("%04d-%02d-%02d.%02d-%02d-%02d (UTC)")
		, DateTime.GetYear(), DateTime.GetMonth(), DateTime.GetDay()
		, DateTime.GetHour(), DateTime.GetMinute(), DateTime.GetSecond()));
}
