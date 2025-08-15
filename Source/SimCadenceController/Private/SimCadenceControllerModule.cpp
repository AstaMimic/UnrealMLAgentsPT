#include "SimCadenceControllerModule.h"
#include "Modules/ModuleManager.h"
#if WITH_EDITOR
	#include "ISettingsModule.h"
	#include "SimCadenceSettings.h"
	#define LOCTEXT_NAMESPACE "FSimCadenceControllerModule"
#endif

IMPLEMENT_MODULE(FSimCadenceControllerModule, SimCadenceController)

void FSimCadenceControllerModule::StartupModule()
{
#if WITH_EDITOR
	if (ISettingsModule* SettingsModule = FModuleManager::LoadModulePtr<ISettingsModule>(TEXT("Settings")))
	{
		SettingsModule->RegisterSettings(TEXT("Project"), TEXT("Plugins"), TEXT("SimCadence"),
			LOCTEXT("SimCadenceName", "Sim Cadence"),
			LOCTEXT("SimCadenceDesc", "Physics-authoritative cadence and training controls."),
			GetMutableDefault<USimCadenceSettings>());
	}
#endif
}

void FSimCadenceControllerModule::ShutdownModule()
{
#if WITH_EDITOR
	if (ISettingsModule* SettingsModule = FModuleManager::LoadModulePtr<ISettingsModule>(TEXT("Settings")))
	{
		SettingsModule->UnregisterSettings(TEXT("Project"), TEXT("Plugins"), TEXT("SimCadence"));
	}
#endif
}

#if WITH_EDITOR
	#undef LOCTEXT_NAMESPACE
#endif