// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/SetLocomotionStyle.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Character/BaseCharacter.h"
USetLocomotionStyle::USetLocomotionStyle()
{
	NodeName = "SetLocomotionStyle";
	CurrentLocomotionKey.AddIntFilter(this, GET_MEMBER_NAME_CHECKED(USetLocomotionStyle, CurrentLocomotionKey));
}
EBTNodeResult::Type USetLocomotionStyle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent*BlackboardComponent=OwnerComp.GetBlackboardComponent();
	if (IsValid(BlackboardComponent))
	{
		if(BlackboardComponent->GetValueAsInt(CurrentLocomotionKey.SelectedKeyName)==LocomotionState)return EBTNodeResult::Succeeded;
		AAIController* AIController = OwnerComp.GetAIOwner();
		if (IsValid(AIController))
		{
			if (ABaseCharacter* Character = Cast<ABaseCharacter>(AIController->GetPawn()))
			{
				Character->SetLocomotionStyle((ELocomotionState)LocomotionState);
				BlackboardComponent->SetValueAsInt(CurrentLocomotionKey.SelectedKeyName, LocomotionState);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}
