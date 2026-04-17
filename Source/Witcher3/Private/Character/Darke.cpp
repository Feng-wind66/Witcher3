// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Darke.h"
#include "UI/Menu/SkillWheel/SkillWheel.h"
#include "FunctionLibrary/CommonFuction.h" 
#include "Components/CapsuleComponent.h" 
#include "UI/Component/MainUIComponent.h"
#include "UI/Main/MainUI.h"
#include "Blueprint/Component/Equipment.h"
#include "Blueprint/Component/Action.h"
#include "Blueprint/Component/Interact.h"
#include "GAS/ASC.h"
#include "Animation/ANS/DisableLockOnRotationAll.h"
#include "GameFramework/SpringArmComponent.h" 
#include "Camera/CameraComponent.h"      
#include "Core/GameMode_1.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h" 
#include "Blueprint/Component/Inventory.h"
#include "Animation/ANS/DisableMovementInput.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include"GAS/AS.h"
#include "Kismet/KismetMathLibrary.h"

ADarke::ADarke()
{
	Init();
}

void ADarke::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CurrentDeltaTime = DeltaTime;
	if (bLockOn&&IsValid(Action) && Action->CurrentLockTarget.IsValid())
	{
		LockCameraSystem();
	}
	else if(!bLockOn&&!bRestoreCameraFinish)
	{
		RestoreCameraSet();
	}
}

UStaticMeshComponent* ADarke::GetStaticMeshByType_Implementation(EDamageSource WeaponType)
{
	switch (WeaponType)
	{
	case EDamageSource::EDS_PrimaryWeapon:return FirstWeaponSheathTarget;
		break;
	case EDamageSource::EDS_SecondaryWeapon:return SecondWeaponSheathTarget;
		break;
	default:return nullptr;
		break;
	}
}

bool ADarke::IsCombatTargettable_Implementation()
{
	return bIsCombatTargettable;
}

void ADarke::Rolling(float Alpha)
{
	if (!IsValid(GetCapsuleComponent()) || !IsValid(GetMesh()))return;
	float TargetHalfHeight = FMath::Lerp(DefaultCapsuleHalfHeight, RollingCapsuleHalfHeight, FMath::InterpEaseInOut(0.0f, 1.0f, Alpha, 2.0f));
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
		GetCharacterMovement()->MaxStepHeight = DefaultStepHeight;
	}
	if (IsValid(SpringArm))
	{
		float NewArmZ = DefaultSpringArmZ + HeightDiff;
		FVector CurrentOffset = SpringArm->SocketOffset;
		SpringArm->SocketOffset = FVector(CurrentOffset.X, CurrentOffset.Y, NewArmZ);
	}
}

void ADarke::EndRoll()
{
	Super::EndRoll();
	if (IsValid(SpringArm))
	{
		FVector CurrentOffset = SpringArm->SocketOffset;
		SpringArm->SocketOffset = FVector(CurrentOffset.X, CurrentOffset.Y, DefaultSpringArmZ);
	}
}

void ADarke::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(GameMode))
	{
		GameMode->SetPlayerRef(this);
	}
	if (IsValid(SpringArm))
	{
		DefaultSpringArmZ = SpringArm->SocketOffset.Z;
	}
	if (IsValid(GetMesh()))
	{
		GetMesh()->SetTickGroup(ETickingGroup::TG_PostPhysics);
	}
	WeakMainUIComponent = UCommonFuction::GetMainUIComponent(this);
}

void ADarke::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);
	if (PrevMovementMode == EMovementMode::MOVE_Walking && GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Falling)
	{
		StartFallingLocation = GetActorLocation();
		EActionState CurrentActionState = Action->GetCurrentActionState();
		if (CurrentActionState == EActionState::EAS_OutOfCombat || CurrentActionState == EActionState::EAS_InCombat || CurrentActionState == EActionState::EAS_InUIInteraction_Loot || CurrentActionState == EActionState::EAS_InUIInteraction_MountHorse || CurrentActionState == EActionState::EAS_Rading)
		{
			Action->SetLastOnGroundActionState(CurrentActionState);
			Action->SetCurrentActionState(EActionState::EAS_InAir);
		}
	}
}

