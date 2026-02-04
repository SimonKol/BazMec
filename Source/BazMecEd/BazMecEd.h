// (c) Simon Kolciter

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

class FBazMacEdModule : public IModuleInterface
{
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};