// 

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Interfaces/IPluginManager.h"
#include "AssetStatusSettings.generated.h"
 
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
UCLASS(BlueprintType, Config = Editor, DefaultConfig, meta = (DisplayName = "Asset Status Settings", ToolTip = "Custom menu configuration for asset status icons"))
class ASSETSTATUSES_API UAssetStatusSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UAssetStatusSettings();
	
	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly, Category="Tool Data")
	FDirectoryPath IconFolderPath = FDirectoryPath(IPluginManager::Get().FindPlugin(UE_PLUGIN_NAME)->GetBaseDir() + "/Resources/Icons");
 
	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly, Category="Tool Data")
	TArray<FAssetStatusEntry> AssetStatusEntries; 

	UFUNCTION(BlueprintCallable, Category="Asset Status Util") 
	TArray<FString> GetAvailableIcons();
	
	UFUNCTION(BlueprintCallable, Category="Asset Status Util") 
	TArray<FName> GetAvailableIconNames();

	UFUNCTION(BlueprintCallable, Category="Asset Status Util")
	FFilePath GetAssetStatusIconPath(FString& LocalFileName);

	UFUNCTION(BlueprintCallable, Category="Asset Status Util")
	void GetAssetDataOf(TArray<FName> StatusValues, TArray<FAssetData>& OutData);

	UFUNCTION(BlueprintPure, Category="Asset Status Util")
	static UAssetStatusSettings* GetAssetStatusSettings();
	
	virtual void PostInitProperties() override;
 
};
 