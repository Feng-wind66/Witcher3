// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacter.h"
#include "Data/CharacterInfoData.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/Component/Action.h"
#include "UI/Scene/BaseHPBar.h"
#include "GAS/ASC.h"
#include "Animation/ANS/DisableMovementInput.h"
#include"GAS/AS.h"
#include "Core/GameMode_1.h"
#include"Animation/AnimNotifies/AnimNotifyState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h" 
#include "Blueprint/Component/Equipment.h"
#include "Blueprint/Component/Inventory.h"
#include "MotionWarpingComponent.h"
ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	InitBaseMesh();
	InitCustomComponent();
}

FORCEINLINE bool ABaseCharacter::IsCombatTargettable_Implementation()
{
	return bIsCombatTargettable;
}

FORCEINLINE void ABaseCharacter::DisableCollisionToPawn_Implementation()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

FORCEINLINE EOwnerType ABaseCharacter::GetOwnerType_Implementation()
{
	return OwnerType;
}

FORCEINLINE bool ABaseCharacter::NotHasAnimNotify_Implementation(TSubclassOf<UAnimNotifyState> NotifyState, bool Not)
{
	return ANSBool(NotifyState,Not);
}

FORCEINLINE UEquipment* ABaseCharacter::GetEquipmentComponent_Implementation()
{
	return Equipment;
}

FORCEINLINE UAction* ABaseCharacter::GetActionComponent_Implementation()
{
	return Action;
}

FORCEINLINE UInventory* ABaseCharacter::GetInventoryComponent_Implementation()
{
	return Inventory;
}

USkeletalMeshComponent* ABaseCharacter::GetSkeletalMeshByType_Implementation(EItemType ItemType)
{
	switch (ItemType)
	{
	case EItemType::EIT_Helmet:return Helmet;
		break;
	case EItemType::EIT_Clothes:return ChestArmor;
		break;
	case EItemType::EIT_Gloves:return ArmArmor;
		break;
	case EItemType::EIT_Pants:return BottomArmor;
		break;
	default:return nullptr;
		break;
	}
}

FORCEINLINE bool ABaseCharacter::IsCombatDamageble_Implementation()
{
	return bIsCanDamageble;
}

void ABaseCharacter::ToggleHPBarVisibility_Implementation(bool Visbility)
{
	if (IsValid(HPBar))
	{
		HPBar->SetVisibility(Visbility);
	}
}

void ABaseCharacter::SetCombatTargettable_Implementation(bool Targettable)
{
	bIsCombatTargettable = Targettable;
}

void ABaseCharacter::SetDodgeType_Implementation(EDodgeType DodgeType)
{
	CurrentDodgeType = DodgeType;
}

void ABaseCharacter::SetCurrentLocomotionState_Implementation(ELocomotionState LocomotionType)
{
	SetLocomotionStyle(LocomotionType);
}

void ABaseCharacter::ApplyEnergyCostAPI_Implementation(float EnergyCost)
{
	ApplyEnergyCost(EnergyCost);
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (GetWorld())
	{
		GameMode = Cast<AGameMode_1>(GetWorld()->GetAuthGameMode());
	}
	if (CharacterInfoData)
	{
		InitCharacterBaseInfo(CharacterInfoData);
	}
	SetLocomotionStyle(ELocomotionState::ELS_Walk);
	if (Action && CharacterInfoData)
	{
		Action->InitActorComponent(CharacterInfoData);
	}
	if (IsValid(HPBar))
	{
		DELAY(GetWorld()->GetTimerManager(), this, InitHPBarTimerHandle, 0.1, &ABaseCharacter::InitHPBar);
	}
	DefaultMeshRelativeZ = GetMesh()->GetRelativeLocation().Z;
	DefaultCapsuleHalfHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	DefaultStepHeight = GetCharacterMovement()->MaxStepHeight;
}

void ABaseCharacter::InitBaseMesh_Implementation()
{
	Helmet = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Helmet"));
	ChestArmor = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ChestArmor"));
	ArmArmor = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmArmor"));
	BottomArmor = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BottomArmor"));
	BottomArmor->SetupAttachment(GetMesh());
	ArmArmor->SetupAttachment(GetMesh());
	ChestArmor->SetupAttachment(GetMesh());
	Helmet->SetupAttachment(GetMesh());
}

