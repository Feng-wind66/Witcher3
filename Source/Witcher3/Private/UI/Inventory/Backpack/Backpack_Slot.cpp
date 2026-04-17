// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/Backpack/Backpack_Slot.h"
#include "Blueprint/WidgetTree.h"
#include"Macro/Macro.h"
#include "FunctionLibrary/CommonFuction.h"
#include "Blueprint/Component/Inventory.h"
#include"Struct/InventoryStruct.h"
#include "Components/UniformGridPanel.h"
#include "UI/Inventory/Backpack/BackpackGrid.h"


void UBackpack_Slot::NativePreConstruct()
{
	Super::NativePreConstruct();
	RebuildGridPanel();
	InitItemTypeName();
	InitBoxSize();
}

void UBackpack_Slot::NativeConstruct()
{
	Super::NativeConstruct();
	if (UWorld* World = GetWorld())
		DELAY(World->GetTimerManager(), this, BeginPlayDelayTimerHandle,0, &UBackpack_Slot::BeginPlayInitial);
}

void UBackpack_Slot::BeginPlayInitial_Implementation()
{
	if (UInventory* InventoryComponent = UCommonFuction::GetInventoryComponnet(GetOwningPlayerPawn()))
	{
		InventoryComponent->OnInventoryModify.AddDynamic(this, &UBackpack_Slot::HandleOnInventoryModify);
	}
}

void UBackpack_Slot::RebuildGridPanel_Implementation()
{
	UniformGridPanel->ClearChildren();
	AllBackpackGrid.Empty();
	ItemGridMap.Empty();
	for (int i = 0; i < Row; ++i)
		for (int j = 0; j < Column; ++j)
		{
			UBackpackGrid* Grid = CreateWidget<UBackpackGrid>(GetWorld(), BP_BackpackGridClass);
			Grid->GridScene = EGridScene::EGS_InInventory;
			Grid->ItemInfo.EquipmentType = ItemType;
			Grid->DefaultType = ItemType;
			Grid->GridSwapEvent.AddDynamic(this, &UBackpack_Slot::HandleGridSwap);
			AllBackpackGrid.Add(Grid);
			UniformGridPanel->AddChildToUniformGrid(Grid, i, j);
		}
}
void UBackpack_Slot::InitItemTypeName_Implementation()
{
}
void UBackpack_Slot::InitBoxSize_Implementation()
{
}
void UBackpack_Slot::HandleOnInventoryModify_Implementation(bool IsAdd, int32 ItemHandle, FEquipmentInfo ItemInfo)
{
	
	if (UCommonFuction::IsSameItemType(GetOwningPlayerPawn(), ItemType, ItemInfo.EquipmentType) && IsAdd)
	{
		if (ItemGridMap.Contains(ItemHandle))
		{
			(*ItemGridMap.Find(ItemHandle))->AddItemByHandle(ItemHandle);
		}
		else 
		{
			if(UBackpackGrid*Grid=GetEmptyGrid())
			ItemGridMap.Add(ItemHandle, Grid)->AddItemByHandle(ItemHandle);//记得处理Swap
		}
	}//添加物品
	else if(UCommonFuction::IsSameItemType(GetOwningPlayerPawn(), ItemType, ItemInfo.EquipmentType)&&!IsAdd)
	{
	}//删除物品
}//施工中...
void UBackpack_Slot::HandleGridSwap_Implementation(UBackpackGrid* OwnerGrid, UBackpackGrid* TargetGrid)
{
	int32 OwnerGridHandle = OwnerGrid->ItemInfo.ItemHandle;
	int32 TargetHandle = TargetGrid->ItemInfo.ItemHandle;
	if (OwnerGridHandle > 0)
	{
		ItemGridMap.Add(OwnerGridHandle, OwnerGrid);
	}
	else
	{
		ItemGridMap.Remove(OwnerGridHandle);
	}

	if (TargetHandle > 0)
	{
		ItemGridMap.Add(TargetHandle, TargetGrid);
	}
	else
	{
		ItemGridMap.Remove(TargetHandle);
	}
}

UBackpackGrid* UBackpack_Slot::GetEmptyGrid_Implementation()
{
	for (UBackpackGrid* Grid : AllBackpackGrid)
		if (Grid->ItemInfo.ItemHandle == 0)
		{
			return Grid;
		}
	return nullptr;
}
