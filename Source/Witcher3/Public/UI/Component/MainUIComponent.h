// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include"Enum/Enum.h"
#include "Components/ActorComponent.h"
#include "MainUIComponent.generated.h"
class UMainUI;
class UEquipmentSlot;
class UBackpack_Slot;
class USkillWheel;
UCLASS(ClassGroup = (Custom), Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class WITCHER3_API UMainUIComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMainUIComponent();
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UMainUI* MainUIRef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkillWheel>SkillWheelRef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UEquipmentSlot* EquipmentSlot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UBackpack_Slot>WeaponSlot;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UBackpack_Slot> ArmorSlot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UBackpack_Slot> FoodSlot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UBackpack_Slot> MaterialSlot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UBackpack_Slot> DraughtSlot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UBackpack_Slot> SundriesSlot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UBackpack_Slot> AlchemyBlueprintSlot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UBackpack_Slot> CraftingBlueprint;
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	UBackpack_Slot* GetBackpackSlotByType(EItemType ItemType);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
