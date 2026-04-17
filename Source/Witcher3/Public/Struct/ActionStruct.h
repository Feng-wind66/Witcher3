// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagsManager.h"
#include"Enum/Enum.h"
#include "Animation/AnimMontage.h" 
#include "ActionStruct.generated.h"

USTRUCT(BlueprintType)
struct FHitReaction
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag MeleeHit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag MeleeParry;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag DeathHit;
};
USTRUCT(BlueprintType)
struct FMotionWarpingInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWhichMotionWarping MotionWarpingType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TriggerRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WarpOffset_X;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WarpOffset_Y;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WarpOffset_Z;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWarpWhat WarpWhat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LocationInterpSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RotationInterpSpeed;
};
USTRUCT(BlueprintType)
struct FActionInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ActionName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag ActionType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* ActionMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMotionWarpingInfo MotionWarping;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlayRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FHitReaction HitReaction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Cooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag CooldownTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Cost;
};
USTRUCT(BlueprintType)
struct FHitInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag ActionType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimMontage>Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMotionWarpingInfo MotionWarping;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlayRate;
};