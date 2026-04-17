// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include"Struct/InventoryStruct.h"
#include "Misc/Optional.h"
#include "CommonActivatableWidget.h"
#include "LootMenu.generated.h"
/**
 * 
 */
UCLASS()
class WITCHER3_API ULootMenu : public UCommonActivatableWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<FEquipmentInfo>DropItems;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ULootItemButton>LootItemButtonClass;
	UPROPERTY()
	TWeakObjectPtr<AActor>WeakActiveActor;
	UPROPERTY()
	TWeakObjectPtr<AActor>WeakInractor;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UScrollBox>ScrollBox;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UButton>BackgroundButton;
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void UpdateDropItemInfo(const TArray<FEquipmentInfo>& DropItemArray,AActor*Interactor,AActor*ActiveActor);
	UFUNCTION(BlueprintCallable)
	void QuitMenu();
	UFUNCTION(BlueprintCallable)
	void ChildOnClicked(class ULootItemButton* ClickedButton);
	UFUNCTION(BlueprintCallable)
	void LootAllItems();
protected:
	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;
	virtual void NativeConstruct() override;
};
