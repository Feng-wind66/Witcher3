// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include"Struct/ActionStruct.h"
#include "CharacterInfoData.generated.h"

/**
 * 
 */
UCLASS()
class WITCHER3_API UCharacterInfoData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BaseCharacterInfo")
	float MaxWalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BaseCharacterInfo")
	float MaxRunSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BaseCharacterInfo")
	float MaxSprintSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HumanoidBehaviorLists|ActionList")
	TMap<FGameplayTag,FActionInfo>LongswordFinisherList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HumanoidBehaviorLists|ActionList")
	TMap<FGameplayTag, FActionInfo>LongswordActionLists;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HumanoidBehaviorLists|ActionList")
	TMap<FGameplayTag, FActionInfo>CommonActionLists;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HumanoidBehaviorLists|ActionList")
	TMap<FGameplayTag, FHitInfo>LongswordHitLists;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HumanoidBehaviorLists|Parry")
	float ParryPercentage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HumanoidBehaviorLists|Parry")
	float ParrySustainTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HumanoidBehaviorLists|Parry")
	float ParryCD;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HumanoidBehaviorLists|Finisher")
	float FinisherTirggerRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HumanoidBehaviorLists|Finisher")
	float FinisherTirggerRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MosterBehaviorLists")
	TMap<FGameplayTag, FActionInfo>MonsterActionLists;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MosterBehaviorLists")
	TMap<FGameplayTag, FHitInfo>MonsterHitLists;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MosterBehaviorLists")
	float DodgePercent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MosterBehaviorLists")
	EMonsterType MonsterType;
};
