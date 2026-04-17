// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/LightAttack.h"
#include "Blueprint/Component/Action.h"
#include "GAS/ASC.h"
#include "AIController.h"
#include "Character/BaseCharacter.h"
uint16 ULightAttack::GetInstanceMemorySize() const
{
	return sizeof(FLightAttackBTTaskMemory);
}
EBTNodeResult::Type ULightAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    if (!IsValid(OwnerComp.GetAIOwner()))return EBTNodeResult::Failed;
	ABaseCharacter*Character=Cast<ABaseCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!IsValid(Character))return EBTNodeResult::Failed;
    UASC* OwnerASC = Character->ASC;
    UAction* OwnerActionComponent = Character->Action;
	if (IsValid(OwnerASC)&&OwnerActionComponent)
	{
        FGameplayTag AttackTag = FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Combat.Combo.LightAttack")));
        UBehaviorTreeComponent* OwnerCompPtr = &OwnerComp;
        FLightAttackBTTaskMemory* MyMem = CastInstanceNodeMemory<FLightAttackBTTaskMemory>(NodeMemory);
        TWeakObjectPtr<ULightAttack> TaskWeakPtr(this);
        auto LambdaDelegate = [OwnerASC, OwnerCompPtr, AttackTag, TaskWeakPtr](const FAbilityEndedData& Data)
        {
            if (!TaskWeakPtr.IsValid())return;
            if (IsValid(OwnerCompPtr) && Data.AbilitySpecHandle.IsValid())
            {
                if (const FGameplayAbilitySpec* Spec = OwnerASC->FindAbilitySpecFromHandle(Data.AbilitySpecHandle))
                {
                    if (Spec->Ability->AbilityTags.HasTag(AttackTag))
                    {
                        TaskWeakPtr->FinishLatentTask(*OwnerCompPtr, EBTNodeResult::Succeeded);
                    }
                }
            }
        };
        MyMem->OnAbilityEndedDelegateHandle = OwnerASC->OnAbilityEnded.AddLambda(LambdaDelegate);
        if (OwnerActionComponent->TriggerAction(AttackTag, EPriority::EP_Supreme, true))
        { 
            return EBTNodeResult::InProgress;
        }
        else
        {
            OwnerASC->OnAbilityEnded.Remove(MyMem->OnAbilityEndedDelegateHandle);
            MyMem->OnAbilityEndedDelegateHandle.Reset();
            return EBTNodeResult::Failed;
        }
	}
	return EBTNodeResult::Failed;
}

void ULightAttack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
    Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
    AAIController* AIController = OwnerComp.GetAIOwner();
    ABaseCharacter* Character =AIController?Cast<ABaseCharacter>(AIController->GetPawn()):nullptr;
	UASC* OwnerASC = Character ? Character->ASC : nullptr;
    FLightAttackBTTaskMemory* MyMem = CastInstanceNodeMemory<FLightAttackBTTaskMemory>(NodeMemory);
    if (IsValid(OwnerASC)&&MyMem)
    {
		OwnerASC->OnAbilityEnded.Remove(MyMem->OnAbilityEndedDelegateHandle);
        MyMem->OnAbilityEndedDelegateHandle.Reset();
    }
    if (TaskResult == EBTNodeResult::Aborted && IsValid(OwnerASC))
    {
        FGameplayTagContainer CancelTags(FGameplayTag::RequestGameplayTag(FName("Action.Combat.Combo.LightAttack")));
        OwnerASC->CancelAbilities(&CancelTags);
    }
}
