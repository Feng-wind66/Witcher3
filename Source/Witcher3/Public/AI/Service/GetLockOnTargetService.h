// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "GetLockOnTargetService.generated.h"
/**
 * 
 */
UCLASS()
class WITCHER3_API UGetLockOnTargetService : public UBTService
{
	GENERATED_BODY()
public:
	UGetLockOnTargetService();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<AActor>DetectActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DetectRadius;
	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector LockOnTargetKeyRef;
	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector DistanceFromLockOnTargetKey;
};
