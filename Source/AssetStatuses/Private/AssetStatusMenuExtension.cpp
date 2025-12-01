// 


#include "AssetStatusMenuExtension.h"

#include "AssetSelection.h"
#include "AssetStatusSettings.h"
#include "EditorAssetLibrary.h"

#define LOCTEXT_NAMESPACE "AssetStatusMenuExtension"
FAssetStatusMenuExtension::FAssetStatusMenuExtension(UAssetStatusSettings* Settings)
{
	check(Settings);
	AssetStatusSettings = Settings;
}

FAssetStatusMenuExtension::~FAssetStatusMenuExtension()
{
}


// pass selected asset data
// set selected asset data to status
TSharedRef<FExtender> FAssetStatusMenuExtension::StatusMenuExtensionDelegate(const TArray<FAssetData>& Assets)
{ 
	TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender);
	MenuExtender->AddMenuExtension(
		"CommonAssetActions",
		EExtensionHook::After,
		TSharedPtr<FUICommandList>(),
		FMenuExtensionDelegate::CreateSP(this, &FAssetStatusMenuExtension::AddAssetStatusMenu)
		);

	return MenuExtender.ToSharedRef();
}

void FAssetStatusMenuExtension::AddAssetStatusMenu(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.AddSubMenu(
		FText::FromString("Apply Asset Status"),
		FText::FromString("Change selected assets to a new asset status"),
		FNewMenuDelegate::CreateSP(this, &FAssetStatusMenuExtension::FillAssetStatusSubMenus),
		false,
		FSlateIcon("AssetStatusEditorStyle", "AssetStatus.MenuIcon") 
		);
}

// once we get functionality can focus on adding menu separators and menu icons
void FAssetStatusMenuExtension::FillAssetStatusSubMenus(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.BeginSection("AssetStatuses", LOCTEXT("AssetStatusTags", "Asset Status Tags"));
	for (auto Entry : AssetStatusSettings->AssetStatusEntries) 
	{
		// Create new menu entry of Asset Status Tag Value
		MenuBuilder.AddMenuEntry(
		FText::FromString(Entry.AssetStatusTagValue),
		FText::FromString("Change this asset's status to " + Entry.AssetStatusTagValue),
		FSlateIcon(),
		FUIAction(
			FExecuteAction::CreateLambda([Entry]()
			{
				// For each selected asset apply the cached asset status
				TArray<FAssetData> SelectedAssets;
				AssetSelectionUtils::GetSelectedAssets(SelectedAssets);
				
				for (const auto& Asset : SelectedAssets)
					UEditorAssetLibrary::SetMetadataTag(Asset.GetAsset(), "AssetStatus", Entry.AssetStatusTagValue);
			}
			))
		);
	}
	MenuBuilder.EndSection();
	MenuBuilder.AddMenuSeparator();
	// Remove Asset Status - Remove metadata
	MenuBuilder.AddMenuEntry(
	FText::FromString("Remove Asset Status"),
	FText::FromString("Completely remove AssetStatus metadata"),
	FSlateIcon(),
	FUIAction(
		FExecuteAction::CreateLambda([]()
		{
			// For each selected asset apply the cached asset status
			TArray<FAssetData> SelectedAssets;
			AssetSelectionUtils::GetSelectedAssets(SelectedAssets);
				
			for (const auto& Asset : SelectedAssets)
				UEditorAssetLibrary::RemoveMetadataTag(Asset.GetAsset(), "AssetStatus");
		}
		))
	); 
	
}

#undef  LOCTEXT_NAMESPACE
