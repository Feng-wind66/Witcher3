// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/Montage/BaseMontageGameplayAbility.h"
#include "GAS/ASC.h"
#include"GAS/AS.h"
#include "Character/Enemy/BasicEnemy.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Blueprint/Component/Action.h"

void UBaseMontageGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	Init();
	if (!Montage)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	if (CommitAbility(Handle,ActorInfo,ActivationInfo)&&ASCRef&& IsValid(ActionRef))
	{
		
		if (ActionRef->CurrentActionType.MatchesTag(FGameplayTag::RequestGameplayTag("Action.Combat.Finisher")))
		{
			ActionRef->SurroundingAIPauseAttack(3000.f, ABasicEnemy::StaticClass());
		}
		//可以再添加其他判断
		UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			NAME_None,
			Montage,
			PlayRate * ASCRef->GetNumericAttribute(UAS::GetAttackSpeedAttribute()),
			NAME_None,                 
			false,                    
			1.f                        
		);
		MontageTask->OnCompleted.AddDynamic(this,&UBaseMontageGameplayAbility::OnMontageEnded);
		MontageTask->OnBlendOut.AddDynamic(this, &UBaseMontageGameplayAbility::OnMontageEnded);
		MontageTask->OnCancelled.AddDynamic(this, &UBaseMontageGameplayAbility::OnMontageEnded);
		MontageTask->OnInterrupted.AddDynamic(this, &UBaseMontageGameplayAbility::OnMontageQuit);
		MontageTask->ReadyForActivation();
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true,true);
	}
}

void UBaseMontageGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsValid(ActionRef))
	{
		if (!ActionRef->PausedAIActors.IsEmpty())
		{
			ActionRef->SurroundingAIResumeAttack();
		}
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}


void UBaseMontageGameplayAbility::Init_Implementation()
{
	if (Owner = GetAvatarActorFromActorInfo())
	{
		ASCRef = GetAbilitySystemComponentFromActorInfo();
		if (ActionRef = Owner->FindComponentByClass<UAction>())
		{
			if (ActionRef->CurrentActionType.MatchesTag(FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Hit")))))
			{
				Montage = ActionRef->CurrentHitInfo.Montage;
				CoolDown = 0;
				CoolDownTag = FGameplayTag();
				PlayRate = ActionRef->CurrentHitInfo.PlayRate;
				Cost = 0;
				return;
			}
			else
			{
				Montage = ActionRef->CurrentActionInfo.ActionMontage;
				CoolDown = ActionRef->CurrentActionInfo.Cooldown;
				CoolDownTag = ActionRef->CurrentActionInfo.CooldownTag;
				PlayRate = ActionRef->CurrentActionInfo.PlayRate;
				Cost = ActionRef->CurrentActionInfo.Cost;
			}
		}
	}
}
void UBaseMontageGameplayAbility::OnMontageEnded()
{
	if (IsValid(ActionRef))
	{
		ActionRef->ResetAction();
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UBaseMontageGameplayAbility::OnMontageQuit()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
