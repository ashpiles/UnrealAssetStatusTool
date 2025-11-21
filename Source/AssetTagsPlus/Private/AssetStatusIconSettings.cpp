#include "AssetStatusIconSettings.h"


UAssetStatusIconSettings::UAssetStatusIconSettings()
{}

TArray<FString> UAssetStatusIconSettings::GetAvailableIcons()
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

FFilePath UAssetStatusIconSettings::GetAssetStatusIconPath(FString& LocalFileName)
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