void ABaseCharacter::InitCharacterBaseInfo_Implementation(UCharacterInfoData*Data)
{
	DefaultMaxWalkSpeed=Data->MaxWalkSpeed;
	DefaultMaxRunSpeed = Data->MaxRunSpeed;
	DefaultMaxSprintSpeed = Data->MaxSprintSpeed;
}

void ABaseCharacter::SetLocomotionStyle_Implementation(ELocomotionState LocomotionStyle)
{	
	switch (LocomotionStyle)
	{
	case ELocomotionState::ELS_Walk:GetCharacterMovement()->MaxWalkSpeed = ASC->GetNumericAttribute(UAS::GetMoveSpeedAttribute()) * DefaultMaxWalkSpeed;
		break;
	case ELocomotionState::ELS_Run:GetCharacterMovement()->MaxWalkSpeed = ASC->GetNumericAttribute(UAS::GetMoveSpeedAttribute()) * DefaultMaxRunSpeed;
		break;
	case ELocomotionState::ELS_Sprint:GetCharacterMovement()->MaxWalkSpeed = ASC->GetNumericAttribute(UAS::GetMoveSpeedAttribute()) * DefaultMaxSprintSpeed;
		break;
	default:return;
		break;
	}
	LastLocomotionState = LocomotionState;
	LocomotionState = LocomotionStyle;
	CurrentMaxWalkSpeed = ASC->GetNumericAttribute(UAS::GetMoveSpeedAttribute()) * DefaultMaxWalkSpeed;//动画蓝图中使用
}
void ABaseCharacter::InitCustomComponent_Implementation()
{
	ASC = CreateDefaultSubobject<UASC>(TEXT("ASC"));
	Action = CreateDefaultSubobject<UAction>(TEXT("Action"));
	MotionWarping = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarping"));
	Equipment = CreateDefaultSubobject<UEquipment>(TEXT("Equipment"));
	Inventory = CreateDefaultSubobject<UInventory>(TEXT("Inventory"));
	HPBar= CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBar"));
	HPBar->SetupAttachment(GetRootComponent());
	HPBar->SetVisibility(false);
	HPBar->SetDrawAtDesiredSize(true);
	HPBar->SetWidgetSpace(EWidgetSpace::Screen);
}
bool ABaseCharacter::ANSBool_Implementation(TSubclassOf<UAnimNotifyState>NotifyState, bool Not)
{
	bool bAnimInAnyState=false;
	if(NotifyState&&GetMesh()&& GetMesh()->GetAnimInstance())
		bAnimInAnyState=GetMesh()->GetAnimInstance()->WasAnimNotifyStateActiveInAnyState(NotifyState);
	return Not ? !bAnimInAnyState : bAnimInAnyState;
}
bool ABaseCharacter::IsNormalState_Implementation()
{
	if (GameMode)
		switch (ICharacterInterface::Execute_GetCurrentActionState(GameMode))
		{
		case EActionState::EAS_OutOfCombat:return true;
		case EActionState::EAS_InCombat:return true;
		case EActionState::EAS_InAir:return true;
		case EActionState::EAS_CombatFinisher_Attacker:return false;
		case EActionState::EAS_CombatFinisher_Receiver:return false;
		case EActionState::EAS_InUIInteraction_Dialogue:return true;
		case EActionState::EAS_Dialogue:return false;
		case EActionState::EAS_Death:return false;
		case EActionState::EAS_InUIMenu:return false;
		case EActionState::EAS_InUIInteraction_Loot:return true;
		case EActionState::EAS_InUIInteraction_MountHorse:return true;
		case EActionState::EAS_Rading:return false;
		default:return false;
		}
	return false;
}//先把IsNormalState留在基类我觉得除了玩家用不到这个
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseCharacter::InitHPBar()
{
	if (UBaseHPBar* HPBarRef = Cast<UBaseHPBar>(HPBar->GetWidget()))
	{
		HPBarRef->InitHPBar(this);
	}
}

