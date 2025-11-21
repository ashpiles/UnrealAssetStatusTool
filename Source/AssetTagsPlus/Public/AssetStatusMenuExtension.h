// 

#pragma once

#include "CoreMinimal.h"

class UAssetStatusIconSettings;
/**
 * 
 */
class ASSETTAGSPLUS_API FAssetStatusMenuExtension : public TSharedFromThis<FAssetStatusMenuExtension>
{
public:
	FAssetStatusMenuExtension(UAssetStatusIconSettings* Settings);
	~FAssetStatusMenuExtension();
	
	TSharedRef<FExtender> StatusMenuExtensionDelegate(const TArray<FAssetData>& Assets);
	
private:
	void FillAssetStatusSubMenus(FMenuBuilder& MenuBuilder);
	void AddAssetStatusMenu(FMenuBuilder& MenuBuilder); 
	
	UAssetStatusIconSettings* AssetStatusSettings;

};
