// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TargettingInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UTargettingInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class WITCHER3_API ITargettingInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "TargettingInterface")
	void SetCombatTargettable(bool Targettable);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "TargettingInterface")
	 bool IsCombatTargettable();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "TargettingInterface")
	 void ToggleHPBarVisibility(bool Visbility);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "TargettingInterface")
	 void AI_GetAttackTarget();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "TargettingInterface")
	 void ToggleTargettable();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "TargettingInterface")
	 void TogglePlayerLockOn();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "TargettingInterface")
	 void GetIsLockOn();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "TargettingInterface")
	 bool IsCombatDamageble();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "TargettingInterface")
	 void TriggerPhysicalHit();
};
