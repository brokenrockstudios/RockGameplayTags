// Copyright 2025 Broken Rock Studios. All Rights Reserved.
// See the LICENSE file for details.

#pragma once

#include "Modules/ModuleManager.h"

class FRockGameplayTagsModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
