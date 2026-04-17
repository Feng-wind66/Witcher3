// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorator/GetDistanceToLockOnTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/AIControllerBase.h"
UGetDistanceToLockOnTarget::UGetDistanceToLockOnTarget()
{
	NodeName = "DistanceInRange";
	DistanceKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UGetDistanceToLockOnTarget, DistanceKey));
}

bool UGetDistanceToLockOnTarget::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if (UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent())
	{
		float Distance = Blackboard->GetValueAsFloat(DistanceKey.SelectedKeyName);
		if (Distance >= MinExecuteRadius && Distance <= MaxExecuteRadius)return true;
		return false;
	}
	return false;
}
