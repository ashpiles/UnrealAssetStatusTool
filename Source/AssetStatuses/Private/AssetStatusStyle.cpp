// 


#include "AssetStatusStyle.h"

#include "AssetStatusIconSettings.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleRegistry.h"

 
FAssetStatusEditorStyle::FAssetStatusEditorStyle()
	: FSlateStyleSet("AssetStatusEditorStyle")
{  
}   

void FAssetStatusEditorStyle::Register()
{
	FSlateStyleRegistry::RegisterSlateStyle(Get());
}

void FAssetStatusEditorStyle::UnRegister()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(Get());
}

FAssetStatusEditorStyle& FAssetStatusEditorStyle::GetMutable()
{
	static FAssetStatusEditorStyle Instance;
	return Instance;
}

const FAssetStatusEditorStyle& FAssetStatusEditorStyle::Get()
{
	return GetMutable();
} 

 

void FAssetStatusEditorStyle::Initialize(UAssetStatusIconSettings* Settings)
{
	FString ResourcePath = IPluginManager::Get().FindPlugin(UE_PLUGIN_NAME)->GetBaseDir() + "\\Resources\\Icons";
	const FString MenuIconFile = ResourcePath + "\\AssetStatusMenuIcon.png";
	if (!IFileManager::Get().DirectoryExists(*ResourcePath))
	{ 
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString("Plugin " UE_PLUGIN_NAME " resource folder not in expected location"));
		return;
	}
	
	UpdateContentRoot(ResourcePath);
	UpdateIconStyles(Settings->AssetStatusEntries);

	if (IFileManager::Get().FileExists(*MenuIconFile))
		GetMutable().Set("AssetStatus.MenuIcon", new FSlateImageBrush( MenuIconFile, CoreStyleConstants::Icon20x20 ));
	else
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString("Plugin " UE_PLUGIN_NAME " resource AssetStatusMenuIcon.png not in expected location!")); 

	Settings->OnSettingChanged().AddStatic(OnAssetStatusSettingsChanged);
	Register();
}

void FAssetStatusEditorStyle::UpdateContentRoot(FString& Path)
{
	GetMutable().SetContentRoot(Path);
}

void FAssetStatusEditorStyle::UpdateIconStyles(TArray<FAssetStatusEntry>& Entries)
{ 
	for (auto Entry : Entries)
	{
		FName IconName = FName("AssetStatus." + Entry.AssetStatusTagValue);
		GetMutable().Set(IconName, new FSlateImageBrush(Get().GetContentRootDir() + "/" + Entry.AssetStatusIcon, CoreStyleConstants::Icon20x20, Entry.FilterColor));
	} 
}

void FAssetStatusEditorStyle::OnAssetStatusSettingsChanged(UObject* InObject, FPropertyChangedEvent& PropertyChangedEvent)
{ 
	UAssetStatusIconSettings* Settings = Cast<UAssetStatusIconSettings>(InObject);
	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UAssetStatusIconSettings, IconFolderPath))
	{
		UnRegister(); 
		UpdateContentRoot(Settings->IconFolderPath.Path);
		UpdateIconStyles(Settings->AssetStatusEntries);
		Register();
	}
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(FAssetStatusEntry, AssetStatusIcon)
		|| PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(FAssetStatusEntry, AssetStatusTagValue))
	{
		UnRegister(); 
		UpdateIconStyles(Settings->AssetStatusEntries);
		Register();
	}
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(FAssetStatusEntry, FilterColor))
	{
		UpdateIconStyles(Settings->AssetStatusEntries);
	}
}

