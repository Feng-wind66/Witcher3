// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include"Struct/InventoryStruct.h"
#include "LootItemButton.generated.h"
class UImage;
class URichTextBlock;
/**
 * 
 */
UCLASS()
class WITCHER3_API ULootItemButton : public UCommonButtonBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FEquipmentInfo ItemInfo;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage>ItemIcon;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<URichTextBlock>ItemName;
	UPROPERTY()
	TWeakObjectPtr<class ULootMenu>WeakLootMenuParent;
	UFUNCTION(BlueprintCallable)
	void UpdateItemInfo(const FEquipmentInfo&DropItemInfo);
	UFUNCTION(BlueprintCallable)
	void BeClicked();
	UFUNCTION(BlueprintCallable)
	FString GetItemColorByType(EItemQuality Quality);
};
