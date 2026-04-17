// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Component/MainUIComponent.h"
#include "UI/Inventory/Backpack/Backpack_Slot.h"
#include "UI/Inventory/Equipment/EquipmentSlot.h"
UMainUIComponent::UMainUIComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


UBackpack_Slot* UMainUIComponent::GetBackpackSlotByType_Implementation(EItemType ItemType)
{
	return TObjectPtr<UBackpack_Slot>();
}

void UMainUIComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UMainUIComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

