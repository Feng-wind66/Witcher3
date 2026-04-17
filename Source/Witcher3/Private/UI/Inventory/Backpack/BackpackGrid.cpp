// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/Backpack/BackpackGrid.h"
#include "UI/Inventory/Backpack/DragDropFunction.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "UI/Inventory/Backpack/DragItemVisual.h"
#include "UI/Inventory/Option/Option.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "FunctionLibrary/CommonFuction.h"
#include "UI/Describe/ItemDescribe.h"
#include "Blueprint/Component/Inventory.h"
FReply UBackpackGrid::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton&&ItemInfo.ItemHandle>0&& BP_OptionClassArray.Num() >= 5)
	{
		UOption*Option = CreateWidget<UOption>(GetWorld(), BP_OptionClassArray[(int32)GridScene]);
		if (LastCreateOption)
			LastCreateOption->RemoveFromParent();
		LastCreateOption = Option;
		Option->AddToViewport(0);
		Option->EquipmentInfo = ItemInfo;
		Option->OwnerGrid = this;
		Option->SetPositionInViewport(UWidgetLayoutLibrary::GetMousePositionOnViewport(this) * UWidgetLayoutLibrary::GetViewportScale(this));
		return FReply::Handled();
	}
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}
	return Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
}
void UBackpackGrid::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	if (ItemInfo.ItemHandle > 0)
	{
		if (UDragDropFunction* DragDropFunction = NewObject<UDragDropFunction>())
		{
			UDragItemVisual*DragItemVisual = CreateWidget<UDragItemVisual>(this, BP_DragItemVisualClass);
			DragDropFunction->DragGrid = this;
			DragItemVisual->ShowImage = ItemInfo.EquipmentIcon;
			DragItemVisual->SetVisualImage();
			DragDropFunction->DefaultDragVisual = DragItemVisual;
			OutOperation = DragDropFunction;
		}
	}
}
bool UBackpackGrid::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	if (UDragDropFunction* DragDropFunction = Cast<UDragDropFunction>(InOperation))
	{
		if (UBackpackGrid* DragGrid = DragDropFunction->DragGrid)
		{
			if (DragGrid == this)
			{
				return true;
			}
			if (DragGrid->GridScene == EGridScene::EGS_InInventory && GridScene == EGridScene::EGS_InEquipmentSlot && GetEquipmentCategory(DragGrid->ItemInfo.EquipmentType) == EquipmentCategory)
			{
				UInventory* Inventory = UCommonFuction::GetInventoryComponnet(GetOwningPlayerPawn());
				if (Inventory)
				{
					if (ItemInfo.ItemHandle > 0 && !UCommonFuction::IsSameItemType(GetOwningPlayerPawn(), ItemInfo.EquipmentType, DragGrid->ItemInfo.EquipmentType))
					{
						return true;
					}//防药水和食物之间的bug
					Inventory->UnWearEquipment(ItemInfo.ItemHandle);
					Inventory->WearEquipment(DragGrid->ItemInfo.ItemHandle);
					SwapGridInfo(DragGrid);
				}
				return true;
			}//背包到装备栏
			else if (DragGrid->GridScene == EGridScene::EGS_InEquipmentSlot && GridScene == EGridScene::EGS_InInventory&&UCommonFuction::IsSameItemType(GetOwningPlayerPawn(),DefaultType,DragGrid->ItemInfo.EquipmentType))
			{
				UInventory* Inventory = UCommonFuction::GetInventoryComponnet(GetOwningPlayerPawn());
				if (Inventory)
				{
					Inventory->UnWearEquipment(DragGrid->ItemInfo.ItemHandle);
					Inventory->WearEquipment(ItemInfo.ItemHandle);
					SwapGridInfo(DragGrid);
				}
				return true;
			}//装备栏到背包
			else if (DragGrid->GridScene==EGridScene::EGS_InInventory &&GridScene==EGridScene::EGS_InInventory&&(GetEquipmentCategory(DragGrid->ItemInfo.EquipmentType) == GetEquipmentCategory(ItemInfo.EquipmentType)&& GetEquipmentCategory(ItemInfo.EquipmentType)!=EEquipmentCategory::EEC_None|| UCommonFuction::IsSameItemType(GetOwningPlayerPawn(), ItemInfo.EquipmentType, DragGrid->ItemInfo.EquipmentType)))
			{
				SwapGridInfo(DragGrid);
				return true;
			}//背包之间
			else if (DragGrid->GridScene ==EGridScene::EGS_InEquipmentSlot && GridScene== EGridScene::EGS_InEquipmentSlot && DragGrid->EquipmentCategory==EEquipmentCategory::EEC_Consumable&& EquipmentCategory == EEquipmentCategory::EEC_Consumable)
			{
				SwapGridInfo(DragGrid);
				return true;
			}//装备栏的消耗品之间的交换
		}
	}
	return false;
}
void UBackpackGrid::SwapGridInfo(UBackpackGrid* DragGrid)
{
	Swap(DragGrid->ItemInfo, ItemInfo);
	Swap(DragGrid->ItemDescribeRef, ItemDescribeRef);
	if (DragGrid->LastCreateOption)
	{
		DragGrid->LastCreateOption->RemoveFromParent();
	}
	if (this->LastCreateOption)
	{
		this->LastCreateOption->RemoveFromParent();
	}
	DragGrid->LastCreateOption = nullptr;
	this->LastCreateOption = nullptr;
	GridSwapEvent.Broadcast(this,DragGrid);
	DragGrid->Updates();
	Updates();
}
void UBackpackGrid::NativePreConstruct()
{
	Super::NativePreConstruct();
	Updates();
}

