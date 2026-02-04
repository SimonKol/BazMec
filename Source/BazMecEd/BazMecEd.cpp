// (c) Simon Kolciter

#include "BazMecEd.h"
#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"

void FBazMacEdModule::StartupModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

	TSharedRef<FPropertySection> Section = PropertyModule.FindOrCreateSection("Object", "_BazMec", INVTEXT("Baz Mec"));
	Section->AddCategory("Bz");
}

void FBazMacEdModule::ShutdownModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

	PropertyModule.RemoveSection("Object", "_BazMec");
}

IMPLEMENT_MODULE(FBazMacEdModule, BazMecEd);
