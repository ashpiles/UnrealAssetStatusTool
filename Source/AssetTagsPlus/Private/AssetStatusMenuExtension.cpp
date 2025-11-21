// 


#include "AssetStatusMenuExtension.h"

#include "AssetSelection.h"
#include "AssetStatusIconSettings.h"
#include "AssetStatusStyle.h"
#include "EditorAssetLibrary.h"


FAssetStatusMenuExtension::FAssetStatusMenuExtension(UAssetStatusIconSettings* Settings)
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
	for (auto Entry : AssetStatusSettings->AssetStatusEntries) 
	{
		// Create new menu entry of Asset Status Tag Value
		MenuBuilder.AddMenuEntry(
		FText::FromString(Entry.AssetStatusTagValue),
		FText::FromString("Change this Asset's status to " + Entry.AssetStatusTagValue),
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
}

