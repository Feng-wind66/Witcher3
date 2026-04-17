// Fill out your copyright notice in the Description page of Project Settings.


#include "VFX/Magic/Axii.h"
#include"Macro/Macro.h"
#include "AIController.h"
#include "Character/Enemy/BasicEnemy.h"
#include "NiagaraComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/Interface/CharacterInterface.h"
#include "NiagaraSystem.h"
#include "Blueprint/Component/Action.h"
#include "NiagaraFunctionLibrary.h"

void AAxii::BeginPlay()
{
	Super::BeginPlay();
	if (!Releaser.IsValid() || !Cast<ICharacterInterface>(Releaser))return;
	UAction* ReleaserAction = ICharacterInterface::Execute_GetActionComponent(Releaser.Get());
	TargetActor = IsValid(ReleaserAction) ? ReleaserAction->CurrentLockTarget : nullptr;
	if (!TargetActor.IsValid() || !Cast<ICharacterInterface>(TargetActor))return;
	TargetActorAction = ICharacterInterface::Execute_GetActionComponent(TargetActor.Get());
	if (!TargetActorAction.IsValid())return;
	if (Cast<ABasicEnemy>(TargetActorAction->CurrentLockTarget))return;
	if (AActor* NewTarget = TargetActorAction->LockTarget(3000,ABasicEnemy::StaticClass(),false))
	{
		AAIController* AIController = Cast<AAIController>(TargetActor->GetInstigatorController());
		if (!IsValid(AIController))return;
		TargetActorBlackboard = AIController->GetBlackboardComponent();
		if (TargetActorBlackboard.IsValid())
		{
			TargetActorBlackboard->SetValueAsObject(TEXT("LockOnTargetRef"), NewTarget);
			DELAY(GetWorld()->GetTimerManager(), this,AxiiEndTimerHandle,15,&AAxii::AxiiEnd);
			if (USkeletalMeshComponent* Mesh = Cast<ACharacter>(TargetActor) ? Cast<ACharacter>(TargetActor)->GetMesh() : nullptr)
			{
				AxiiNiagara=UNiagaraFunctionLibrary::SpawnSystemAttached(
					TargetNiagara,               
					Mesh,                 
					FName(TEXT("Axii")),
					FVector::ZeroVector,           
					FRotator::ZeroRotator,         
					EAttachLocation::SnapToTarget, 
					false                         
				);
				TargetActorAction->AxiiActor = this;
			}
		}
	}
	
	
}

void AAxii::Destroyed()
{
	if (IsValid(AxiiNiagara))
	{
		AxiiNiagara->DestroyComponent();
	}
}

void AAxii::AxiiEnd()
{
	if (TargetActorAction.IsValid()&&TargetActorBlackboard.IsValid())
	{
		TargetActorAction->CurrentLockTarget = nullptr;
		TargetActorBlackboard->ClearValue(TEXT("LockOnTargetRef"));
		//交给行为树处理
	}
	AxiiNiagara->DestroyComponent();
	Destroy();
}