void UBackpackGrid::Updates_Implementation()
{
	UpdateImage();
	UpdateItemCount();
}

void UBackpackGrid::UpdateImage_Implementation()
{
}

void UBackpackGrid::UpdateItemCount_Implementation()
{
}
void UBackpackGrid::AddItemByHandle_Implementation(int32 ItemHandle)
{
	if (ItemHandle > 0)
	{
		if (UInventory* InventoryComponent = UCommonFuction::GetInventoryComponnet(GetOwningPlayerPawn()))
		{
			if(FEquipmentInfo *EquipmentInfo=InventoryComponent->Inventory.Find(ItemHandle))
			{
				ItemInfo = *EquipmentInfo;
				Updates();
			}
			else
			{
				ItemInfo = FEquipmentInfo();
				Updates();
			}
		}
	}
	else
	{
		ItemInfo = FEquipmentInfo();
		Updates();
	}
}
UItemDescribe* UBackpackGrid::CreateDescribeWidget_Implementation()
{
	if (ItemInfo.ItemHandle <= 0)
	{
		if (ItemDescribeRef)
			ItemDescribeRef->RemoveFromParent();
		SetToolTip(nullptr);
		return nullptr;
	}
	ItemDescribeRef=CreateWidget<UItemDescribe>(GetWorld(), BP_ItemDescribeClass);
	if (ItemDescribeRef)
	{
		FString ItemName = ItemInfo.EquipmentName.ToString();
		ItemName = GetItemColorStringByQuality(ItemInfo.EquipmentQuality) + ItemName + GetEndString();
		ItemDescribeRef->AddItemDescribe(FText::FromString(ItemName), true);//添加物品名称
		FString ItemType = GetItemTypeString(ItemInfo.EquipmentType);
		ItemType = GetColorString(EColor::EC_White) + ItemType + GetEndString();
		ItemDescribeRef->AddItemDescribe(FText::FromString(ItemType), true);//添加物品类型
		FString MainEntryDescribe;
		for (const auto& Pair : ItemInfo.MainEntry)
		{
			if(ItemInfo.MainEntry.Num()==1)
				ItemDescribeRef->AddItemDescribe(ToRichText(GetColorString(EColor::EC_White), Pair.Value.Describe.ToString(), FString(TEXT("X")), Pair.Value.Level), false);
			else
				ItemDescribeRef->AddItemDescribe(ToRichText(GetColorString(EColor::EC_White), Pair.Value.Describe.ToString(), FString(TEXT("X")), Pair.Value.Level/100.f), false);//没设置打组
		}//添加物品主词条
		for (const auto& Pair : ItemInfo.SubEntry)
			ItemDescribeRef->AddItemDescribe(ToRichText(GetColorString(EColor::EC_White), Pair.Value.Describe.ToString(), FString(TEXT("X")), Pair.Value.Level), false);
		//添加物品副词条
		FString ItemDescribe = FString(FString("<White>"))+ItemInfo.EquipmentDescribe.ToString()+GetEndString();
		ItemDescribeRef->AddItemDescribe(FText::FromString(ItemDescribe),false);//添加物品描述
		ItemDescribeRef->AddItemDescribe(ToRichText(GetColorString(EColor::EC_White), FString(TEXT("物品等级{X}")), FString(TEXT("X")), ItemInfo.EquipmentLevel), false);
		//添加物品等级
		ItemDescribeRef->AddItemDescribe(ToRichText(GetColorString(EColor::EC_White), FString(TEXT("重量{X}")), FString(TEXT("X")),UCommonFuction::GetInventoryComponnet(GetOwningPlayerPawn())->ItemTableMap.Find(ItemInfo.EquipmentID)->ItemBurden), false);
		//添加物品重量
		ItemDescribeRef->AddItemDescribe(ToRichText(GetColorString(EColor::EC_White), FString(TEXT("价格{X}")), FString(TEXT("X")), ItemInfo.Price), false);
		//添加物品价格
		SetToolTip(ItemDescribeRef);
	}
	return ItemDescribeRef;
}
FString UBackpackGrid::GetItemColorStringByQuality_Implementation(EItemQuality ItemQuality)
{
	switch (ItemQuality)
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
	default:return FString(TEXT("<White>"));
		break;
	}
}
FString UBackpackGrid::GetItemTypeString_Implementation(EItemType ItemType)
{
	switch (ItemType)
	{
	case EItemType::EIT_SteelSword:return FString(TEXT("银剑"));
		break;
	case EItemType::EIT_WoodenClub:return FString(TEXT("棍棒"));
		break;
	case EItemType::EIT_SilverSword:return FString(TEXT("银剑"));
		break;
	case EItemType::EIT_Crossbow:return FString(TEXT("弩箭"));
		break;
	case EItemType::EIT_LongRangeWeapon:return FString(TEXT("远程武器"));
		break;
	case EItemType::EIT_Helmet:return FString(TEXT("头盔"));
		break;
	case EItemType::EIT_Clothes:return FString(TEXT("衣服"));
		break;
	case EItemType::EIT_Gloves:return FString(TEXT("手套"));
		break;
	case EItemType::EIT_Pants:return FString(TEXT("裤子"));
		break;
	case EItemType::EIT_Material:return FString(TEXT("材料"));
		break;
	case EItemType::EIT_Draught:return FString(TEXT("药水"));
		break;
	case EItemType::EIT_Food:return FString(TEXT("食物"));
		break;
	case EItemType::EIT_Sundries:return FString(TEXT("杂物"));
		break;
	case EItemType::EIT_AlchemyBlueprint:return FString(TEXT("炼金蓝图"));
		break;
	case EItemType::EIT_CraftingBlueprint:return FString(TEXT("工艺蓝图"));
		break;
	default:return FString(TEXT(""));
		break;
	}
}
FText UBackpackGrid::ToRichText_Implementation(const FString& TextColor, const FString& MainText, const FString& VariableName, float Value)
{
	FTextFormat Format = FTextFormat::FromString(TextColor+MainText+GetEndString());
	FFormatNamedArguments Args;
	Args.Add(VariableName,Value);
	return FText::Format(Format, Args);
}
FString UBackpackGrid::GetColorString_Implementation(EColor Color)
{
	switch (Color)
	{
	case EColor::EC_White:return FString(TEXT("<White>"));
		break;
	case EColor::EC_Green:return FString(TEXT("<Green>"));
		break;
	case EColor::EC_Blue:return FString(TEXT("<Blue>"));
		break;
	case EColor::EC_Purple:return FString(TEXT("<Purple>"));
		break;
	case EColor::EC_Golden:return FString(TEXT("<Golden>"));
		break;
	default:return FString(TEXT("<White>"));
		break;
	}
}
EEquipmentCategory UBackpackGrid::GetEquipmentCategory_Implementation(EItemType Type)
{
	switch (Type)
	{
	case EItemType::EIT_SteelSword:return EEquipmentCategory::EEC_FirstWeaponSlot;
		break;
	case EItemType::EIT_WoodenClub:return EEquipmentCategory::EEC_FirstWeaponSlot;
		break;
	case EItemType::EIT_SilverSword:return EEquipmentCategory::EEC_SilverSword;
		break;
	case EItemType::EIT_Crossbow:return EEquipmentCategory::EEC_Crossbow;
		break;
	case EItemType::EIT_LongRangeWeapon:return EEquipmentCategory::EEC_LongRangeWeapon;
		break;
	case EItemType::EIT_Helmet:return EEquipmentCategory::EEC_Helmet;
		break;
	case EItemType::EIT_Clothes:return EEquipmentCategory::EEC_Clothes;
		break;
	case EItemType::EIT_Gloves:return EEquipmentCategory::EEC_Gloves;
		break;
	case EItemType::EIT_Pants:return EEquipmentCategory::EEC_Pants;
		break;
	case EItemType::EIT_Material:return EEquipmentCategory::EEC_None;
		break;
	case EItemType::EIT_Draught:return EEquipmentCategory::EEC_Consumable;
		break;
	case EItemType::EIT_Food:return EEquipmentCategory::EEC_Consumable;
		break;
	case EItemType::EIT_Sundries:return EEquipmentCategory::EEC_None;
		break;
	case EItemType::EIT_AlchemyBlueprint:return EEquipmentCategory::EEC_None;
		break;
	case EItemType::EIT_CraftingBlueprint:return EEquipmentCategory::EEC_None;
		break;
	default:return EEquipmentCategory::EEC_None;
		break;
	}
}