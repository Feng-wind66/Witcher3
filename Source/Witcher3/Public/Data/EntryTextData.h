// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EntryTextData.generated.h"

/**
 * 
 */
UCLASS()
class WITCHER3_API UEntryTextData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="EffectText")
	FText AdrenalineRecoveryBonus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="EffectText")
	FText Attack_Base;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="EffectText")
	FText Attack_Percentage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="EffectText")
	FText Burden;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="EffectText")
	FText ChopResistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="EffectText")
	FText CritDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="EffectText")
	FText CritRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="EffectText")
	FText EnergyRecoveryBonus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="EffectText")
	FText ExecuctionChance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="EffectText")
	FText Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="EffectText")
	FText BluntImpactResistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="EffectText")
	FText MaxAdrenaline;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="EffectText")
	FText MaxBurden;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="EffectText")
	FText MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="EffectText")
	FText MaxToxin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="EffectText")
	FText MonsterExperienceAcquisition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="EffectText")
	FText PunctureResistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="EffectText")
	FText ShopSellingPrice;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="EffectText")
	FText SpellPower;
};