void ADarke::Landed(const FHitResult& Hit)
{
	bJumpPressed = false;
	IsFallingDeath();
	if (GameMode&&ICharacterInterface::Execute_GetCurrentActionState(GameMode) == EActionState::EAS_InAir)
		Action->SetCurrentActionState(Action->GetLastOnGroundActionState());
}
void ADarke::Init_Implementation()
{
	InitDarkeMesh();
	InitCamera();
}
void ADarke::InitDarkeMesh_Implementation()
{
	Arms = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Arms"));
	Body = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body"));
	Leg = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Leg"));
	Hands = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hands"));
	Hair = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hair"));
	Arms->SetupAttachment(GetMesh());
	Body->SetupAttachment(GetMesh());
	Leg->SetupAttachment(GetMesh());
	Hands->SetupAttachment(GetMesh());
	Hair->SetupAttachment(GetMesh());
	InteractComponent = CreateDefaultSubobject<UInteract>(TEXT("InteractComponent"));
	FirstWeaponStorePoint = CreateDefaultSubobject<USceneComponent>(TEXT("FirstWeaponStorePoint"));
	FirstWeaponStorePoint->SetupAttachment(GetMesh(),FName(TEXT("spine_03")));
	FirstWeaponSheathRoot = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FirstWeaponSheathRoot"));
	FirstWeaponSheathRoot->SetupAttachment(FirstWeaponStorePoint);
	FirstPhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("FirstPhysicsConstraint"));
	FirstPhysicsConstraint->SetupAttachment(FirstWeaponSheathRoot);
	FirstWeaponSheathTarget = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FirstWeaponSheathTarget"));
	FirstWeaponSheathTarget->SetupAttachment(FirstWeaponStorePoint);//
	SecondWeaponStorePoint = CreateDefaultSubobject<USceneComponent>(TEXT("SecondWeaponStorePoint"));
	SecondWeaponStorePoint->SetupAttachment(GetMesh(), FName(TEXT("spine_03")));
	SecondWeaponSheathRoot = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SecondWeaponSheathRoot"));
	SecondWeaponSheathRoot->SetupAttachment(SecondWeaponStorePoint);
	SecondPhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("SecondPhysicsConstraint"));
	SecondPhysicsConstraint->SetupAttachment(SecondWeaponSheathRoot);
	SecondWeaponSheathTarget = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SecondWeaponSheathTarget"));
	SecondWeaponSheathTarget->SetupAttachment(SecondWeaponStorePoint);
}
void ADarke::InitCamera_Implementation()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 170.f;
	SpringArm->bUsePawnControlRotation = true;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}
