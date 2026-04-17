// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "Input/Events.h"
#include"Struct/InventoryStruct.h"
#include "BackpackGrid.generated.h"
class UItemDescribe;
class UDragItemVisual;
class UOption;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGridSwap, UBackpackGrid*, OwnerGrid, UBackpackGrid*, TargetGrid);
UCLASS()
class WITCHER3_API UBackpackGrid : public UCommonButtonBase
{
	GENERATED_BODY()
public:
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;
	void SwapGridInfo(UBackpackGrid* DragGrid);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FOnGridSwap GridSwapEvent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UOption* LastCreateOption;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UItemDescribe* ItemDescribeRef;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FEquipmentInfo ItemInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEquipmentCategory EquipmentCategory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType DefaultType;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Update")
	void AddItemByHandle(int32 ItemHandle);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Describe")
	TSubclassOf<UItemDescribe>BP_ItemDescribeClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta = (ExposeOnSpawn = "true"))
	EGridScene GridScene;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BPClass")
	TArray<TSubclassOf<UOption>>BP_OptionClassArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BPClass")
	TSubclassOf<UDragItemVisual>BP_DragItemVisualClass;

	/*
	FUNCTION
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Update")
	void Updates();
protected:
	virtual void NativePreConstruct() override;
	FORCEINLINE FString GetEndString() { return FString(TEXT("</>")); };
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Update")
	void UpdateImage();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Update")
	void UpdateItemCount();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Describe")
	UItemDescribe* CreateDescribeWidget();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Describe")
	FString GetItemColorStringByQuality(EItemQuality ItemQuality);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Describe")
	FString GetItemTypeString(EItemType ItemType);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Describe")
	FText ToRichText(const FString& TextColor, const FString& MainText,const FString&VariableName ,float Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Describe")
	FString GetColorString(EColor Color);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	EEquipmentCategory GetEquipmentCategory(EItemType Type);
};
