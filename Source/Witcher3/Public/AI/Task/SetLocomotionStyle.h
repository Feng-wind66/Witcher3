// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SetLocomotionStyle.generated.h"
/**
 * 
 */
UCLASS()
class WITCHER3_API USetLocomotionStyle : public UBTTaskNode
{
	GENERATED_BODY()
public:
	USetLocomotionStyle();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	UPROPERTY(EditAnywhere, Category = "Condition")
	FBlackboardKeySelector CurrentLocomotionKey;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 LocomotionState;
};