void ADarke::OnControllerChanged_Implementation(AController* NewController)
{
	if (APlayerController* PlayerController = Cast<APlayerController>(NewController))
		if (EnhanceInputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			EnhanceInputSystem->ClearAllMappings();
			EnhanceInputSystem->AddMappingContext(InputMappingContext, 0);
			//少了RemoveHouseMapping
			bJumpPressed = false;
		}
}
void ADarke::MoveForward_Implementation(const FInputActionValue& Value)
{
	MoveForwardValue = Value.Get<float>();
	if (IsNormalState() && ANSBool(UDisableMovementInput::StaticClass(), true))
		AddMovementInput(FRotationMatrix(FRotator(0, GetControlRotation().Yaw, 0)).GetUnitAxis(EAxis::X), MoveForwardValue);
}
void ADarke::MoveRight_Implementation(const FInputActionValue& Value)
{
	MoveForwardValue = Value.Get<float>();
	if (IsNormalState() && ANSBool(UDisableMovementInput::StaticClass(), true))
		AddMovementInput(FRotationMatrix(FRotator(0, GetControlRotation().Yaw, 0)).GetUnitAxis(EAxis::Y), MoveForwardValue);
}
void ADarke::OnJump_Implementation(const FInputActionValue& Value)
{
	if (!IsValid(GameMode))return;
	EActionState CurrentActionState = ICharacterInterface::Execute_GetCurrentActionState(GameMode);
	if (CurrentActionState == EActionState::EAS_OutOfCombat)
	{
		if (bLockOn)
		{
			//锁定后的逻辑
		}
		else
			if (!bJumpPressed)
			{
				bJumpPressed = true;
			}
	}
	if (CurrentActionState == EActionState::EAS_InCombat)
	{
		if (!IsValid(ASC) || ASC->GetNumericAttribute(UAS::GetEnergyAttribute()) < 30)return;
		FGameplayTag RollTag = FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Combat.Dodge.Roll_F")));
		if (IsValid(Action))
		{
			Action->TriggerAction(RollTag, EPriority::EP_VeryHigh, true);
		}
	}
}
void ADarke::Look_Implementation(const FInputActionValue& Value)
{
	if (IsNormalState() && !bLockOn)
	{
		AddControllerPitchInput(-Value.Get<FVector2D>().Y);
		AddControllerYawInput(Value.Get<FVector2D>().X);
	}
}
void ADarke::SingleTapLeftThumbKey_Implementation(const FInputActionValue& Value)
{

}
void ADarke::ToggleMoveStyle_Implementation(const FInputActionValue& Value)
{
	bMoveStyleFlip = bMoveStyleFlip ? false : true;
	bMoveStyleFlip ? SetLocomotionStyle(ELocomotionState::ELS_Run) : SetLocomotionStyle(ELocomotionState::ELS_Walk);
}
void ADarke::Dodge_Implementation(const FInputActionValue& Value)
{
	if (!IsValid(GameMode)||!IsValid(Action)||!IsValid(ASC) || ASC->GetNumericAttribute(UAS::GetEnergyAttribute()) < 20)return;
	UCharacterMovementComponent*MovementComponent=GetCharacterMovement();
	if (!IsValid(MovementComponent))return;
	EActionState CurrentActionState = ICharacterInterface::Execute_GetCurrentActionState(GameMode);
	if (CurrentActionState == EActionState::EAS_InCombat || CurrentActionState == EActionState::EAS_OutOfCombat)
	{
		Action->TriggerAction(FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Combat.Dodge"))), EPriority::EP_High, true);
	}
}
void ADarke::ChangeWeapon_Implementation(const FInputActionValue& Value)
{
	if (Value.Get<float>() > 0)
	{
		if (Equipment && !Equipment->CurrentMainWeaponRef)
		{
			DrawSecondWeapon();
		}
		else if (Equipment &&Equipment->CurrentMainWeaponRef)
		{
			switch (Equipment->CurrentMainWeaponType)
			{
			case EDamageSource::EDS_PrimaryWeapon:DrawSecondWeapon();
				break;
			case EDamageSource::EDS_SecondaryWeapon:DrawFirstWeapon();
				break;
			}
		}
	}
	else
	{
		if (Equipment && !Equipment->CurrentMainWeaponRef)
		{
			DrawFirstWeapon();
		}
		else if (Equipment && Equipment->CurrentMainWeaponRef)
		{
			switch (Equipment->CurrentMainWeaponType)
			{
			case EDamageSource::EDS_PrimaryWeapon:DrawSecondWeapon();
				break;
			case EDamageSource::EDS_SecondaryWeapon:DrawFirstWeapon();
				break;
			}
		}
	}
}
void ADarke::LightAttack_Implementation(const FInputActionValue& Value)
{
	if (!IsValid(ASC) || ASC->GetNumericAttribute(UAS::GetEnergyAttribute()) < 20)return;
	EActionState CurrentActionState = ICharacterInterface::Execute_GetCurrentActionState(GameMode);
	if (Action&&CurrentActionState==EActionState::EAS_InCombat)
	{
		Action->TriggerAction(FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Combat.Combo.LightAttack"))), EPriority::EP_Medium, true);
	}
}
void ADarke::SummonHorse_Implementation(const FInputActionValue& Value)
{

}
void ADarke::ToggleLockOnTarget_Implementation(const FInputActionValue& Value)
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(LockTargetTimerHandle);
		DELAY(GetWorld()->GetTimerManager(), this, LockTargetTimerHandle,0.2,&ADarke::LockTarget);
	}
}
void ADarke::ToggleParry()
{
	if (!IsValid(Action)|| !IsValid(GetMesh()))return;
	EActionState CurrentActionState=ICharacterInterface::Execute_GetCurrentActionState(GameMode);
	if (CurrentActionState == EActionState::EAS_InCombat)
	{
		Action->bIsInParryState = true;
		ICharacterInterface::Execute_SendParryPress(GetMesh()->GetAnimInstance(),true);
		//原版Darke是有bisparry变量的
	}
}
void ADarke::EndParry()
{
	if (!IsValid(Action) || !IsValid(GetMesh()))return;
	Action->bIsInParryState = false;
	ICharacterInterface::Execute_SendParryPress(GetMesh()->GetAnimInstance(), false);
}
void ADarke::Interactive_Implementation(const FInputActionValue& Value)
{
	if (!IsValid(GameMode) || !IsValid(InteractComponent)||!InteractComponent->WeakCurrentInteractActor.IsValid())return;
	EActionState CurrentActionState = ICharacterInterface::Execute_GetCurrentActionState(GameMode);
	if (!(CurrentActionState == EActionState::EAS_OutOfCombat || CurrentActionState == EActionState::EAS_InCombat))return;
	if (IInteractableInterface::Execute_CanBeInteracted(InteractComponent->WeakCurrentInteractActor.Get(), this))
	{	
		IInteractableInterface::Execute_Interact(InteractComponent->WeakCurrentInteractActor.Get(), this);
		InteractComponent->bIsInteracting = true;
	}
}
void ADarke::Back_Implementation(const FInputActionValue& Value)
{

}
void ADarke::Comsumables_1_Implementation(const FInputActionValue& Value)
{

}
void ADarke::Comsumables_2_Implementation(const FInputActionValue& Value)
{

}
void ADarke::Comsumables_3_Implementation(const FInputActionValue& Value)
{

}
void ADarke::Comsumables_4_Implementation(const FInputActionValue& Value)
{

}
void ADarke::InventoryPickUp_Implementation(const FInputActionValue& Value)
{

}
void ADarke::InventoryPickUpAll_Implementation(const FInputActionValue& Value)
{

}
void ADarke::OpenBigMap_Implementation(const FInputActionValue& Value)
{

}
void ADarke::ToggleInventoryUI_Implementation(const FInputActionValue& Value)
{
	if (CanOpenInventory())
	{
		OpenMenuEvent.Broadcast(EMenuType::EMT_Inventory);
	}
}
bool ADarke::CanOpenInventory_Implementation()
{
	if (GameMode)
		switch (ICharacterInterface::Execute_GetCurrentActionState(GameMode))
		{
		case EActionState::EAS_OutOfCombat:return true;
			break;
		case EActionState::EAS_InCombat:return true;
			break;
		case EActionState::EAS_InAir:return true;
			break;
		case EActionState::EAS_CombatFinisher_Attacker:return true;
			break;
		case EActionState::EAS_CombatFinisher_Receiver:return true;
			break;
		case EActionState::EAS_InUIInteraction_Dialogue:return false;
			break;
		case EActionState::EAS_Dialogue:return false;
			break;
		case EActionState::EAS_Death:return false;
			break;
		case EActionState::EAS_InUIMenu:return true;
			break;
		case EActionState::EAS_InUIInteraction_Loot:return false;
			break;
		case EActionState::EAS_InUIInteraction_MountHorse:return false;
			break;
		case EActionState::EAS_Rading:return false;
			break;
		default:return false;
			break;
		}
	else
		return false;
}

