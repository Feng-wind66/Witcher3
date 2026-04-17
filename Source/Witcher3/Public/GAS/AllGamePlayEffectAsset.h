// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "Engine/DataAsset.h"
#include "AllGamePlayEffectAsset.generated.h"

/**
 * 
 */
UCLASS()
class WITCHER3_API UAllGamePlayEffectAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CurveTable")
	UCurveTable* CurveTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayEffect")
	TSubclassOf<UGameplayEffect> GE_Entry_AdrenalineRecoveryBonus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayEffect")
	TSubclassOf<UGameplayEffect> GE_Entry_Attack_Base;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayEffect")
	TSubclassOf<UGameplayEffect> GE_Entry_Attack_Percentage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayEffect")
	TSubclassOf<UGameplayEffect> GE_Entry_Burden;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayEffect")
	TSubclassOf<UGameplayEffect> GE_Entry_ChopResistance;//Åü¿³¿¹ÐÔ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayEffect")
	TSubclassOf<UGameplayEffect> GE_Entry_CritDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayEffect")
	TSubclassOf<UGameplayEffect> GE_Entry_CritRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayEffect")
	TSubclassOf<UGameplayEffect> GE_Entry_EnergyRecoveryBonus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayEffect")
	TSubclassOf<UGameplayEffect> GE_Entry_ExecuctionChance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayEffect")
	TSubclassOf<UGameplayEffect> GE_Entry_Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayEffect")
	TSubclassOf<UGameplayEffect> GE_Entry_BluntImpactResistance;//¶Û»÷¿¹ÐÔ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayEffect")
	TSubclassOf<UGameplayEffect> GE_Entry_MaxAdrenaline;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayEffect")
	TSubclassOf<UGameplayEffect> GE_Entry_MaxBurden;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayEffect")
	TSubclassOf<UGameplayEffect> GE_Entry_MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayEffect")
	TSubclassOf<UGameplayEffect> GE_Entry_MaxToxin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayEffect")
	TSubclassOf<UGameplayEffect> GE_Entry_MonsterExperienceAcquisition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayEffect")
	TSubclassOf<UGameplayEffect> GE_Entry_PunctureResistance;//´©´Ì¿¹ÐÔ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayEffect")
	TSubclassOf<UGameplayEffect> GE_Entry_ShopSellingPrice;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayEffect")
	TSubclassOf<UGameplayEffect> GE_Entry_SpellPower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayEffect")
	TSubclassOf<UGameplayEffect>GE_Entry_RemoveAttack_Base;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayEffect")
	TSubclassOf<UGameplayEffect>GE_Burn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayEffect")
	TSubclassOf<UGameplayEffect>GE_ApplyDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayEffect")
	TSubclassOf<UGameplayEffect>GE_EnergyRecovery;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayEffect")
	TSubclassOf<UGameplayEffect>GE_EnergyCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayEffect")
	TSubclassOf<UGameplayEffect>GE_Tired;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayEffect")
	TSubclassOf<UGameplayEffect>GE_Slow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayEffect")
	TSubclassOf<UGameplayEffect>GE_Quen;
};
