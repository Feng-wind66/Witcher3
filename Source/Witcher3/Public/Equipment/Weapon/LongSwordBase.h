// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equipment/WeaponBase.h"
#include "LongSwordBase.generated.h"

/**
 * 
 */
UCLASS()
class WITCHER3_API ALongSwordBase : public AWeaponBase
{
	GENERATED_BODY()
public:
protected:
	void BeginPlay() override;
};
