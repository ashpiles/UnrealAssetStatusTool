// 

#pragma once

#include "CoreMinimal.h"

DECLARE_DELEGATE_RetVal( FString, FAssetStatusAttributeGetter)

class SAssetTagIcon;
/**
 * Responsible for switching out the actual widget icon
 */
class ASSETTAGSPLUS_API SAssetTagIcon : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SAssetTagIcon) {}
		SLATE_ATTRIBUTE(FString, AssetStatus)
	SLATE_END_ARGS()

	
	void Construct(const FArguments& args);
private:

	FAssetStatusAttributeGetter AssetStatus;


};
