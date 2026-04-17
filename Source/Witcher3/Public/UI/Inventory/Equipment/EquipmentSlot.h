// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include"Enum/Enum.h"
#include "EquipmentSlot.generated.h"
class UBackpackGrid;
/**
 * 
 */
UCLASS()
class WITCHER3_API UEquipmentSlot : public UCommonUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	UBackpackGrid* GetEquipmentGridByItemType(EItemType ItemType);
};
