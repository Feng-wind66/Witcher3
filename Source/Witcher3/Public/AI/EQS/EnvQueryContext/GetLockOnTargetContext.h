// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "GetLockOnTargetContext.generated.h"
UCLASS(Blueprintable)
class WITCHER3_API UGetLockOnTargetContext : public UEnvQueryContext
{
	GENERATED_BODY()
public:
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FName TargetKeyName = FName(TEXT("LockOnTargetRef"));
};
