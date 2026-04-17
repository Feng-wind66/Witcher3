// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Button/LootItemButton.h"
#include "UI/Menu/LootMenu.h"
#include "Components/Image.h"
#include "Components/RichTextBlock.h"

void ULootItemButton::UpdateItemInfo(const FEquipmentInfo& DropItemInfo)
{
	ItemInfo = DropItemInfo;
	ItemIcon->SetBrushFromTexture(DropItemInfo.EquipmentIcon);
	FString ItemRichText = GetItemColorByType(DropItemInfo.EquipmentQuality) + DropItemInfo.EquipmentName.ToString() + FString(TEXT("</>"));
	ItemName->SetText(FText::FromString(ItemRichText));
}

void ULootItemButton::BeClicked()
{
	if (!WeakLootMenuParent.IsValid())return;
	WeakLootMenuParent->ChildOnClicked(this);
}

FString ULootItemButton::GetItemColorByType(EItemQuality Quality)
{
	switch (Quality)
	{
	case EItemQuality::EIQ_Ordinary:return FString(TEXT("<White>"));
		break;
	case EItemQuality::EIQ_Rare:return FString(TEXT("<Green>"));
		break;
	case EItemQuality::EIQ_Exquisite:return FString(TEXT("<Blue>"));
		break;
	case EItemQuality::EIQ_Epic:return FString(TEXT("<Purple>"));
		break;
	case EItemQuality::EIQ_UniqueLegend:return FString(TEXT("<Golden>"));
		break;
	}
	return FString();
}
