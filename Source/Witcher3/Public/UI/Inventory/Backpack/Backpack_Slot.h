// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include"Enum/Enum.h"
#include "Backpack_Slot.generated.h"
class UUniformGridPanel;
class UBackpackGrid;
struct FEquipmentInfo;
/**
 * 
 */
UCLASS()
class WITCHER3_API UBackpack_Slot : public UCommonActivatableWidget
{
	GENERATED_BODY()
public:
	FTimerHandle BeginPlayDelayTimerHandle;
	UPROPERTY(BlueprintReadOnly,meta = (BindWidget))
	UUniformGridPanel* UniformGridPanel;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 Row;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Column;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType ItemType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UBackpackGrid>BP_BackpackGridClass;
	UPROPERTY(Transient, BlueprintReadOnly)
	TArray<UBackpackGrid*>AllBackpackGrid;
	UPROPERTY(Transient, BlueprintReadOnly)
	TMap<int32,UBackpackGrid*>ItemGridMap;
	/*
	FUNCTION
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UBackpackGrid* GetEmptyGrid();
	
protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct()override;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Init")
	void BeginPlayInitial();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Init")
	void RebuildGridPanel();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Init")
	void InitItemTypeName();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Init")
	void InitBoxSize();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void HandleOnInventoryModify(bool IsAdd, int32 ItemHandle, FEquipmentInfo ItemInfo);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void HandleGridSwap(UBackpackGrid*OwnerGrid,UBackpackGrid*TargerGrid);
};
