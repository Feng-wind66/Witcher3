// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLibrary/CommonFuction.h"
#include "UI/Component/MainUIComponent.h"
#include "UI/Inventory/Equipment/EquipmentSlot.h"
#include "Core/DarkeState.h"
#include"GameFramework/HUD.h"
#include "Character/Darke.h"
#include "Core/GameMode_1.h"
#include "Blueprint/Component/Inventory.h"
#include "Blueprint/Component/RetainInventoryData.h"
#include <Kismet/GameplayStatics.h>

int32 UCommonFuction::GetPlayerLevel(UObject* WorldContextObject)
{
	if (!WorldContextObject)
	{
		UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
		if (ADarkeState* PlayerState = Cast<ADarkeState>(UGameplayStatics::GetPlayerState(World, 0)))
			return PlayerState->PlayerLevel;
	}
	return 1;
}

URetainInventoryData* UCommonFuction::GetRetainInventoryDataComponnet(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{
		UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
		if (ADarkeState* PlayerState = Cast<ADarkeState>(UGameplayStatics::GetPlayerState(World, 0)))
			return PlayerState->RetainIventoryDataComponent;
	}
	return nullptr;
}

UInventory* UCommonFuction::GetInventoryComponnet(AActor* Owner)
{
	if (IsValid(Owner)&&Owner->GetClass()->ImplementsInterface(UCharacterInterface::StaticClass()))
	{
		return ICharacterInterface::Execute_GetInventoryComponent(Owner);
	}
	return nullptr;
}

ADarke* UCommonFuction::GetDarkePtr(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{
		UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
		AGameMode_1* GameMode = Cast<AGameMode_1>(World->GetAuthGameMode());
		if (GameMode)
		{
			return Cast<ADarke>(ICharacterInterface::Execute_GetPlayerRef(GameMode));
		}
	}
	return nullptr;
}

UMainUIComponent* UCommonFuction::GetMainUIComponent(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{
		UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
		if (APlayerController* Controller = World->GetFirstPlayerController())
		{
			return Controller->GetHUD()->FindComponentByClass<UMainUIComponent>();
		}
	}
	return nullptr;
}

bool UCommonFuction::IsSameItemType(AActor* Owner,EItemType TypeOne, EItemType TypeTwo)
{
	if (UInventory* InventoryComponent = GetInventoryComponnet(Owner))
	{
		return (InventoryComponent->IsArmor(TypeOne) && InventoryComponent->IsArmor(TypeTwo) || InventoryComponent->IsWeapon(TypeOne) && InventoryComponent->IsWeapon(TypeTwo) || TypeOne == TypeTwo);
	}
	return false;
}

UBackpackGrid* UCommonFuction::SafeGetEquipmentGridByType(UObject* WorldContextObject, EItemType EquipmentType)
{
	if (WorldContextObject)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject))
			if (UMainUIComponent* MainUIComponent = UCommonFuction::GetMainUIComponent(World))
				if (UEquipmentSlot* EquipmentSlot = MainUIComponent->EquipmentSlot)
					if (UBackpackGrid* EquipmentGrid = EquipmentSlot->GetEquipmentGridByItemType(EquipmentType))
					{
						return EquipmentGrid;
					}
	}
	return nullptr;
}

UBackpack_Slot* UCommonFuction::SafeGetBackpackSlotByType(UObject* WorldContextObject, EItemType EquipmentType)
{
	if (WorldContextObject)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject))
			if (UMainUIComponent* MainUIComponent = UCommonFuction::GetMainUIComponent(World))
				if (UBackpack_Slot* BackpackSlot = MainUIComponent->GetBackpackSlotByType(EquipmentType))
					return BackpackSlot;
	}
	return nullptr;
}