void ADarke::IsFallingDeath_Implementation()
{
	if (StartFallingLocation.Z - GetActorLocation().Z >= MaxFallingHigh && GetMesh())
	{
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetMesh()->SetSimulatePhysics(true);
		GetCharacterMovement()->DisableMovement();
	}
}

void ADarke::DrawFirstWeapon_Implementation()
{
	if (Equipment && !Equipment->CurrentFirstWeaponActorRef)
	{
		return;
	}
	EActionState ActionState = ICharacterInterface::Execute_GetCurrentActionState(GameMode);
	if (ActionState == EActionState::EAS_OutOfCombat||ActionState==EActionState::EAS_InCombat)
	{
		if (Equipment && Equipment->CurrentMainWeaponRef && Action)
		{
			switch (Equipment->CurrentMainWeaponType)
			{
			case EDamageSource::EDS_PrimaryWeapon:Action->TriggerAction(FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Combat.Misc.SheathPrimaryWeapon"))), EPriority::EP_Medium,true);
				break;
			case EDamageSource::EDS_SecondaryWeapon:Action->TriggerAction(FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Combat.Misc.SwitchStoPweapon"))), EPriority::EP_Medium,true);
				break;
			}
		}
		else if (Equipment && !Equipment->CurrentMainWeaponRef && Action)
		{
			Action->TriggerAction(FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Combat.Misc.DrawPrimaryWeapon"))), EPriority::EP_Medium, true);
		}
	}
}

void ADarke::DrawSecondWeapon_Implementation()
{
	if (Equipment && !Equipment->CurrentSecondWeaponActorRef)
	{
		return;
	}
	EActionState ActionState = ICharacterInterface::Execute_GetCurrentActionState(GameMode);
	if (ActionState == EActionState::EAS_OutOfCombat || ActionState == EActionState::EAS_InCombat)
	{
		if (Equipment && Equipment->CurrentMainWeaponRef && Action)
		{
			switch (Equipment->CurrentMainWeaponType)
			{
			case EDamageSource::EDS_PrimaryWeapon:Action->TriggerAction(FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Combat.Misc.SwitchPtoSweapon"))), EPriority::EP_Medium,true);
				break;
			case EDamageSource::EDS_SecondaryWeapon:Action->TriggerAction(FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Combat.Misc.SheathSecondaryWeapon"))), EPriority::EP_Medium,true);
				break;
			}
		}
		else if (Equipment && !Equipment->CurrentMainWeaponRef && Action)
		{
			Action->TriggerAction(FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Combat.Misc.DrawSecondaryWeapon"))), EPriority::EP_Medium,true);
		}
	}
}

void ADarke::LockCameraSystem_Implementation()
{
	if (ANSBool(UDisableLockOnRotationAll::StaticClass(),true) && Action && Action->CurrentLockTarget.Get())//放ansbool
	{
		EActionState CurrentActionState = ICharacterInterface::Execute_GetCurrentActionState(GameMode);
		if (CurrentActionState == EActionState::EAS_OutOfCombat || CurrentActionState == EActionState::EAS_CombatFinisher_Attacker)
		{

		}
		else if (CurrentActionState == EActionState::EAS_InCombat)
		{
			if (ANSBool(DisableLockOnRotation,true) && Action && Action->CurrentLockTarget.Get())//放ansbool
			{
				SetActorRotation(FRotator(0,FMath::RInterpTo(GetActorRotation(), UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Action->CurrentLockTarget->GetActorLocation()), CurrentDeltaTime, 5.f).Yaw,0));
			}
		}
		AdjustCameraInLock();
	}
}

void ADarke::LockTarget_Implementation()
{
	EActionState CurrentActionState = ICharacterInterface::Execute_GetCurrentActionState(GameMode);
	if (EnhanceInputSystem->GetPlayerInput()->GetActionValue(IA_ToggleLockOnTarget).Get<bool>())
	{
		if (Action && CurrentActionState == EActionState::EAS_InCombat || CurrentActionState == EActionState::EAS_OutOfCombat)
		{
			bLockOn = false;
			Action->ClearCurrentLockTarget();
			Action->ResetLockTargetVariables();
			SetLocomotionStyle(LastLocomotionState);
		}
	}
	else
	{
		if (CurrentActionState == EActionState::EAS_InCombat || CurrentActionState == EActionState::EAS_OutOfCombat)
		{
			if (Action&&Action->LockTarget(Action->DetectRadius,Action->DetectActorClass, false)&&Action->CurrentLockTarget.IsValid())
			{
				SetLocomotionStyle(ELocomotionState::ELS_Walk);
				bLockOn = true;
				CameraLockSpringArmLength =GetSpringArmLengthByTarget(ICharacterInterface::Execute_GetOwnerType(Action->CurrentLockTarget.Get()));
			}
		}
	}
}

void ADarke::AdjustCameraInLock_Implementation()
{
	FRotator TempRotator = FMath::RInterpTo(Camera->GetComponentRotation(), UKismetMathLibrary::FindLookAtRotation(Camera->GetComponentLocation(), Action->CurrentLockTarget->GetActorLocation()), CurrentDeltaTime, 2.f);
	if (FMath::IsNearlyEqual(SpringArm->TargetArmLength, CameraLockSpringArmLength) && SpringArm->SocketOffset.Equals(GetSpringArmSocketTargetOffsetByDodgeType(CurrentDodgeType)) && Camera->GetComponentRotation().Equals(FRotator(TempRotator.Pitch, TempRotator.Yaw, 0)))
	{
		return;
	}
	if (Controller && Action && Action->CurrentLockTarget.Get())
	{
		bRestoreCameraFinish = false;
		Controller->SetControlRotation(FRotator(0, FMath::RInterpTo(GetControlRotation(), UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Action->CurrentLockTarget->GetActorLocation()), CurrentDeltaTime, 4.f).Yaw, 0));
		SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, CameraLockSpringArmLength, CurrentDeltaTime, 1.f);
		SpringArm->SocketOffset = FMath::VInterpTo(SpringArm->SocketOffset, GetSpringArmSocketTargetOffsetByDodgeType(CurrentDodgeType), CurrentDeltaTime, 3.f);
		if (Action && Action->CurrentLockTarget.Get())
		{
			Camera->SetWorldRotation(FRotator(TempRotator.Pitch, TempRotator.Yaw, 0));
		}
	}
}

float ADarke::GetSpringArmLengthByTarget_Implementation(EOwnerType TargetType)
{
	switch (TargetType)
	{
	case EOwnerType::EOT_Player:return 300;
		break;
	case EOwnerType::EOT_NPC:return 300;
		break;
	case EOwnerType::EOT_Dragon:return 400;
		break;
	default:return 170;
		break;
	}
}

FVector ADarke::GetSpringArmSocketTargetOffsetByDodgeType_Implementation(EDodgeType DodgeType)
{
	switch (DodgeType)
	{
	case EDodgeType::EDT_Left:return FVector(0, 160, 50);
		break;
	case EDodgeType::EDT_Right:return FVector(0, -160, 50);
		break;
	default:return FVector();
		break;
	}
}

void ADarke::RestoreCameraSet_Implementation()
{
	if (FMath::IsNearlyEqual(SpringArm->TargetArmLength, DefaultSpringArmLength) && SpringArm->SocketOffset.Equals(DefaultSpringArmSocketOffset, 1) && Camera->GetRelativeRotation().Equals(FRotator(0), 1))
	{
		bRestoreCameraFinish = true;
		return;
	}
	else
	{
		SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, DefaultSpringArmLength, CurrentDeltaTime, 0.8);
		SpringArm->SocketOffset = FMath::VInterpTo(SpringArm->SocketOffset, DefaultSpringArmSocketOffset, CurrentDeltaTime, 2.f);
		Camera->SetRelativeRotation(FMath::RInterpTo(Camera->GetRelativeRotation(), FRotator(0), CurrentDeltaTime, 2.f));
	}
}

void ADarke::ChooseMagic()
{
	EActionState CurrentActionState = ICharacterInterface::Execute_GetCurrentActionState(GameMode);
	if (CurrentActionState == EActionState::EAS_InCombat || CurrentActionState == EActionState::EAS_OutOfCombat)
	{
		if (USkillWheel* SkillWheel = WeakMainUIComponent.IsValid() ? WeakMainUIComponent->SkillWheelRef : nullptr)
		{
			SkillWheel->OpenWheel();
		}
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetWorld()->GetFirstPlayerController()->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(IMC_RadialMenu, 1);
		}
	}
}

void ADarke::ChooseMagicComplete()
{
	if (USkillWheel* SkillWheel = WeakMainUIComponent.IsValid()? WeakMainUIComponent->SkillWheelRef : nullptr)
	{
		SkillWheel->CloseWheel();
	}
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetWorld()->GetFirstPlayerController()->GetLocalPlayer()))
	{
		Subsystem->RemoveMappingContext(IMC_RadialMenu);
	}
}

