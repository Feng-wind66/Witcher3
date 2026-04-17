// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include"Struct/InventoryStruct.h"
#include "CommonUserWidget.h"
#include "Option.generated.h"
class UMainUIComponent;
class UBackpackGrid;
class UEquipmentSlot;
class UBackpack_Slot;
/**
 * 
 */
UCLASS()
class WITCHER3_API UOption : public UCommonUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent)override;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FEquipmentInfo EquipmentInfo;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UBackpackGrid* OwnerGrid;
protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void HandleEquipOption();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void HandleUnLoadEquipment();
};