void ABaseCharacter::HandleDeath(AActor* Killer)
{
	if (ActorHasTag(FName(TEXT("Death")))) return;
	KillerOwnerRef = Killer;
	if (IsValid(Action))
	{
		if (ExcuteDeathOnce.Execute())
		{
			Action->ExecuteDeath(Killer);
			bIsCombatTargettable = false;
			ITargettingInterface::Execute_ToggleHPBarVisibility(this, false);
			ICharacterInterface::Execute_DisableCollisionToPawn(this);
		}
	}
}

void ABaseCharacter::PreDismemberment(FName BoneName)
{
	if (!KillerOwnerRef.IsValid() || !IsValid(GetMesh()))return;
	Dismemberment(BoneName,(GetMesh()->GetBoneLocation(BoneName)- KillerOwnerRef->GetActorLocation()).GetSafeNormal());
}

void ABaseCharacter::Rolling(float Alpha)
{
	if (!IsValid(GetCapsuleComponent()) || !IsValid(GetMesh()))return;
	float TargetHalfHeight = FMath::Lerp(DefaultCapsuleHalfHeight, RollingCapsuleHalfHeight,FMath::InterpEaseInOut(0.0f, 1.0f, Alpha, 2.0f));
	float Radius = GetCapsuleComponent()->GetUnscaledCapsuleRadius();
	TargetHalfHeight = FMath::Max(TargetHalfHeight, Radius);
	GetCapsuleComponent()->SetCapsuleSize(Radius, TargetHalfHeight);
	float HeightDiff = DefaultCapsuleHalfHeight - TargetHalfHeight;
	float NewMeshZ = DefaultMeshRelativeZ + HeightDiff;
	FVector CurrentLoc = GetMesh()->GetRelativeLocation();
	GetMesh()->SetRelativeLocation(FVector(CurrentLoc.X, CurrentLoc.Y, NewMeshZ));
	if (Alpha > 0.1f)
	{
		GetCharacterMovement()->MaxStepHeight = RollingStepHeight;
	}
	else
	{
		GetCharacterMovement()->MaxStepHeight =DefaultStepHeight;
	}
}

void ABaseCharacter::EndRoll()
{
	if (!IsValid(GetCapsuleComponent()) || !IsValid(GetMesh()))return;
	float Radius = GetCapsuleComponent()->GetUnscaledCapsuleRadius();
	GetCapsuleComponent()->SetCapsuleSize(Radius, DefaultCapsuleHalfHeight);
	FVector CurrentLoc = GetMesh()->GetRelativeLocation();
	GetMesh()->SetRelativeLocation(FVector(CurrentLoc.X, CurrentLoc.Y, DefaultMeshRelativeZ));
	GetCharacterMovement()->MaxStepHeight = DefaultStepHeight;
	//没有考虑头顶有东西
}

void ABaseCharacter::SetCurrentMagicType(EMagicType MagicType)
{
	if (!IsValid(Action))return;
	Action->CurrentMagicType = MagicType;
}

float ABaseCharacter::GetMagicBaseCost(EMagicType MagicType)
{
	switch (MagicType)
	{
	case EMagicType::EMT_Aard:return 30;
		break;
	case EMagicType::EMT_Axii:return 60;
		break;
	case EMagicType::EMT_Igni:return 80;
		break;
	case EMagicType::EMT_Quen:return 0;
		break;
	case EMagicType::EMT_Yrden:return 40;
		break;
	}
	return 0.0f;
}

void ABaseCharacter::ApplyEnergyCost(float EnergyCost)
{
	if (AllGE && AllGE->GE_EnergyCost && IsValid(ASC))
	{
		FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
		FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(AllGE->GE_EnergyCost, 1.0f, Context);
		if (SpecHandle.Data.IsValid())
		{
			FGameplayTag CostTag = FGameplayTag::RequestGameplayTag(FName("Data.Cost.Energy"));
			SpecHandle.Data->SetSetByCallerMagnitude(CostTag, -EnergyCost);
			ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
		}
	}
}
