// Fill out your copyright notice in the Description page of Project Settings.


#include "Blueprint/Component/Action.h"
#include "Blueprint/Component/Interact.h"
#include "Blueprint/Component/Equipment.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Character/Darke.h"
#include "GAS/ASC.h"
#include"GAS/AS.h"
#include "Kismet/KismetSystemLibrary.h"
#include "FunctionLibrary/CommonFuction.h"
#include "Blueprint/Interface/CharacterInterface.h"
#include "Blueprint/Interface/TargettingInterface.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include"Data/CharacterInfoData.h"
#include "Core/GameMode_1.h"
#include "Kismet/KismetMathLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
UAction::UAction()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UAction::BeginPlay()
{
	Super::BeginPlay();
}
void UAction::InitActorComponent_Implementation(UCharacterInfoData* Data)//在BaseCharacter中调用
{
	OwnerInfoData = Data;
	GameModeRef = Cast<AGameMode_1>(GetWorld()->GetAuthGameMode());
	OwnerRef = GetOwner();
	ASCRef = OwnerRef->FindComponentByClass<UASC>();
	PlayerRef = Cast<ADarke>(UCommonFuction::GetDarkePtr(OwnerRef));
	if (OwnerRef)
	{
		OwnerMovementComponent = OwnerRef->FindComponentByClass<UCharacterMovementComponent>();
		OwnerMesh = OwnerRef->FindComponentByClass<USkeletalMeshComponent>();
		if (OwnerRef->ActorHasTag(FName("Player")))
			OwnerType = EOwnerType::EOT_Player;
		else if (OwnerRef->ActorHasTag(FName("Monster")) && OwnerInfoData && OwnerInfoData->MonsterType == EMonsterType::EMT_Dragon)
			OwnerType = EOwnerType::EOT_Dragon;
		else
			OwnerType = EOwnerType::EOT_NPC;
		if (Cast<ICharacterInterface>(OwnerRef))
		{
			EquipmentComponent = ICharacterInterface::Execute_GetEquipmentComponent(OwnerRef);
		}
		CharacterRef = Cast<ABaseCharacter>(OwnerRef);
	}
}
void UAction::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	CurrentDeltaTime = DeltaTime;
    ExecuteInputBuffer();
}
void UAction::SetCurrentActionState_Implementation(EActionState ActionState)
{
	LastActionState = CurrentActionState;
	CurrentActionState = ActionState;
	if (CurrentActionState == EActionState::EAS_InCombat)
	{
		bIsInCombat = true;
	}
	else if(CurrentActionState==EActionState::EAS_OutOfCombat)
	{
		bIsInCombat = false;
	}
	if (OwnerType == EOwnerType::EOT_Player && GameModeRef)
	{
		ICharacterInterface::Execute_ChangeCurrentActionState(GameModeRef, ActionState);
	}
}
bool UAction::TriggerAction_Implementation(FGameplayTag NewAction, EPriority Priority, bool bUseInputBuffer)
{
	if (CurrentActionPriority < Priority)
	{
		return HandleNewAction(NewAction,Priority);
	}
	else
	{
		FGameplayTagContainer TagContainer;
		TagContainer.AddTag(FGameplayTag::RequestGameplayTag("Action.CanCombo"));
		TagContainer.AddTag(FGameplayTag::RequestGameplayTag("Action.NoAction"));
		if (NewAction.MatchesAny(TagContainer))
		{
			return HandleNewAction(NewAction,Priority);
		}
		else
		{
			if (bUseInputBuffer)
			{
				InputBuffer(NewAction);
			}
		}
	}
	return false;
}

