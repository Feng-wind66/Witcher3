// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include"Enum\Enum.h"
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterInterface.generated.h"

class UEquipment;
class UAction;
class  UInventory;
class UASC;
// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class WITCHER3_API ICharacterInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CharacterInterface")
	void SendJumpPress();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CharacterInterface")
	void SendParryPress(bool bIsPress);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CharacterInterface")
    void ChangeCurrentActionState(EActionState ActionState);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CharacterInterface")
	EActionState GetCurrentActionState();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CharacterInterface")
	float GetActionXValue();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CharacterInterface")
	float GetActionYValue();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CharacterInterface")
	APawn* GetPlayerRef();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CharacterInterface")
	void DisableCollisionToPawn();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CharacterInterface")
	void GetDesiredMovement();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CharacterInterface")
	void ToggleIsRiderOn();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CharacterInterface")
	bool GetIsRiderOn();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CharacterInterface")
	EOwnerType GetOwnerType();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CharacterInterface")
	bool NotHasAnimNotify(TSubclassOf<UAnimNotifyState>NotifyState, bool Not);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CharacterInterface")
	USkeletalMeshComponent* GetSkeletalMeshByType(EItemType ItemType);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CharacterInterface")
	UStaticMeshComponent* GetStaticMeshByType(EDamageSource WeaponType);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CharacterInterface")
	UEquipment* GetEquipmentComponent();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CharacterInterface")
	UAction* GetActionComponent();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CharacterInterface")
	UInventory* GetInventoryComponent();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CharacterInterface")
	void SetDodgeType(EDodgeType DodgeType);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CharacterInterface")
	void SetCurrentLocomotionState(ELocomotionState LocomotionType);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CharacterInterface")
	void ApplyEnergyCostAPI(float EnergyCost);
};