void ADarke::HandleRadialInput(const FInputActionValue& Value)
{
	FVector2D CursorDelta = Value.Get<FVector2D>();
	VirtualCursorPos += CursorDelta;
	if (VirtualCursorPos.Size() > 100.0f)
	{
		VirtualCursorPos = VirtualCursorPos.GetSafeNormal();
	}
	float Angle = FMath::RadiansToDegrees(FMath::Atan2(VirtualCursorPos.Y, VirtualCursorPos.X));
	Angle < 0 ? Angle += 360 : Angle;
	OnRadialInputUpdateEvent.Broadcast(VirtualCursorPos,Angle);
}

void ADarke::ReleaseMagic()
{
	if (!IsValid(ASC)||!IsValid(Action)||ASC->GetNumericAttribute(UAS::GetEnergyAttribute())<GetMagicBaseCost(Action->CurrentMagicType))return;
	EActionState CurrentActionState = ICharacterInterface::Execute_GetCurrentActionState(GameMode);
	if (CurrentActionState == EActionState::EAS_InCombat || CurrentActionState == EActionState::EAS_OutOfCombat)
	{
		Action->TriggerAction(FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Combat.Magic.Universal"))), EPriority::EP_High, true);
	}
}

void ADarke::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ADarke::Look);
		EnhancedInputComponent->BindAction(IA_MoveForward, ETriggerEvent::Triggered, this, &ADarke::MoveForward);
		EnhancedInputComponent->BindAction(IA_MoveRight, ETriggerEvent::Triggered, this, &ADarke::MoveRight);
		EnhancedInputComponent->BindAction(IA_MoveForward, ETriggerEvent::Completed, this, &ADarke::MoveForwardComplete);
		EnhancedInputComponent->BindAction(IA_MoveRight, ETriggerEvent::Completed, this, &ADarke::MoveRightComplete);
		EnhancedInputComponent->BindAction(IA_ToggleMoveStyle, ETriggerEvent::Started, this, &ADarke::ToggleMoveStyle);
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &ADarke::OnJump);
		EnhancedInputComponent->BindAction(IA_ToggleInventoryUI, ETriggerEvent::Started, this, &ADarke::ToggleInventoryUI);
		EnhancedInputComponent->BindAction(IA_EquipIronSword, ETriggerEvent::Started, this, &ADarke::DrawFirstWeapon);
		EnhancedInputComponent->BindAction(IA_EquipSilverSword, ETriggerEvent::Started, this, &ADarke::DrawSecondWeapon);
		EnhancedInputComponent->BindAction(IA_ChangeWeapon, ETriggerEvent::Triggered, this, &ADarke::ChangeWeapon);
		EnhancedInputComponent->BindAction(IA_ToggleLockOnTarget, ETriggerEvent::Started, this, &ADarke::ToggleLockOnTarget);
		EnhancedInputComponent->BindAction(IA_LightAttack, ETriggerEvent::Started, this, &ADarke::LightAttack);
		EnhancedInputComponent->BindAction(IA_ToggleParry, ETriggerEvent::Started, this, &ADarke::ToggleParry);
		EnhancedInputComponent->BindAction(IA_ToggleParry, ETriggerEvent::Canceled, this, &ADarke::EndParry);
		EnhancedInputComponent->BindAction(IA_ToggleParry, ETriggerEvent::Completed, this, &ADarke::EndParry);
		EnhancedInputComponent->BindAction(IA_Dodge, ETriggerEvent::Started, this, &ADarke::Dodge);
		EnhancedInputComponent->BindAction(IA_ChooseMagic, ETriggerEvent::Started, this, &ADarke::ChooseMagic);
		EnhancedInputComponent->BindAction(IA_ChooseMagic, ETriggerEvent::Completed, this, &ADarke::ChooseMagicComplete);
		EnhancedInputComponent->BindAction(IA_ReleaseMagic, ETriggerEvent::Started, this, &ADarke::ReleaseMagic);
		EnhancedInputComponent->BindAction(IA_Interactive, ETriggerEvent::Started, this, &ADarke::Interactive);
		/*
		
		*/
		EnhancedInputComponent->BindAction(IA_RadialMenuLook, ETriggerEvent::Triggered, this, &ADarke::HandleRadialInput);
	}
}
