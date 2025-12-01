// 

#pragma once

#include "CoreMinimal.h"

class UAssetStatusSettings;
/**
 * 
 */
class ASSETSTATUSES_API FAssetStatusMenuExtension : public TSharedFromThis<FAssetStatusMenuExtension>
{
public:
	FAssetStatusMenuExtension(UAssetStatusSettings* Settings);
	~FAssetStatusMenuExtension();
	
	TSharedRef<FExtender> StatusMenuExtensionDelegate(const TArray<FAssetData>& Assets);
	
private:
	void FillAssetStatusSubMenus(FMenuBuilder& MenuBuilder);
	void AddAssetStatusMenu(FMenuBuilder& MenuBuilder); 
	
	UAssetStatusSettings* AssetStatusSettings;

};
