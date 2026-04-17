// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/GetLockOnTargetService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/AIControllerBase.h"
#include "Blueprint/Component/Action.h"
#include "Blueprint/Interface/CharacterInterface.h"
UGetLockOnTargetService::UGetLockOnTargetService()
{
    NodeName = "GetLockOnTarget";
    bNotifyTick = true;
    Interval = 0.5f;
    RandomDeviation = 0.1f;
    LockOnTargetKeyRef.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UGetLockOnTargetService, LockOnTargetKeyRef), AActor::StaticClass());
    DistanceFromLockOnTargetKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UGetLockOnTargetService, DistanceFromLockOnTargetKey));
}
void UGetLockOnTargetService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
    AAIControllerBase * OwnerController = Cast<AAIControllerBase>(OwnerComp.GetAIOwner());
    UAction* OwnerActionComponent =IsValid(OwnerController)?(OwnerController->ActionComponent).Get():nullptr;
    UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	APawn* Pawn = OwnerController->GetPawn();
    if (IsValid(OwnerActionComponent)&& IsValid(BlackboardComponent)&&IsValid(Pawn))
    {
        AActor* Target =OwnerActionComponent->CurrentLockTarget.Get();
        if (!IsValid(Target))
        {
            Target = OwnerActionComponent->LockTarget(DetectRadius, DetectActor, false);
            if (IsValid(Target))
            {
                BlackboardComponent->SetValueAsObject(LockOnTargetKeyRef.SelectedKeyName, Target); 
                OwnerController->SetFocus(Target);
            }
        }
        if (IsValid(Target))
        {
            BlackboardComponent->SetValueAsFloat(DistanceFromLockOnTargetKey.SelectedKeyName, FVector::Dist(Pawn->GetActorLocation(), Target->GetActorLocation()));
            OwnerController->SetFocus(Target);
        }
    }
}
