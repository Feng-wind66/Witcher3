// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "GetDistanceToLockOnTarget.generated.h"

/**
 * 
 */
UCLASS()
class WITCHER3_API UGetDistanceToLockOnTarget : public UBTDecorator
{
	GENERATED_BODY()
public:
	UGetDistanceToLockOnTarget();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	UPROPERTY(EditAnywhere, Category = "Condition")
	FBlackboardKeySelector DistanceKey;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float MinExecuteRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxExecuteRadius;
};
