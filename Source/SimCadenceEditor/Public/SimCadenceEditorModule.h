#pragma once
#include "Modules/ModuleManager.h"

class FSimCadenceEditorModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
