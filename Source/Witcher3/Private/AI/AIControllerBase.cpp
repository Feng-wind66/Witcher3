// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIControllerBase.h"
#include "Character/Enemy/BasicEnemy.h"
#include "BehaviorTree/BehaviorTree.h"
void AAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	ABasicEnemy* Enemy = Cast<ABasicEnemy>(InPawn);
	GetWorld()->GetTimerManager().SetTimer(
		InitHandle,
		FTimerDelegate::CreateWeakLambda(this, [this, Enemy]() {
		if (IsValid(Enemy) && IsValid(Enemy->BehaviorTreeAsset))
		{
			RunBehaviorTree(Enemy->BehaviorTreeAsset);
			ActionComponent = Enemy->Action;
		}
	    }),
		0.2f,
		false
	);
}
