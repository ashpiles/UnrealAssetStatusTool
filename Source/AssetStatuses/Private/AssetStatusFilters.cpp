// 


#include "AssetStatusFilters.h"

#include "AssetStatusSettings.h"
#include "AssetToolsModule.h"
#include "ContentBrowserModule.h"
#include "FileHelpers.h"
#include "IContentBrowserSingleton.h"
#include "Filters/CustomTextFilters.h"


FAssetStatusFilters::FAssetStatusFilters(UAssetStatusSettings* Settings)
{
	AssetStatusSettings = Settings;
	check(AssetStatusSettings) 
}

FAssetStatusFilters::~FAssetStatusFilters()
{
}

void FAssetStatusFilters::InitalizeFilters()
{
	FilterLabels = GetMutableDefault<UStatusFilterLabelStorage>();
	UpdateLabels();
}

void FAssetStatusFilters::OnAssetStatusChanged(UObject* InObject, FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UAssetStatusSettings, AssetStatusEntries))
	{
		if (PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayAdd)
			return;
		UpdateLabels();
	}
}

FCustomTextFilterData FAssetStatusFilters::CreateFilter(FAssetStatusEntry Entry) const
{ 
	FCustomTextFilterData FilterData;
	FilterData.FilterLabel = FText::FromString(Entry.AssetStatusTagValue);
	FilterData.FilterString = FText::FromString("AssetStatus==" + Entry.AssetStatusTagValue);
	FilterData.FilterColor = Entry.FilterColor;
	return FilterData;
}

void FAssetStatusFilters::UpdateLabels() const
{
	TSet<FString > CurrentLabels;
	for (auto& Entry : AssetStatusSettings->AssetStatusEntries)
	{
		CurrentLabels.Add(Entry.AssetStatusTagValue);
		FCustomTextFilterData Filter = CreateFilter(Entry);
		//If Filter exists Modify existing Filter 
		if (!FilterLabels->TryAddFilter(Filter))
			FilterLabels->TryModifyFilter(Filter, Entry.AssetStatusTagValue);
	}
	// Remove stale filters
	for (const auto& Label : FilterLabels->GetDifference(CurrentLabels))
	{
		FilterLabels->TryRemoveFilter(Label);
	}
	FilterLabels->SaveConfig();
	// we don't save packages because this func gets called a lot
}



bool UStatusFilterLabelStorage::TryAddFilter(FCustomTextFilterData FilterData)
{ 
	if (HasLabel(FilterData.FilterLabel.ToString()))
		return false;

	Labels.Add(FilterData.FilterLabel.ToString());
	GetContentBrowserModule()->Get().AddCustomTextFilter(FilterData, false);
	return true;
}

bool UStatusFilterLabelStorage::TryRemoveFilter(FString Label)
{
	if (!HasLabel(Label))
		return false;
	
	Labels.Remove(Label);
	GetContentBrowserModule()->Get().DeleteCustomTextFilterByLabel(FText::FromString(Label));
	return true;
}

bool UStatusFilterLabelStorage::TryModifyFilter(FCustomTextFilterData FilterData, FString Label)
{
	if (!HasLabel(Label) || !HasLabel(FilterData.FilterLabel.ToString()) || FilterData.FilterLabel.ToString() != Label)
		return false;

	GetContentBrowserModule()->Get().ModifyCustomTextFilterByLabel(FilterData, FText::FromString(Label));
	return true;
}


bool UStatusFilterLabelStorage::HasLabel(FString Label) const
{
	return Labels.Contains(Label);
}

TSet<FString> UStatusFilterLabelStorage::GetDifference(TSet<FString>& OtherSet) const
{
	return Labels.Difference(OtherSet);
}
