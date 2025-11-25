// 


#include "SAssetTagIcon.h"
#include "AssetStatusStyle.h"


void SAssetTagIcon::Construct(const FArguments& args)
{
	// storing a local copy of the attribute delegate mandatory for stopping race conditions
	// 
	AssetStatus = args._AssetStatus.GetBinding();
	ChildSlot
	[
		SNew(SBox)
		.IsEnabled_Lambda([&]
		{
			if (AssetStatus.IsBound())
				return AssetStatus.Execute() == "None";
			return false;
		})
		[
			SNew(SImage)
			.Visibility_Lambda([&]
			{
				
				if (AssetStatus.IsBound())
				{ 
					bool bIsNone = AssetStatus.Execute() == "None"; 
					return bIsNone ? EVisibility::Collapsed : EVisibility::Visible;
				}
				return EVisibility::Collapsed;
			})
			.Image_Lambda( [&]
			{
				if (AssetStatus.IsBound())
					return FAssetStatusEditorStyle::Get().GetBrush(FName("AssetStatus." + AssetStatus.Execute())); 

				return FAppStyle::GetBrush("GenericWhiteBox");
			})
		]
	];
}



