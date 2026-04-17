// Copyright to Benoit L'hoest 2025

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FProceduralDismembermentSystemModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
