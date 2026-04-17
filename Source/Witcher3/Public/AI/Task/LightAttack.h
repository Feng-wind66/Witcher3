// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "LightAttack.generated.h"
struct FLightAttackBTTaskMemory
{
    FDelegateHandle OnAbilityEndedDelegateHandle;
};
/**
 * 
 */
UCLASS()
class WITCHER3_API ULightAttack : public UBTTaskNode
{
	GENERATED_BODY()
public:
	virtual uint16 GetInstanceMemorySize() const override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
};
