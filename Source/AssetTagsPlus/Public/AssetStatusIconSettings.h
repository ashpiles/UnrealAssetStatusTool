// 

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "AssetStatusIconSettings.generated.h"


USTRUCT(BlueprintType)
struct FAssetStatusEntry
{
	GENERATED_BODY() 
	
	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly, Category="Tool Data")
	FString AssetStatusTagValue;

	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly, meta=(GetOptions="GetAvailableIcons"), Category="Tool Data")
	FString AssetStatusIcon;
	
	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly, Category="Tool Data")
	FLinearColor FilterColor; 
};

/**
 * 
 */
UCLASS(Config = Editor, defaultconfig, meta = (DisplayName = "Asset Status Icons", ToolTip = "Custom menu configuration for asset status icons"))
class ASSETTAGSPLUS_API UAssetStatusIconSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UAssetStatusIconSettings();
	
	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly, Category="Tool Data")
	FDirectoryPath IconFolderPath;
 
	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly, Category="Tool Data")
	TArray<FAssetStatusEntry> AssetStatusEntries;

	UFUNCTION() 
	TArray<FString> GetAvailableIcons();

	UFUNCTION()
	FFilePath GetAssetStatusIconPath(FString& LocalFileName);
	

};
 