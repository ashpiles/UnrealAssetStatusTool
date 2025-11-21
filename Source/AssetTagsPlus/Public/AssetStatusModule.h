// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AssetStatusIconSettings.h"
#include "Modules/ModuleManager.h"


class FAssetStatusEditorStyle;
class FAssetStatusFilters;
struct FCustomTextFilterData;
class FAssetStatusMenuExtension;

class FAssetStatusModule : public IModuleInterface
{
public:

	virtual void StartupModule() override;
	virtual void ShutdownModule() override; 

	static TSharedRef<SWidget> GenerateAssetTagIcons(const FAssetData& AssetData);

private:

	TSharedPtr<FAssetStatusMenuExtension> AssetStatusMenuExtension;
	TSharedPtr<FAssetStatusFilters> AssetStatusFilters;
	TObjectPtr<UAssetStatusIconSettings> Settings;
	
};
