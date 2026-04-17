// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EQS/EnvQueryContext/GetLockOnTargetContext.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "Character/Darke.h"
#include <Kismet/GameplayStatics.h>
void UGetLockOnTargetContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
    
    UObject* QueryOwner = QueryInstance.Owner.Get();
    AAIController* AIController = Cast<AAIController>(QueryOwner);
    if (!IsValid(AIController))
    {
        if (APawn* PawnOwner = Cast<APawn>(QueryOwner))
        {
            AIController = Cast<AAIController>(PawnOwner->GetController());
        }
    }
    if (!IsValid(AIController))return;
    if (UBlackboardComponent*BlackboardCompnent=AIController->GetBlackboardComponent())
    {
        UEnvQueryItemType_Actor::SetContextHelper(ContextData, Cast<AActor>(BlackboardCompnent->GetValueAsObject(TargetKeyName)));
    }
}
