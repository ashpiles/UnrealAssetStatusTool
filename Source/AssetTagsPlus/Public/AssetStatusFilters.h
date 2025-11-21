
#pragma once
#include "CoreMinimal.h"
#include "AssetStatusFilters.generated.h"

class FAssetStatusFilters;
class FContentBrowserModule;
struct FAssetStatusEntry;
class UAssetStatusIconSettings;
struct FCustomTextFilterData;


/**
 * For saving labels across sessions
 */
UCLASS(Config=DefaultEditorPerProjectUserSettings, DefaultConfig)
class UStatusFilterLabelStorage : public UObject
{
	GENERATED_BODY()
	
public:
	bool HasLabel(FString Label) const;
	TSet<FString> GetDifference(TSet<FString>& OtherSet) const; 
	bool TryAddFilter(FCustomTextFilterData FilterData);
	bool TryRemoveFilter(FString Label);
	bool TryModifyFilter(FCustomTextFilterData FilterData, FString Label);
  
private:
	UPROPERTY(Config, EditDefaultsOnly, Category="Tool Data")
	TSet<FString> Labels;
	FContentBrowserModule* GetContentBrowserModule() const
	{
		if (FModuleManager::Get().IsModuleLoaded("ContentBrowser"))
			return &FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
    
		return nullptr;
	} 
};

/**
 * 
 */
class ASSETTAGSPLUS_API FAssetStatusFilters : public TSharedFromThis<FAssetStatusFilters>
{
public:
	explicit FAssetStatusFilters(UAssetStatusIconSettings* Settings);
	~FAssetStatusFilters();
	
	void InitalizeFilters();
	
	void OnAssetStatusChanged(UObject* InObject, FPropertyChangedEvent& InCustomData);
	FCustomTextFilterData CreateFilter(FAssetStatusEntry Entry) const; 
	void UpdateLabels() const;

private:
	TWeakObjectPtr<UStatusFilterLabelStorage> FilterLabels;
	UAssetStatusIconSettings* AssetStatusSettings;

};
