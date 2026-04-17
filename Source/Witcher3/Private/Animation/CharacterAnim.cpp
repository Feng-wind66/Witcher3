
#include "Animation/CharacterAnim.h"
#include "Core/GameMode_1.h"
#include"Macro/Macro.h"
#include "Blueprint/Component/Action.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Blueprint/Component/Equipment.h"
#include "Character/Darke.h"
float SafeDivide(float Numerator, float Denominator, float DefaultValue = 0.0f)
{
	if (FMath::Abs(Denominator) < SMALL_NUMBER)
		return DefaultValue;
	return Numerator / Denominator;
}
void UCharacterAnim::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
	CalculateLeanAngle();
}
void UCharacterAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	this->DeltaTime = DeltaSeconds;
	SetEssentialMovementData();
	BEGAVIOR_MONITORING(bHasMovementInput, &OnTrue, &OnFalse, OnStart, OnEnd, WhileTrue, WhileFalse);
	if (Action)
	{
		bIsInCombat=Action->bIsInCombat;
	}
}
void UCharacterAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	if (UWorld* World = GetWorld())
		DELAY(World->GetTimerManager(), this, IntialDelayHandle,0.001, &UCharacterAnim::Inital);
}

void UCharacterAnim::NativePostEvaluateAnimation()
{
	Super::NativePostEvaluateAnimation();
	ResetTransition();
}

void UCharacterAnim::SendParryPress_Implementation(bool bIsPress)
{
	bParryPress = bIsPress;
}

void UCharacterAnim::Inital_Implementation()
{
	InitReference();
	InitLocomotion();
	OnFalse.Execute();
}

void UCharacterAnim::InitReference_Implementation()
{
	CharacterRef =Cast<ABaseCharacter>(GetOwningActor());
	if(CharacterRef&&CharacterRef->GameMode)
		DarkeRef= Cast<ADarke>(ICharacterInterface::Execute_GetPlayerRef(CharacterRef->GameMode));
	if (CharacterRef)
	{
		CharacterMovement = CharacterRef->GetCharacterMovement();
		Equipment = CharacterRef->Equipment;
		Action = CharacterRef->Action;
		Mesh=CharacterRef->GetMesh();
	}
}

void UCharacterAnim::InitLocomotion_Implementation()
{
	if(CharacterRef)
	CurrentLocomotion = CharacterRef->LocomotionState;
	LastLocomotion = ELocomotionState::ELS_Walk;
}

void UCharacterAnim::SetEssentialMovementData_Implementation()
{
	if (CharacterRef)
	{
		LastLocomotion = CurrentLocomotion;
		CurrentLocomotion = CharacterRef->LocomotionState;
		ActorRotation = CharacterRef->GetActorRotation();
		bJumpPress = CharacterRef->bJumpPressed;
	}
	if (CharacterMovement)
	{
		Velocity = CharacterMovement->Velocity;
		Speed = Velocity.Length();
		MaxSpeed = CharacterMovement->GetMaxSpeed();
		CurrentAcceleration = CharacterMovement->GetCurrentAcceleration();
		InputVector=CharacterMovement->GetLastInputVector().GetClampedToSize(0,1);
		bIsFalling = CharacterMovement->IsFalling();
		bMovingOnGround = CharacterMovement->IsMovingOnGround();
		bHasMovementInput=InputVector.Length() == 0 ? false : true;
		CycleAngle =CalculateDirection(InputVector, ActorRotation);
	}
	if (bHasMovementInput)
	{
		StopFootSelection = GetCurveValue(FName(TEXT("StopFootSelection")));
	}
}
void UCharacterAnim::CalculateLeanAngle_Implementation()
{
	PreviewActorYaw = ActorYaw;
	ActorYaw = ActorRotation.Yaw;
	ActorDeltaYaw = ActorYaw - PreviewActorYaw;
	LeanAngle=FMath::Clamp(SafeDivide(ActorDeltaYaw, DeltaTime)/LeftOrRightMultiple,-90,90)*GetLeanRate();
}

float UCharacterAnim::GetLeanRate_Implementation()
{
	switch (CurrentLocomotion)
	{
	case ELocomotionState::ELS_Walk:return 1;
	case ELocomotionState::ELS_Run:return 0.5;
	case ELocomotionState::ELS_Sprint:return 0.2;
	default:return 0;
	}
}
void UCharacterAnim::ResetTransition_Implementation()
{
	bCanTurn180 = false;
}
void UCharacterAnim::OnEnd()
{
}

void UCharacterAnim::WhileTrue()
{
	if (!bIsInCombat && !FMath::IsWithinInclusive(CycleAngle, -135, 135))
	{
		if (OnTurn.Execute())
		{
			bCanTurn180 = true;
		}
	}
	else
		OnTurn.Reset();
}

void UCharacterAnim::WhileFalse()
{
}

void UCharacterAnim::OnStart()
{
	MoveStartAngle= CalculateDirection(InputVector, ActorRotation);
}
