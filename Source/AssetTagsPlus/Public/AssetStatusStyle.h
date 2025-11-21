// 

#pragma once

#include "CoreMinimal.h"
#include "EditorStyleSet.h"

struct FAssetStatusEntry;
class UAssetStatusIconSettings;
/**
 * 
 */
class ASSETTAGSPLUS_API FAssetStatusEditorStyle : public FSlateStyleSet
{
public:
	static void Register();
	static void UnRegister();

	static const FAssetStatusEditorStyle& Get();

	static void Initialize(UAssetStatusIconSettings* Settings);

 
private:
	FAssetStatusEditorStyle();
	
	static FAssetStatusEditorStyle& GetMutable();
	static void UpdateContentRoot(FString& Path);
	static void UpdateIconStyles(TArray<FAssetStatusEntry>& Entries);
	static void OnAssetStatusSettingsChanged(UObject* InObject, FPropertyChangedEvent& PropertyChangedEvent);


};
