// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include"Enum/Enum.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CommonFuction.generated.h"
class URetainInventoryData;
class UInventory;
class UEquipment;
class ADarke;
class UBackpack_Slot;
class UMainUIComponent;
class UBackpackGrid;
UCLASS(ClassGroup = (Custom), Blueprintable, BlueprintType)
class WITCHER3_API UCommonFuction : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure,BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static int32 GetPlayerLevel(UObject* WorldContextObject);
	UFUNCTION(BlueprintPure, BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static URetainInventoryData* GetRetainInventoryDataComponnet(UObject* WorldContextObject);
	UFUNCTION(BlueprintPure, BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static UInventory* GetInventoryComponnet(AActor*Owner);
	UFUNCTION(BlueprintPure, BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static ADarke* GetDarkePtr(UObject* WorldContextObject);
	UFUNCTION(BlueprintPure, BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static UMainUIComponent* GetMainUIComponent(UObject* WorldContextObject);
	UFUNCTION(BlueprintPure, BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static bool IsSameItemType(AActor* Owner,EItemType TypeOne,EItemType TypeTwo);
	UFUNCTION(BlueprintPure, BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static UBackpackGrid* SafeGetEquipmentGridByType(UObject* WorldContextObject, EItemType EquipmentType);
	UFUNCTION(BlueprintPure, BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static UBackpack_Slot* SafeGetBackpackSlotByType(UObject* WorldContextObject, EItemType EquipmentType);
};
