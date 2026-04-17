// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include"Enum/Enum.h"
#include "ASC.generated.h"
class UAS;
class UAllGamePlayEffectAsset;
/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WITCHER3_API UASC : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	UASC();
	FTimerHandle InitComponentHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAS>AS;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UDataTable* CharacterDataTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<UGameplayAbility>>BaseAbility;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TWeakObjectPtr<class UAction>ActionComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TWeakObjectPtr<class UInventory>InventoryComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TWeakObjectPtr<AActor>Owner;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAllGamePlayEffectAsset>AllGE;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TWeakObjectPtr<class UEquipment>EquipmentComponent;
	/*
	FUNCTION
	*/
	UFUNCTION(BlueprintCallable)
	bool DealWithDamage(AActor*HittedActor,FGameplayTag DamageType,EDamageSource DamageSource);
protected:
	virtual void BeginPlay()override;
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void Init();
	UFUNCTION(BlueprintCallable)
	void InitComponent();
	UFUNCTION(BlueprintCallable)
	void AddDamgaeTag(EDamageSource DamageSource, EItemType WeaponType, FGameplayEffectSpec&Spec);
	UFUNCTION(BlueprintCallable)
	EItemType GetWeaponType(EDamageSource DamageSource);
};
