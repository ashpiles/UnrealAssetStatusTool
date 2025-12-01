#include "AssetStatusSettings.h"

#include "AssetRegistry/AssetRegistryModule.h"

UAssetStatusSettings::UAssetStatusSettings()
{ 
}



TArray<FString> UAssetStatusSettings::GetAvailableIcons()
{
	TArray<FString> AvailableIcons;
	FString Folder = IconFolderPath.Path;
	FString AbsolutePath = FPaths::ConvertRelativePathToFull(Folder);
	FPaths::NormalizeDirectoryName(AbsolutePath);
	AbsolutePath += TEXT("/");

	TArray<FString> FoundFiles;
	IFileManager::Get().FindFilesRecursive(
		FoundFiles,
		*AbsolutePath,
		TEXT("*.png"),
		true, false
	); 

 
	for (const FString& FilePath : FoundFiles) 
	{
		AvailableIcons.AddUnique(FPaths::GetCleanFilename(FilePath));
	}
	return AvailableIcons;
}

TArray<FName> UAssetStatusSettings::GetAvailableIconNames()
{
	TArray<FString> AvailableIcons = GetAvailableIcons();
	TArray<FName> AvailableIconNames;
	for (const FString& IconName : AvailableIcons)
		AvailableIconNames.Add(*IconName);
	return AvailableIconNames;
}

FFilePath UAssetStatusSettings::GetAssetStatusIconPath(FString& LocalFileName)
{
	FString Path = IconFolderPath.Path;
	FString AbsolutePath = FPaths::ConvertRelativePathToFull(Path);
	FPaths::NormalizeDirectoryName(AbsolutePath);
	FPaths::NormalizeDirectoryName(LocalFileName);
	AbsolutePath += TEXT("/") + LocalFileName;
	if (IFileManager::Get().FileExists(*AbsolutePath))
		return FFilePath(AbsolutePath);
	// put an err screen up
	return FFilePath();
}

void UAssetStatusSettings::GetAssetDataOf(TArray<FName> StatusValues, TArray<FAssetData>& OutData) 
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	AssetRegistryModule.Get().GetAssetsByTags(StatusValues, OutData);
}

UAssetStatusSettings* UAssetStatusSettings::GetAssetStatusSettings()
{
	return GetMutableDefault<UAssetStatusSettings>();
}

void UAssetStatusSettings::PostInitProperties()
{
	Super::PostInitProperties();

	FString Section = "/Script/AssetStatuses.AssetStatusIconSettings";
	FString EditorConfigPath = "D:/DungeonDash/Config/DefaultEditor.ini";
	FConfigFile InitConfig;
	bool bIsFirstInit;
	FString PluginInitConfigDir = IPluginManager::Get()
		   .FindPlugin(UE_PLUGIN_NAME)
		   ->GetBaseDir() / TEXT("Config");

	FString IniPath = PluginInitConfigDir / TEXT("DefaultAssetStatus.ini");

	// Load into the "AssetStatus" config namespace
	FConfigCacheIni::LoadExternalIniFile(
		InitConfig,
		*IniPath,
		TEXT("AssetStatus"),
		nullptr,        // Platform (nullptr = use current)
		false           // bDoNotLoadGlobalINIFile
	);
	InitConfig.GetBool(*Section, TEXT("bIsFirstInit"), bIsFirstInit);

	if (bIsFirstInit)
	{
		InitConfig.SetBool(TEXT("/Script/AssetStatuses.AssetStatusIconSettings"), TEXT("bIsFirstInit"), false);
		FConfigFile LocalConfig;
		TArray<FString> ConfigAssetStatusEntries;
		FString AssetStatusIconDir;
		FConfigCacheIni::LoadLocalIniFile(LocalConfig, TEXT("DefaultEditor"), false);
		LocalConfig.Combine(IniPath);
		LocalConfig.Write(EditorConfigPath);
		InitConfig.Write(IniPath);
	}
	LoadConfig(StaticClass(), *EditorConfigPath);

}

