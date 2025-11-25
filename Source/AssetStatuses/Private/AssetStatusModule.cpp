// Copyright Epic Games, Inc. All Rights Reserved.

#include "AssetStatusModule.h"

#include "AssetStatusFilters.h"
#include "AssetStatusIconSettings.h"
#include "AssetStatusMenuExtension.h"
#include "AssetStatusStyle.h"
#include "ContentBrowserModule.h"
#include "EditorAssetLibrary.h"
#include "IContentBrowserSingleton.h"
#include "SAssetTagIcon.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Engine/AssetManagerSettings.h"
#include "Styling/CoreStyle.h"
#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleRegistry.h"

#define LOCTEXT_NAMESPACE "AssetTagsPlusModule"


void FAssetStatusModule::StartupModule()
{

	// Load Modules and delegates
	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	TArray<FContentBrowserMenuExtender_SelectedAssets>& MenuExtenderDelegates = ContentBrowserModule.GetAllAssetViewContextMenuExtenders();
	
	// Get Developer Settings
	Settings = TObjectPtr<UAssetStatusIconSettings>(GetMutableDefault<UAssetStatusIconSettings>());
 
	// Add Style
	FAssetStatusEditorStyle::Initialize(Settings.Get()); 

	// Generate and add Icon State widgets to thumbnails
	FAssetViewExtraStateGenerator StateGenerator(
			FOnGenerateAssetViewExtraStateIndicators::CreateStatic(&FAssetStatusModule::GenerateAssetTagIcons),
			FOnGenerateAssetViewExtraStateIndicators()
		); 
	ContentBrowserModule.AddAssetViewExtraStateGenerator(StateGenerator);


	// Add menu extensions
	AssetStatusMenuExtension = MakeShareable<FAssetStatusMenuExtension>(new FAssetStatusMenuExtension(Settings.Get()));
	MenuExtenderDelegates.Add(FContentBrowserMenuExtender_SelectedAssets::CreateSP(AssetStatusMenuExtension.ToSharedRef(), &FAssetStatusMenuExtension::StatusMenuExtensionDelegate));
	
	// Applying Metadata tag to Asset Registry for filters
	GetMutableDefault<UAssetManagerSettings>()->MetaDataTagsForAssetRegistry.Add("AssetStatus");

	// Add filters
	AssetStatusFilters = MakeShareable<FAssetStatusFilters>(new FAssetStatusFilters(Settings.Get())); 
	Settings->OnSettingChanged().AddSP(AssetStatusFilters.ToSharedRef(), &FAssetStatusFilters::OnAssetStatusChanged);
	AssetRegistryModule.Get().OnFilesLoaded().AddSP(AssetStatusFilters.ToSharedRef(), &FAssetStatusFilters::InitalizeFilters);

}

void FAssetStatusModule::ShutdownModule()
{
	FAssetStatusEditorStyle::UnRegister();
} 

TSharedRef<SWidget> FAssetStatusModule::GenerateAssetTagIcons(const FAssetData& AssetData)
{
	return SNew(SAssetTagIcon)
		.AssetStatus_Lambda([ObjectPath = AssetData.GetSoftObjectPath()]() 
		{
			TMap<FName, FString> MetaData = UEditorAssetLibrary::GetMetadataTagValues(ObjectPath.ResolveObject());
			if (MetaData.Contains("AssetStatus"))
				return MetaData["AssetStatus"];
			return FString("None");
		});
}



#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FAssetStatusModule, AssetTagsPlus)