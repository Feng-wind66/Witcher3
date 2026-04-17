// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "PhysicalDamage.generated.h"

/**
 * 
 */
UCLASS()
class WITCHER3_API UPhysicalDamage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UPhysicalDamage();
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
