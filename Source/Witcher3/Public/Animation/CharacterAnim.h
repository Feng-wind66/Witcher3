// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include"Enum/Enum.h"
#include "Blueprint/Interface/CharacterInterface.h"
#include"Macro/Macro.h"
#include "Animation/AnimInstance.h"
#include "CharacterAnim.generated.h"
class ABaseCharacter;
class ADarke;
class UCharacterMovementComponent;
class UEquipment;
class UAction;
/**
 * 
 */
UCLASS()
class WITCHER3_API UCharacterAnim : public UAnimInstance,public ICharacterInterface
{
	GENERATED_BODY()
public:
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds)override;
	virtual void NativeUpdateAnimation(float DeltaSeconds)override;
	virtual void NativeInitializeAnimation()override;
	virtual void NativePostEvaluateAnimation()override;
	virtual void SendParryPress_Implementation(bool bIsPress)override;
protected:
	/*
	PROPERTY
	*/
	FDoOnce OnTrue;
	FDoOnce OnFalse;
	FDoOnce OnTurn;
	FTimerHandle IntialDelayHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reference")
	ABaseCharacter* CharacterRef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reference")
	ADarke* DarkeRef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reference")
	UCharacterMovementComponent* CharacterMovement;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reference")
	UEquipment* Equipment;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reference")
	UAction* Action;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reference")
	USkeletalMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Locomotion")
	ELocomotionState CurrentLocomotion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Locomotion")
	ELocomotionState LastLocomotion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Locomotion")
	float CycleAngle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EssentialData")
	float DeltaTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EssentialData")
	FVector Velocity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EssentialData")
	float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EssentialData")
	float MaxSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EssentialData")
	bool bJumpPress;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EssentialData")
	FVector CurrentAcceleration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EssentialData")
	FVector InputVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EssentialData")
	bool bIsFalling;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EssentialData")
	bool bMovingOnGround;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EssentialData")
	bool bHasMovementInput;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EssentialData")
	float ActorYaw;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EssentialData")
	float PreviewActorYaw;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EssentialData")
	float ActorDeltaYaw;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EssentialData")
	float LeftOrRightMultiple=5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EssentialData")
	float LeanAngle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EssentialData")
	float StopFootSelection;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EssentialData")
	float CyclePlayRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EssentialData")
	float MoveStartAngle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EssentialData")
	bool bIsInCombat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	FRotator ActorRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	bool bCanTurn180;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRideOn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bParryPress;
	/*
	UFUNCTION
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Inital();
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void InitReference();  
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void InitLocomotion();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetEssentialMovementData();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void CalculateLeanAngle();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	float GetLeanRate();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ResetTransition();
	
	void OnStart();
	void OnEnd();
	void WhileTrue();
	void WhileFalse();
};