void  UAction::SetPriority_Implementation(EPriority NewPriority)
{	
	LastActionPriority = CurrentActionPriority;
	CurrentActionPriority = NewPriority;
}
void UAction::SetCurrentActionTag_Implementation(FGameplayTag NewActionTag)
{
	CurrentActionTag = NewActionTag;
}
bool UAction::HandleNewAction_Implementation(FGameplayTag& NewAction,EPriority Priority)
{
	SetPriority(Priority);
	SetCurrentActionTag(NewAction);
	if (CurrentActionTag.MatchesTag( FGameplayTag::RequestGameplayTag(FName(TEXT("Action.NoAction")))))
	{
		SetPriority(EPriority::EP_NotApplicable);
		ResetCombos();
		return false;
	}
	else if (CurrentActionTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName(TEXT("Action.CanCombo")))))
	{
		SetPriority(EPriority::EP_VeryLow);
		return false;
	}
	else
	{
		if (OwnerType == EOwnerType::EOT_Player || OwnerType == EOwnerType::EOT_NPC)
		{
			if (CurrentActionTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Common")))))
			{

			}
			else if (CurrentActionTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Combat")))))
			{
				if (CurrentActionTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Combat.Magic")))))
				{
					SetCurrentActionInfoByTag(CurrentActionTag);
				}
				else
				{
					switch (CurrentWeaponType)
					{
					case EWeaponType::EWT_LongSword:HandleLongSwordAction();
						break;
					case EWeaponType::EWT_OneHandedSword:
						break;
					case EWeaponType::EWT_Club:
						break;
					case EWeaponType::EWT_Crossbow_RotateOnce:
						break;
					default:
						break;
					}
				}
			}
		}
	}
	CurrentActionType = CurrentActionInfo.ActionType;
	if (ASCRef)
	{
		if (ASCRef->TryActivateAbilitiesByTag(FGameplayTagContainer(CurrentActionType)))
		{
			return true;
		}
	}
	return false;
}
void UAction::ResetCombos_Implementation()
{
	CurrentComboCounter = 0;
}
void UAction::ResetAction_Implementation()
{
	TriggerAction(FGameplayTag::RequestGameplayTag(FName(TEXT("Action.NoAction"))), EPriority::EP_Supreme);
}

void UAction::HandleLongSwordAction_Implementation()
{
	if (!IsValid(OwnerRef) || !Cast<ICharacterInterface>(OwnerRef))return;
	if (CurrentActionTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Combat.Combo")))))
	{
		if (CurrentActionTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Combat.Combo.LightAttack")))))
		{
			SetCurrentActionInfoByTag(RandomGetComboActionTag());
			ICharacterInterface::Execute_ApplyEnergyCostAPI(OwnerRef,20);
		}
	}
	else if (CurrentActionTag.MatchesTag( FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Combat.Finisher")))))
	{
		SetCurrentActionInfoByTag(CurrentActionTag);
	}
	else if (CurrentActionTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Combat.Dodge")))))
	{
		if (CurrentActionTag.MatchesTagExact(FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Combat.Dodge.Roll_F")))))
		{
			SetCurrentActionInfoByTag(CurrentActionTag);
			ICharacterInterface::Execute_ApplyEnergyCostAPI(OwnerRef, 30);
		}
		else
		{
			ChooseDodge();
			ICharacterInterface::Execute_ApplyEnergyCostAPI(OwnerRef, 20);
		}
	}
	else
	{
		SetCurrentActionInfoByTag(CurrentActionTag);
	}
}
AActor* UAction::LockTarget_Implementation(float Radius, TSubclassOf<AActor> ActorClass, bool ResetVariables)
{
	if (ResetVariables)
	{
		ResetLockTargetVariables();
	}
	DetectRadius = Radius;
	AActor*ClosestActor=GetClosestActorInRange(Radius, ActorClass);
	if (!Cast<ICharacterInterface>(ClosestActor) || !Cast<ICharacterInterface>(OwnerRef))return nullptr;
	if (OwnerRef->ActorHasTag(FName(TEXT("Enemy"))))
	{
		DetectActorClass = ActorClass;//后面行为树会回正
		if (DetectActorClass->IsChildOf(ADarke::StaticClass())&& AxiiActor.IsValid())
		{
			AxiiActor->Destroy();
		}//移除Axii特效
	}
	if (IsValid(ClosestActor))
	{
		if (CurrentLockTarget.IsValid())
		{
			ITargettingInterface::Execute_ToggleHPBarVisibility(CurrentLockTarget.Get(), false);
			if (ABaseCharacter* OldTarget = Cast<ABaseCharacter>(CurrentLockTarget.Get()))
			{
				OldTarget->OnCharacterDeathEvent.RemoveDynamic(this, &UAction::HandleDeathEvent);
			}
		}
		if (ABaseCharacter* NewTarget = Cast<ABaseCharacter>(ClosestActor))
		{
			NewTarget->OnCharacterDeathEvent.AddDynamic(this, &UAction::HandleDeathEvent);
		}
		CurrentLockTarget = ClosestActor;
		SetLockOnTargetType(CurrentLockTarget.Get());
		if (ClosestActor->ActorHasTag(FName(TEXT("Enemy"))))//原本是判断ownertype的
		{
			ITargettingInterface::Execute_ToggleHPBarVisibility(ClosestActor,true);
		}
		RecentlyLockedTargets.AddUnique(ClosestActor);
	}
	return ClosestActor;
}

void UAction::ResetLockTargetVariables_Implementation()
{
	TargetsInRange.Empty();
	RecentlyLockedTargets.Empty();
}

TArray<AActor*> UAction::GetTargetsInRange_Implementation(float Radius, TSubclassOf<AActor> ActorClass)
{
	TargetsInRange.Empty();
	TArray<AActor*> FreshLockTragetInRange;
	if (Radius>0&&OwnerRef)
	{
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(OwnerRef);
		TArray<AActor*> OutActors;
		if(UKismetSystemLibrary::SphereOverlapActors(this, OwnerRef->GetTargetLocation(), Radius, ObjectTypes, ActorClass, ActorsToIgnore, OutActors))
		{ 
			for (AActor* Actor : OutActors)
			{
				if (CanLockOnActor(Actor))
				{
					TargetsInRange.AddUnique(Actor);
					if (!RecentlyLockedTargets.Contains(Actor))
					{
						FreshLockTragetInRange.Add(Actor);
					}
				}
			}
			if (FreshLockTragetInRange.IsEmpty())
			{
				TArray<AActor*>ResultActors;
				RecentlyLockedTargets.Empty();
				for (auto&WeakPtr:TargetsInRange)
				{
					if (WeakPtr.IsValid())
					{
						ResultActors.Add(WeakPtr.Get());
					}
				}
				return ResultActors;
			}
			else
			{
				return FreshLockTragetInRange;
			}
		}
		else
		{
			ClearCurrentLockTarget();
		}
	}
	return TArray<AActor*>();
}

AActor* UAction::GetClosestActorInRange_Implementation(float Radius, TSubclassOf<AActor> ActorClass)
{
	TArray<AActor*>TargetsInRangeArray;
	TargetsInRangeArray=GetTargetsInRange(Radius, ActorClass);
	AActor* ClosestActor = nullptr;
	CloseDistance = 0;
	if (OwnerRef)
	{
		for (int i = 0; i < TargetsInRangeArray.Num(); ++i)
		{
			if (i == 0)
			{
				ClosestActor = TargetsInRangeArray[0];
				CloseDistance = OwnerRef->GetDistanceTo(ClosestActor);
			}
			else
			{
				float Distance = OwnerRef->GetDistanceTo(TargetsInRangeArray[i]);
				if (Distance < CloseDistance)
				{
					ClosestActor = TargetsInRangeArray[i];
					CloseDistance = Distance;
				}
			}
		}
		return ClosestActor;
	}
	return nullptr;
}
EOwnerType UAction::SetLockOnTargetType_Implementation(AActor* LockActor)
{
	if (LockActor->ActorHasTag(FName(TEXT("Player"))))
	{
		LockTargetType = EOwnerType::EOT_Player;
	}
	else if(LockActor->ActorHasTag(FName(TEXT("Dragon"))))
	{
		LockTargetType = EOwnerType::EOT_Dragon;
	}
	else
	{
		LockTargetType = EOwnerType::EOT_NPC;
	}
	return LockTargetType;
}

void UAction::ClearCurrentLockTarget_Implementation()
{
	if (CurrentLockTarget.IsValid())
	{
		ITargettingInterface::Execute_ToggleHPBarVisibility(CurrentLockTarget.Get(), false);
	}
	CurrentLockTarget.Reset();
}

FGameplayTag UAction::RandomGetComboActionTag_Implementation()
{
	int32 LightCombo=FMath::RandRange(0, 5);
	switch (LightCombo)
	{
	case 0:return FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Combat.Combo.LightAttack.00")));
	case 1:return FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Combat.Combo.LightAttack.01")));
	case 2:return FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Combat.Combo.LightAttack.02")));
	case 3:return FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Combat.Combo.LightAttack.03")));
	case 4:return FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Combat.Combo.LightAttack.04")));
	case 5:return FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Combat.Combo.LightAttack.05")));
	default:return FGameplayTag();
	}
}
void UAction::InputBuffer_Implementation(FGameplayTag BufferAction)
{
	if (CurrentActionState == EActionState::EAS_InUIInteraction_Dialogue || CurrentActionState == EActionState::EAS_Death)
	{
		return;
	}
	if (ICharacterInterface::Execute_NotHasAnimNotify(OwnerRef, InputBufferNotifyState, false))
	{
		PendingAction = BufferAction;
		LastInputRequestTime = GetWorld()->GetTimeSeconds();
	}
}

void UAction::ExecuteInputBuffer_Implementation()
{
	if (OwnerType == EOwnerType::EOT_Player)
	{
		double CurrentTime = GetWorld()->GetTimeSeconds();
		if ((CurrentTime - LastInputRequestTime) > InputBufferDuration|| CurrentActionState == EActionState::EAS_CombatFinisher_Attacker || CurrentActionState == EActionState::EAS_CombatFinisher_Receiver || CurrentActionState == EActionState::EAS_InUIInteraction_Dialogue || CurrentActionState == EActionState::EAS_Death)
		{
			PendingAction = FGameplayTag();
			return;
		}
		if (PendingAction.IsValid()&&(CurrentActionState == EActionState::EAS_InCombat || CurrentActionState == EActionState::EAS_OutOfCombat))
		{
			FGameplayTagContainer TagContainer;
			TagContainer.AddTag(FGameplayTag::RequestGameplayTag("Action.CanCombo"));
			TagContainer.AddTag(FGameplayTag::RequestGameplayTag("Action.NoAction"));
			if (CurrentActionTag.MatchesAny(TagContainer)&&!PendingAction.MatchesTagExact(FGameplayTag::RequestGameplayTag(FName(TEXT("Action.NoAction")))))
			{
				EPriority Priority = EPriority::EP_Medium;
				Priority = PendingAction.MatchesTag(FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Combat.Dodge")))) ? PendingAction.MatchesTag(FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Combat.Dodge.Roll_F")))) ? EPriority::EP_VeryHigh : EPriority::EP_High : EPriority::EP_Medium;
				Priority = PendingAction.MatchesTag(FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Combat.Magic")))) ? EPriority::EP_High : Priority;
				if (TriggerAction(PendingAction, Priority, false))
				{
					PendingAction = FGameplayTag();
				}
			}
		}
	}
}
void UAction::GetMotionWarpingLocationAndRotation_Implementation()
{
	if (CurrentActionType.MatchesTag(FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Combat.Combo.LightAttack"))))|| CurrentActionType.MatchesTag(FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Combat.Combo.HeavyAttack"))))||CurrentActionType.MatchesTag(FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Combat.Magic")))))
	{
		UpdateAttackerToTargetLocationAndRotation();
	}
	if (CurrentActionType.MatchesTag(FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Hit")))))
	{
		if (CurrentAttackOwnerActorRef.IsValid())
		{
			UpdateHitLocationAndRotation(CurrentAttackOwnerActorRef.Get());
		}
	}
	if (CurrentActionType.MatchesTag(FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Combat.Finisher")))))
	{
		UpdateFinisherLocationAndRotation();
	}
	if (CurrentActionType.MatchesTag(FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Combat.Dodge")))))
	{
		UpdateDodgeLocationAndRotation();
	}
}
void UAction::UpdateAttackerToTargetLocationAndRotation_Implementation()
{
	if (OwnerRef && CurrentLockTarget.IsValid())
	{
		if (OwnerRef->GetDistanceTo(CurrentLockTarget.Get()) <= CurrentActionInfo.MotionWarping.TriggerRange)
		{
			float TargetCapsuleHalfRadius = 0;
			if (ACharacter* Character = Cast<ACharacter>(CurrentLockTarget))
			{
				TargetCapsuleHalfRadius = Character->GetCapsuleComponent()->GetScaledCapsuleRadius();
			}
			FVector TargetToAttackerNormalizedVector = (OwnerRef->GetActorLocation() - CurrentLockTarget->GetActorLocation()).GetSafeNormal();
			FVector FinalLocation_X = CurrentLockTarget->GetActorLocation() + (CurrentActionInfo.MotionWarping.WarpOffset_X+ TargetCapsuleHalfRadius)* TargetToAttackerNormalizedVector;
			FVector FinalLocation_Y = (CurrentActionInfo.MotionWarping.WarpOffset_Y+ TargetCapsuleHalfRadius) * OwnerRef->GetActorRightVector();
			FVector FinalLocation_Z = (CurrentActionInfo.MotionWarping.WarpOffset_Z+ TargetCapsuleHalfRadius) * OwnerRef->GetActorUpVector();
			FinalMotionWarpingLocation = FinalLocation_X + FinalLocation_Y + FinalLocation_Z;
			FinalMotionWarpingRotation = UKismetMathLibrary::FindLookAtRotation(OwnerRef->GetActorLocation(), CurrentLockTarget->GetActorLocation());
			bCanMotionWarping = true;
		}
	}
	else
	{
		bCanMotionWarping = false;
	}
}
void UAction::MotionWarping_Implementation()
{
	static FGameplayTagContainer ActionMotionWarpingTags;
	if (ActionMotionWarpingTags.IsEmpty())
	{
		ActionMotionWarpingTags.AddTag(FGameplayTag::RequestGameplayTag("Action.Combat.Combo.LightAttack"));
		ActionMotionWarpingTags.AddTag(FGameplayTag::RequestGameplayTag("Action.Combat.Combo.HeavyAttack"));
		ActionMotionWarpingTags.AddTag(FGameplayTag::RequestGameplayTag("Action.Combat.Dodge"));
		ActionMotionWarpingTags.AddTag(FGameplayTag::RequestGameplayTag("Action.Combat.Magic"));
		ActionMotionWarpingTags.AddTag(FGameplayTag::RequestGameplayTag("Action.Combat.Finisher"));
		ActionMotionWarpingTags.AddTag(FGameplayTag::RequestGameplayTag("Action.Common"));
		ActionMotionWarpingTags.AddTag(FGameplayTag::RequestGameplayTag("Action.Combat.MonsterAttack"));
	}
	EWhichMotionWarping WhichMotionWarping;
	if (CurrentActionType.MatchesAny(ActionMotionWarpingTags))
	{
		WhichMotionWarping = CurrentActionInfo.MotionWarping.MotionWarpingType;
	}
	if (CurrentActionType.MatchesTag(FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Hit")))))
	{
		WhichMotionWarping = CurrentHitInfo.MotionWarping.MotionWarpingType;
	}
	if (WhichMotionWarping == EWhichMotionWarping::EWMW_MeleeAttack)
	{
		if (CurrentLockTarget.IsValid() && OwnerRef && OwnerRef->GetDistanceTo(CurrentLockTarget.Get()) <= CurrentActionInfo.MotionWarping.TriggerRange)
		{
			if (bCanMotionWarping)
			{
				MotionWarpingOnGroundAttacker();
			}
		}
		else
		{
			bCanMotionWarping = false;
		}
	}
	else if (WhichMotionWarping == EWhichMotionWarping::EWMW_Hit)
	{
		MotionWarpingOnGroundHit();
	}
	else if (WhichMotionWarping == EWhichMotionWarping::EWMW_Roll|| WhichMotionWarping == EWhichMotionWarping::EWMW_Dodge)
	{
		MotionWarpingOnGroundDodge();
	}
}

void UAction::MotionWarpingOnGroundAttacker_Implementation()
{
	if (CurrentLockTarget.IsValid()&&IsValid(OwnerRef))
	{
		if (CurrentActionInfo.MotionWarping.WarpWhat == EWarpWhat::EWW_LocationOnly)
		{
			OwnerRef->SetActorLocation(FMath::VInterpTo(OwnerRef->GetActorLocation(), FinalMotionWarpingLocation, CurrentDeltaTime, CurrentActionInfo.MotionWarping.LocationInterpSpeed));
		}
		else if (CurrentActionInfo.MotionWarping.WarpWhat == EWarpWhat::EWW_RotationOnly)
		{
			OwnerRef->SetActorRotation(FRotator(0,FMath::RInterpTo(OwnerRef->GetActorRotation(), FinalMotionWarpingRotation, CurrentDeltaTime, CurrentActionInfo.MotionWarping.RotationInterpSpeed).Yaw,0));
		}
		else if (CurrentActionInfo.MotionWarping.WarpWhat == EWarpWhat::EWW_Both)
		{
			OwnerRef->SetActorLocation(FMath::VInterpTo(OwnerRef->GetActorLocation(), FinalMotionWarpingLocation, CurrentDeltaTime, CurrentActionInfo.MotionWarping.LocationInterpSpeed));
			OwnerRef->SetActorRotation(FRotator(0, FMath::RInterpTo(OwnerRef->GetActorRotation(), FinalMotionWarpingRotation, CurrentDeltaTime, CurrentActionInfo.MotionWarping.RotationInterpSpeed).Yaw, 0));
		}
	}
}

void UAction::ResetMotionWarping_Implementation()
{
	bCanMotionWarping = true;
	bCanRotate = true;
	CurveTime = 0;
}

void UAction::PlayHitAnim(AActor* Attacker, EWhichDamageType DamageType, bool bIsParry)
{
	CurrentAttackOwnerActorRef = Attacker;
	UAbilitySystemComponent* AttackerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Attacker);
	if (!Cast<ICharacterInterface>(OwnerRef)|| !Cast<ICharacterInterface>(Attacker)||!Cast<ITargettingInterface>(OwnerRef))return;
	UAction* AttackerAction = ICharacterInterface::Execute_GetActionComponent(Attacker);
	EquipmentComponent = EquipmentComponent.IsValid() ? EquipmentComponent : ICharacterInterface::Execute_GetEquipmentComponent(OwnerRef);
	if (!IsValid(ASCRef)||!IsValid(AttackerASC)||!IsValid(OwnerRef)||!EquipmentComponent.IsValid()||!IsValid(AttackerAction)||!OwnerInfoData)return;
	SetPriority(EPriority::EP_High);
	SetCurrentActionTag(FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Hit"))));
	double Angle=UKismetMathLibrary::FindRelativeLookAtRotation(OwnerRef->GetTransform(), Attacker->GetActorLocation()).Yaw;
	if (ASCRef->GetNumericAttribute(UAS::GetHealthAttribute())>0)
	{
		if (FMath::Abs(Angle) <= 110)
		{
			if (OwnerType != EOwnerType::EOT_Dragon)
			{
				if (!bIsParry)
				{
					if ((DamageType == EWhichDamageType::EWDT_MeleeWeapon || DamageType == EWhichDamageType::EWDT_Monster))
					{
						HandleMeleeHit(AttackerAction);
					}
					if (DamageType == EWhichDamageType::EWDT_Magic_Aard||DamageType==EWhichDamageType::EWDT_Magic_Igni||DamageType==EWhichDamageType::EWDT_Magic_Quen)
					{
						if (Angle >= -110 && Angle < -70)
						{
							int32 RandIndex = FMath::RandRange(2, 3);
							GetAndSetHitInfoByTag(MeleeHitReactions.IsValidIndex(RandIndex) ? MeleeHitReactions[RandIndex] : FGameplayTag());
						}
						else if (Angle >= -70 && Angle <= 70)
						{
							GetAndSetHitInfoByTag(MeleeHitReactions.IsValidIndex(1) ? MeleeHitReactions[1] : FGameplayTag());
						}
						else
						{
							int32 RandIndex = FMath::RandRange(4, 5);
							GetAndSetHitInfoByTag(MeleeHitReactions.IsValidIndex(RandIndex) ? MeleeHitReactions[RandIndex] : FGameplayTag());
						}
					}
				}
				else 
				{
					if ((DamageType == EWhichDamageType::EWDT_MeleeWeapon || DamageType == EWhichDamageType::EWDT_Monster))
					{
						if (EquipmentComponent->CurrentMainWeaponAnimType == EWeaponType::EWT_LongSword)
						{
							GetAndSetHitInfoByTag(AttackerAction->CurrentActionInfo.HitReaction.MeleeParry);
						}
						if (!bIsInParryState && OwnerInfoData)
						{
							bIsInParryState = true;
							DELAY(GetWorld()->GetTimerManager(), this, ParryTimeHandle, OwnerInfoData->ParrySustainTime, &UAction::EndParry);
						}
					}
				}//格挡
			}//人形生物受击
			else
			{

			}//龙的受击
		}//前方受击
		else
		{
			if (OwnerType != EOwnerType::EOT_Dragon)
			{
				if (DamageType==EWhichDamageType::EWDT_MeleeWeapon&&EquipmentComponent->CurrentMainWeaponAnimType == EWeaponType::EWT_LongSword)
				{
					GetAndSetHitInfoByTag(FGameplayTag::RequestGameplayTag(FName("Action.Hit.Melee.B")));
				}
				if (DamageType == EWhichDamageType::EWDT_Magic_Aard||DamageType==EWhichDamageType::EWDT_Magic_Igni || DamageType == EWhichDamageType::EWDT_Magic_Quen)
				{
					GetAndSetHitInfoByTag(FGameplayTag::RequestGameplayTag(FName("Action.Hit.Melee.B")));
				}
			}//人形生物受击
			else
			{

			}//龙的受击
		}//后方受击
		CurrentActionType = CurrentHitInfo.ActionType;
		if (ASCRef)
		{
			ASCRef->TryActivateAbilitiesByTag(FGameplayTagContainer(CurrentActionType));
		}
	}//受击
	else
	{
		LethalDamageType = DamageType;
	}
}

bool UAction::CanLockOnActor(AActor* Target)
{
	if (!Target||!Cast<ITargettingInterface>(Target))return false;
	return ITargettingInterface::Execute_IsCombatTargettable(Target);
}

void UAction::MotionWarpingOnGroundHit()
{
	if (FinalHitMotionWarpingLocation.IsNearlyZero())return;
	if (CurrentHitInfo.MotionWarping.WarpWhat == EWarpWhat::EWW_LocationOnly)
	{
		OwnerRef->SetActorLocation(FMath::VInterpTo(OwnerRef->GetActorLocation(), FinalHitMotionWarpingLocation, CurrentDeltaTime, CurrentHitInfo.MotionWarping.LocationInterpSpeed));
	}
	else if (CurrentHitInfo.MotionWarping.WarpWhat == EWarpWhat::EWW_RotationOnly)
	{
		OwnerRef->SetActorRotation(FRotator(0, FMath::RInterpTo(OwnerRef->GetActorRotation(), FinalHitMotionWarpingRotation, CurrentDeltaTime, CurrentHitInfo.MotionWarping.RotationInterpSpeed).Yaw, 0));
	}
	else if (CurrentHitInfo.MotionWarping.WarpWhat == EWarpWhat::EWW_Both)
	{
		OwnerRef->SetActorLocation(FMath::VInterpTo(OwnerRef->GetActorLocation(), FinalHitMotionWarpingLocation, CurrentDeltaTime, CurrentHitInfo.MotionWarping.LocationInterpSpeed));
		OwnerRef->SetActorRotation(FRotator(0, FMath::RInterpTo(OwnerRef->GetActorRotation(), FinalHitMotionWarpingRotation, CurrentDeltaTime, CurrentHitInfo.MotionWarping.RotationInterpSpeed).Yaw, 0));
	}
	else
	{
		OwnerRef->SetActorLocation(FMath::VInterpTo(OwnerRef->GetActorLocation(), FinalHitMotionWarpingLocation, CurrentDeltaTime, CurrentHitInfo.MotionWarping.LocationInterpSpeed));
		if (bCanRotate)
		{
			OwnerRef->SetActorRotation(FRotator(0, FinalHitMotionWarpingRotation.Yaw, 0));
			bCanRotate = false;
		}
	}//没发现剩下两个在他的蓝图案例有什么区别
}

void UAction::UpdateHitLocationAndRotation(AActor*Attacker)
{
	if (!Attacker || !OwnerRef)return;
	FVector AttackToOwenrVector = (OwnerRef->GetActorLocation()- Attacker->GetActorLocation()).GetSafeNormal();
	FVector FinalLocation_X = OwnerRef->GetActorLocation() + CurrentHitInfo.MotionWarping.WarpOffset_X  * AttackToOwenrVector;
	FVector FinalLocation_Y = CurrentHitInfo.MotionWarping.WarpOffset_Y * Attacker->GetActorRightVector();
	FVector FinalLocation_Z = CurrentHitInfo.MotionWarping.WarpOffset_Z* OwnerRef->GetActorUpVector();
	FinalHitMotionWarpingLocation = FinalLocation_X+FinalLocation_Y + FinalLocation_Z;
	if (CurrentHitInfo.MotionWarping.WarpWhat == EWarpWhat::EWW_Both_ReverseTotateOnce)
	{
		FinalHitMotionWarpingRotation = UKismetMathLibrary::FindLookAtRotation(Attacker->GetActorLocation(),OwnerRef->GetActorLocation());
	}
	else
	{
		FinalHitMotionWarpingRotation = UKismetMathLibrary::FindLookAtRotation(OwnerRef->GetActorLocation(), Attacker->GetActorLocation());
	}
}

bool UAction::CheakCanFinishered(AActor*Attacker)
{
	if (!Cast<ICharacterInterface>(Attacker))return false;
	UAction* AttackerAction = ICharacterInterface::Execute_GetActionComponent(Attacker);
	if (!IsValid(AttackerAction)||!IsValid(OwnerRef))return false;
	if (UCharacterInfoData*AttackerInfoData=AttackerAction->OwnerInfoData)
	{
		return AttackerInfoData->FinisherTirggerRate >= FMath::RandRange(0, 100) 
			&& Attacker->GetDistanceTo(OwnerRef) <= AttackerInfoData->FinisherTirggerRange 
			&& AttackerAction->CurrentLockTarget == OwnerRef;
	}
	return false;
}

void  UAction::GetAndSetHitInfoByTag(const FGameplayTag& HitTag)
{
	FHitInfo*HitInfo=OwnerInfoData ? OwnerInfoData->LongswordHitLists.Find(HitTag) : nullptr;
	CurrentHitInfo = HitInfo ? *HitInfo : CurrentHitInfo;
}

void UAction::HandleDeathEvent(AActor* DeadCharacter)
{
	if (CurrentLockTarget == DeadCharacter&&!LockTarget(DetectRadius, DetectActorClass, false)&&IsValid(DeadCharacter))
	{
		if (ABaseCharacter* Character = Cast<ABaseCharacter>(GetOwner()))
		{
			Character->bLockOn = false;
			ClearCurrentLockTarget(); 
			ResetLockTargetVariables();
			Character->bRestoreCameraFinish = false;
		}
	}
	if (UInteract* InteractComponent = Cast<ADarke>(OwnerRef)? Cast<ADarke>(OwnerRef)->InteractComponent:nullptr)
	{
		InteractComponent->WeakOverlappingActors.AddUnique(DeadCharacter);
	}
}

void UAction::ExecuteDeath(AActor* Killer)
{
	if (!IsValid(Killer))
	{
		GetAndSetHitInfoByTag(PendingDeathTag);
	}
	else
	{
		if (UAction* KillerAction = Cast<ICharacterInterface>(Killer) ? ICharacterInterface::Execute_GetActionComponent(Killer) : nullptr)
		{
			double Angle = UKismetMathLibrary::FindRelativeLookAtRotation(OwnerRef->GetTransform(), Killer->GetActorLocation()).Yaw;
			if (FMath::Abs(Angle) <= 110)
			{
				if (OwnerType != EOwnerType::EOT_Dragon)
				{
					if (CheakCanFinishered(Killer)&&KillerAction->CurrentLockTarget == OwnerRef)//加一层重复判断
					{
						KillerAction->SetCurrentActionState(EActionState::EAS_CombatFinisher_Attacker);
						SetCurrentActionState(EActionState::EAS_CombatFinisher_Receiver);
						PlayPreFinisherAnim();
						FTimerDelegate ExecuteFinishTimerDel;
						ExecuteFinishTimerDel.BindUObject(this, &UAction::ExecuteFinisher, KillerAction);
						GetWorld()->GetTimerManager().SetTimer(ExecuteFinishTimeHandle,ExecuteFinishTimerDel, 0.6f, false);
					}//触发处决
					else
					{
						if (LethalDamageType==EWhichDamageType::EWDT_MeleeWeapon||LethalDamageType==EWhichDamageType::EWDT_Monster)//EquipmentComponent->CurrentMainWeaponAnimType == EWeaponType::EWT_LongSword
						{
							GetAndSetHitInfoByTag(KillerAction->CurrentActionInfo.HitReaction.DeathHit);
						}
						else if (LethalDamageType == EWhichDamageType::EWDT_Magic_Igni)
						{
							GetAndSetHitInfoByTag(FGameplayTag::RequestGameplayTag(FName("Action.Hit.Death.F")));
						}
					}//正常死亡
				}//人形死亡
				else
				{

				}//龙的死亡
			}//前方死亡
			else
			{
				if (OwnerType != EOwnerType::EOT_Dragon)
				{
					GetAndSetHitInfoByTag(FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Hit.Death.B"))));
				}//人形死亡
				else
				{

				}//龙的死亡
			}//后方
		}
	}
	if (IsValid(OwnerRef))
	{
		if (APawn* Pawn = Cast<APawn>(OwnerRef))
		{
			if (AAIController* AIC = Cast<AAIController>(Pawn->GetController()))
			{
				AIC->GetBrainComponent()->StopLogic("Death");
				AIC->StopMovement();
				AIC->ClearFocus(EAIFocusPriority::Gameplay);
			}
		}
		if (UCapsuleComponent* Capsule = OwnerRef->FindComponentByClass<UCapsuleComponent>())
		{
			Capsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		}
	}//警用行为树和碰撞
	if (AxiiActor.IsValid())
	{
		AxiiActor->Destroy();
	}
	CurrentActionType = CurrentHitInfo.ActionType;
	if (ASCRef)
	{
		ASCRef->TryActivateAbilitiesByTag(FGameplayTagContainer(CurrentActionType));
	}
	SetCurrentActionState(EActionState::EAS_Death);
}

void UAction::PlayPreFinisherAnim()
{
	if (!EquipmentComponent.IsValid())return;
	if (EquipmentComponent->CurrentMainWeaponAnimType == EWeaponType::EWT_LongSword)
	{
		GetAndSetHitInfoByTag(FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Hit.Melee.F_Upper"))));
		CurrentActionType = CurrentHitInfo.ActionType;
		if (IsValid(ASCRef))
		{
			ASCRef->TryActivateAbilitiesByTag(FGameplayTagContainer(CurrentActionType));
		}
	}

}

void UAction::ExecuteFinisher(UAction* FinisherAction)
{
	if (!IsValid(FinisherAction)||!IsValid(OwnerRef))return;
	int32 RandIndex = FMath::RandRange(1,2);
	if (RandIndex == 1)
	{
		FinisherAction->TriggerAction(FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Combat.Finisher.Attacker.01"))), EPriority::EP_VeryHigh);
		TriggerAction(FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Combat.Finisher.Receiver.01"))), EPriority::EP_VeryHigh);
	}
	if (RandIndex == 2)
	{
		FinisherAction->TriggerAction(FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Combat.Finisher.Attacker.02"))), EPriority::EP_VeryHigh);
		TriggerAction(FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Combat.Finisher.Receiver.02"))), EPriority::EP_VeryHigh);
	}
	SetCurrentActionState(EActionState::EAS_Death);
}

void UAction::SetCurrentActionInfoByTag(FGameplayTag ActionTag)
{
	if (!IsValid(OwnerInfoData))return;
	if (ActionTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Combat.Finisher")))))
	{
		if (FActionInfo* ActionInfo = OwnerInfoData->LongswordFinisherList.Find(ActionTag))
		{
			CurrentActionInfo = *ActionInfo;
		}
	}
	else if (ActionTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Combat.Magic")))))
	{
		if (FActionInfo* ActionInfo = OwnerInfoData->CommonActionLists.Find(ActionTag))
		{
			CurrentActionInfo = *ActionInfo;
		}
	}
	else
	{
		if (FActionInfo* ActionInfo = OwnerInfoData->LongswordActionLists.Find(ActionTag))
		{
			CurrentActionInfo = *ActionInfo;
		}
	}
}
void UAction::UpdateFinisherLocationAndRotation()
{
	if (OwnerRef && CurrentLockTarget.IsValid())
	{
		FVector TargetForwardVector = CurrentLockTarget->GetActorForwardVector();
		FVector FinalLocation_X = CurrentLockTarget->GetActorLocation() + (TargetForwardVector * CurrentActionInfo.MotionWarping.WarpOffset_X);
		FVector FinalLocation_Y = CurrentLockTarget->GetActorRightVector() * CurrentActionInfo.MotionWarping.WarpOffset_Y;
		FVector FinalLocation_Z = CurrentLockTarget->GetActorUpVector() * CurrentActionInfo.MotionWarping.WarpOffset_Z;
		FinalMotionWarpingLocation = FinalLocation_X + FinalLocation_Y + FinalLocation_Z;
		FinalMotionWarpingRotation = (-TargetForwardVector).Rotation();
		bCanMotionWarping = true;
	}
	else
	{
		bCanMotionWarping = false;
	}
}

void UAction::EndParry()
{
	if (ControlParryCD.Execute()&&IsValid(OwnerInfoData))
	{
		bIsInParryState = false;
		bIsInParryCD = true;
		GetWorld()->GetTimerManager().SetTimer(
			ParryCDTimeHandle,
			FTimerDelegate::CreateWeakLambda(this, [this]() {
			bIsInParryCD = false;
			ControlParryCD.Reset();
		}),
			OwnerInfoData->ParryCD,
			false
		);
	}
}

void UAction::SurroundingAIPauseAttack(float Raduis, TSubclassOf<AActor> ActorClass)
{
	TArray<AActor*> RawActors = GetTargetsInRange(Raduis, ActorClass);
	PausedAIActors.Empty();
	for (AActor* Actor : RawActors)
	{
		if (IsValid(Actor))
		{
			SetActorAIAttackState(Actor, false);
			PausedAIActors.Add(Actor);
		}
	}
}

void UAction::SurroundingAIResumeAttack()
{
	for (TWeakObjectPtr<AActor> WeakActor : PausedAIActors)
	{
		if (WeakActor.IsValid())
		{
			SetActorAIAttackState(WeakActor.Get(), true);
		}
	}
	PausedAIActors.Empty();
}
void UAction::SetActorAIAttackState(AActor* Actor, bool AttackState)
{
	APawn* Pawn = Cast<APawn>(Actor);
	if (AAIController* AIController = Pawn ? Cast<AAIController>(Pawn->GetController()) : nullptr)
	{
		UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent();
		if (IsValid(BlackboardComponent))
		{
			BlackboardComponent->SetValueAsBool(FName(TEXT("bStopAttack")), AttackState);
		}
	}
}

void UAction::UpdateDodgeLocationAndRotation()
{
	if (!CharacterRef.IsValid())return;
	UCharacterMovementComponent*MovementComponent=CharacterRef->GetCharacterMovement();
	if (!IsValid(MovementComponent))return;
	if (CurrentActionInfo.MotionWarping.MotionWarpingType == EWhichMotionWarping::EWMW_Roll)
	{
		FVector CurrentAcceleration = MovementComponent->GetCurrentAcceleration().GetSafeNormal();
		float CapsuleRadius =IsValid(CharacterRef->GetCapsuleComponent())? CharacterRef->GetCapsuleComponent()->GetUnscaledCapsuleRadius():0.f;
		if (CurrentAcceleration.IsZero())
		{
			FinalDodgeMotionWarpingLocation=CharacterRef->GetActorLocation()+MovementComponent->GetForwardVector()* (CurrentActionInfo.MotionWarping.WarpOffset_X+ CapsuleRadius);
			FinalDodgeMotionWarpingRotation = CharacterRef->GetActorRotation();
		}
		else
		{
			FinalDodgeMotionWarpingLocation = CharacterRef->GetActorLocation() +CurrentAcceleration * (CurrentActionInfo.MotionWarping.WarpOffset_X + CapsuleRadius);
			FinalDodgeMotionWarpingRotation = UKismetMathLibrary::MakeRotFromX(CurrentAcceleration);//CurrentAcceleration.Rotation();
			if (CurrentAcceleration.Y > 0)
			{
				SetDodgeType(EDodgeType::EDT_Left);
			}
			else if (CurrentAcceleration.Y < 0)
			{
				SetDodgeType(EDodgeType::EDT_Right);
			}
		}
	}
	if (CurrentActionInfo.MotionWarping.MotionWarpingType == EWhichMotionWarping::EWMW_Dodge)
	{
		FVector InputVector = CharacterRef->GetLastMovementInputVector().GetSafeNormal();
		float CapsuleRadius = IsValid(CharacterRef->GetCapsuleComponent()) ? CharacterRef->GetCapsuleComponent()->GetUnscaledCapsuleRadius() : 0.f;
		if (InputVector.IsZero())
		{
			FinalDodgeMotionWarpingLocation = CharacterRef->GetActorLocation()-MovementComponent->GetForwardVector() * (CurrentActionInfo.MotionWarping.WarpOffset_X + CapsuleRadius);
			FinalDodgeMotionWarpingRotation = CharacterRef->GetActorRotation();
		}
		else
		{
			FinalDodgeMotionWarpingLocation = CharacterRef->GetActorLocation() + InputVector * (CurrentActionInfo.MotionWarping.WarpOffset_X + CapsuleRadius);
			FinalDodgeMotionWarpingRotation = UKismetMathLibrary::MakeRotFromX(InputVector);
		}
	}
}

void UAction::MotionWarpingOnGroundDodge()
{
	if (FinalDodgeMotionWarpingLocation.IsNearlyZero())return;
	if (CurrentActionInfo.MotionWarping.WarpWhat == EWarpWhat::EWW_LocationOnly)
	{
		OwnerRef->SetActorLocation(FMath::VInterpTo(OwnerRef->GetActorLocation(), FinalDodgeMotionWarpingLocation, CurrentDeltaTime, CurrentActionInfo.MotionWarping.LocationInterpSpeed));
	}
	else if (CurrentActionInfo.MotionWarping.WarpWhat == EWarpWhat::EWW_RotationOnly)
	{
		OwnerRef->SetActorRotation(FRotator(0, FMath::RInterpTo(OwnerRef->GetActorRotation(), FinalDodgeMotionWarpingRotation, CurrentDeltaTime, CurrentActionInfo.MotionWarping.RotationInterpSpeed).Yaw, 0));
	}
	else if (CurrentActionInfo.MotionWarping.WarpWhat == EWarpWhat::EWW_Both)
	{
		OwnerRef->SetActorLocation(FMath::VInterpTo(OwnerRef->GetActorLocation(), FinalDodgeMotionWarpingLocation, CurrentDeltaTime, CurrentActionInfo.MotionWarping.LocationInterpSpeed));
		OwnerRef->SetActorRotation(FRotator(0, FMath::RInterpTo(OwnerRef->GetActorRotation(), FinalDodgeMotionWarpingRotation, CurrentDeltaTime, CurrentActionInfo.MotionWarping.RotationInterpSpeed).Yaw, 0));
	}
	else
	{
		OwnerRef->SetActorLocation(FMath::VInterpTo(OwnerRef->GetActorLocation(), FinalDodgeMotionWarpingLocation, CurrentDeltaTime, CurrentActionInfo.MotionWarping.LocationInterpSpeed));
		if (bCanRotate)
		{
			OwnerRef->SetActorRotation(FRotator(0, FinalDodgeMotionWarpingRotation.Yaw, 0));
			bCanRotate = false;
		}
	}
}

void UAction::ChooseDodge()
{
	if (!CharacterRef.IsValid()|| !Cast<ICharacterInterface>(CharacterRef))return;
	UCharacterMovementComponent* MovementComponent = CharacterRef->GetCharacterMovement();
	if (!IsValid(MovementComponent))return;
	FVector InputVector =MovementComponent->GetLastInputVector().GetSafeNormal();
	FGameplayTag DodgeTag = FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Combat.Dodge.B")));
	if(!InputVector.IsZero())
	{
		FVector NormalizedInput = InputVector.GetSafeNormal();
		FVector LocalInputVector =CharacterRef->GetActorTransform().InverseTransformVectorNoScale(NormalizedInput);
		float Angle = LocalInputVector.Rotation().Yaw;
		if (Angle >= -135 && Angle < -45)
		{
			DodgeTag = FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Combat.Dodge.L")));
			SetDodgeType(EDodgeType::EDT_Left);
		}
		else if (Angle >= -45 && Angle <= 45)
		{
			DodgeTag = FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Combat.Dodge.F")));
		}

		else if (Angle > 45 && Angle <= 135)
		{
			DodgeTag = FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Combat.Dodge.R")));
			SetDodgeType(EDodgeType::EDT_Right);
		}
		else
		{
			DodgeTag = FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Combat.Dodge.B")));
		}
	}
	SetCurrentActionInfoByTag(DodgeTag);
}

void UAction::SetDodgeType(EDodgeType DodgeType)
{
	if (!CharacterRef.IsValid() || !Cast<ICharacterInterface>(CharacterRef))return;
	ICharacterInterface::Execute_SetDodgeType(OwnerRef,DodgeType);
}

void UAction::HandleMeleeHit(UAction *AttackerAction)
{
	if (!IsValid(AttackerAction))return;
	if (AttackerAction->CurrentActionInfo.HitReaction.MeleeHit == FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Hit.Melee.F_Left"))))
	{
		if (EquipmentComponent->CurrentMainWeaponAnimType == EWeaponType::EWT_LongSword)
		{
			int32 RandIndex = FMath::RandRange(2, 3);
			GetAndSetHitInfoByTag(MeleeHitReactions.IsValidIndex(RandIndex) ? MeleeHitReactions[RandIndex] : FGameplayTag());
		}
	}//向左受击
	else if (AttackerAction->CurrentActionInfo.HitReaction.MeleeHit == FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Hit.Melee.F_Right"))))
	{
		if (EquipmentComponent->CurrentMainWeaponAnimType == EWeaponType::EWT_LongSword)
		{
			int32 RandIndex = FMath::RandRange(4, 5);
			GetAndSetHitInfoByTag(MeleeHitReactions.IsValidIndex(RandIndex) ? MeleeHitReactions[RandIndex] : FGameplayTag());
		}
	}//向右受击
	else if (AttackerAction->CurrentActionInfo.HitReaction.MeleeHit == FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Hit.Melee.Random"))))
	{
		if (EquipmentComponent->CurrentMainWeaponAnimType == EWeaponType::EWT_LongSword)
		{
			int32 RandIndex = FMath::RandRange(0, 5);
			GetAndSetHitInfoByTag(MeleeHitReactions.IsValidIndex(RandIndex) ? MeleeHitReactions[RandIndex] : FGameplayTag());
		}
	}//随机
	else
	{
		if (EquipmentComponent->CurrentMainWeaponAnimType == EWeaponType::EWT_LongSword)
		{
			GetAndSetHitInfoByTag(AttackerAction->CurrentActionInfo.HitReaction.MeleeHit);
		}
	}//默认
}
