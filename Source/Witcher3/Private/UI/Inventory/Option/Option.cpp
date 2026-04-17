// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/Option/Option.h"
#include"GameFramework/HUD.h"
#include "FunctionLibrary/CommonFuction.h"
#include "Blueprint/Component/Inventory.h"
#include "UI/Inventory/Backpack/Backpack_Slot.h"
#include "UI/Inventory/Backpack/BackpackGrid.h"
#include "UI/Inventory/Equipment/EquipmentSlot.h"
#include "UI/Component/MainUIComponent.h"

void UOption::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	RemoveFromParent();
}

void UOption::HandleEquipOption_Implementation()
{
	if (UBackpackGrid* EquipmentGrid = UCommonFuction::SafeGetEquipmentGridByType(this,EquipmentInfo.EquipmentType))
	{
		UInventory* Inventory = UCommonFuction::GetInventoryComponnet(GetOwningPlayerPawn());
		if (OwnerGrid)
		{
			Inventory->UnWearEquipment(EquipmentGrid->ItemInfo.ItemHandle);
			Inventory->WearEquipment(OwnerGrid->ItemInfo.ItemHandle);
			Swap(OwnerGrid->ItemInfo, EquipmentGrid->ItemInfo);
			Swap(OwnerGrid->ItemDescribeRef, EquipmentGrid->ItemDescribeRef);
			if (OwnerGrid->LastCreateOption)
			{
				OwnerGrid->LastCreateOption->RemoveFromParent();
			}
			if (EquipmentGrid->LastCreateOption)
			{
				EquipmentGrid->LastCreateOption->RemoveFromParent();
			}
			OwnerGrid->LastCreateOption = nullptr;
			EquipmentGrid->LastCreateOption = nullptr;
			//OwnerGrid->GridSwapEvent.Broadcast(OwnerGrid, EquipmentGrid);
			OwnerGrid->Updates();
			EquipmentGrid->Updates();
		}
		RemoveFromParent();
	}
}
void UOption::HandleUnLoadEquipment_Implementation()
{
	if (UInventory* Inventory = UCommonFuction::GetInventoryComponnet(GetOwningPlayerPawn()))
	{
		UBackpackGrid*BackpackGrid=UCommonFuction::SafeGetBackpackSlotByType(this, OwnerGrid->DefaultType)->GetEmptyGrid();
		Inventory->UnWearEquipment(OwnerGrid->ItemInfo.ItemHandle);
		Swap(OwnerGrid->ItemInfo, BackpackGrid->ItemInfo);
		Swap(OwnerGrid->ItemDescribeRef, BackpackGrid->ItemDescribeRef);
		if (OwnerGrid->LastCreateOption)
		{
			OwnerGrid->LastCreateOption->RemoveFromParent();
		}
		if (BackpackGrid->LastCreateOption)
		{
			BackpackGrid->LastCreateOption->RemoveFromParent();
		}
		OwnerGrid->LastCreateOption = nullptr;
		BackpackGrid->LastCreateOption = nullptr;
		OwnerGrid->Updates();
		BackpackGrid->Updates();
	}
	RemoveFromParent();
	//没处理如果背包满了的情况
}