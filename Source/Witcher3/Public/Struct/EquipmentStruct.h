// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Sound/SoundBase.h"
#include"Enum/Enum.h"
#include "CoreMinimal.h"
#include "EquipmentStruct.generated.h"
USTRUCT(BlueprintType)
struct FWeaponInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponType WeaponAnimType;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<USoundBase>WeaponLightAttackWooshSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USoundBase>WeaponLightAttackHitSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USoundBase>WeaponHeavyAttackWooshSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USoundBase>WeaponHeavyAttackHitSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USoundBase>WeaponParrySoundMetal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USoundBase>WeaponParrySoundWooden;
};
