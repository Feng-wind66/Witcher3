// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagsManager.h"//得放在头文件
#include"Enum/Enum.h"
#include"Macro/Macro.h"
#include "Struct/ActionStruct.h"
#include "Components/ActorComponent.h"
#include "Action.generated.h"
class UCharacterInfoData;
class ICharacterInterface;
class UCharacterMovementComponent;
class AGameMode_1;
class ADarke;
class UASC;
class UEquipment;
UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WITCHER3_API UAction : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAction();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    /*
	PROPERTY
	*/
	FTimerHandle ParryTimeHandle;
	FTimerHandle ParryCDTimeHandle;
	FTimerHandle ExecuteFinishTimeHandle;
	FDoOnce ControlParryCD;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AGameMode_1>GameModeRef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr <ADarke>PlayerRef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UASC>ASCRef;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bIsInCombat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class")
	TSubclassOf<UCharacterMovementComponent>MovementComponentClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class")
	TSubclassOf<USkeletalMeshComponent>MeshComponentClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reference")
	UCharacterInfoData* OwnerInfoData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reference")
	AActor* OwnerRef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reference")
	UCharacterMovementComponent* OwnerMovementComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reference")
	USkeletalMeshComponent* OwnerMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActionState")
	EActionState LastActionState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActionPriority")
	EPriority LastActionPriority;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActionPriority")
	EPriority CurrentActionPriority;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActionTag")
	FGameplayTag CurrentActionTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FActionInfo CurrentActionInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag CurrentActionType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CurrentComboCounter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponType CurrentWeaponType;
	UPROPERTY(VisibleAnywhere, Category = "Targeting")
	TArray<TWeakObjectPtr<AActor>> TargetsInRange;
	UPROPERTY(VisibleInstanceOnly, Transient, Category = "Targeting")
	TArray<TWeakObjectPtr<AActor>> RecentlyLockedTargets;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DetectRadius;//蓝图中设置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor>DetectActorClass;//蓝图中设置
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Transient, Category = "Targeting")
	TWeakObjectPtr<AActor> CurrentLockTarget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CloseDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EOwnerType LockTargetType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimNotifyState> InputBufferNotifyState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag PendingAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentDeltaTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InputBufferDuration = 0.4f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double LastInputRequestTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector FinalMotionWarpingLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator FinalMotionWarpingRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanMotionWarping=false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanRotate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurveTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActionState")
	EOwnerType OwnerType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TWeakObjectPtr<UEquipment>EquipmentComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FHitInfo CurrentHitInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector FinalHitMotionWarpingLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator FinalHitMotionWarpingRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TWeakObjectPtr<AActor>CurrentAttackOwnerActorRef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FGameplayTag>MeleeHitReactions;//在蓝图设置默认值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag PendingDeathTag;//在蓝图设置默认值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsInParryState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsInParryCD;
	UPROPERTY(VisibleAnywhere)
	TArray<TWeakObjectPtr<AActor>>PausedAIActors;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TWeakObjectPtr<class ABaseCharacter>CharacterRef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector FinalDodgeMotionWarpingLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator FinalDodgeMotionWarpingRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMagicType CurrentMagicType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWhichDamageType LethalDamageType;
	UPROPERTY(VisibleAnywhere)
	TWeakObjectPtr<AActor>AxiiActor;
	/*
	FUNCTION
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Init")
	void InitActorComponent(UCharacterInfoData* Data);
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetLastOnGroundActionState(EActionState ActionState) { LastOnGroundActionState = ActionState; };
	UFUNCTION(BlueprintCallable)
	FORCEINLINE EActionState GetLastOnGroundActionState() { return LastOnGroundActionState; };
	FORCEINLINE EActionState GetCurrentActionState() { return CurrentActionState; };
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void SetCurrentActionState(EActionState ActionState);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool TriggerAction(FGameplayTag NewAction,EPriority Priority,bool bUseInputBuffer=true);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetPriority(EPriority NewPriority);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetCurrentActionTag(FGameplayTag NewActionTag);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool HandleNewAction(FGameplayTag& NewAction, EPriority Priority);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ResetCombos();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ResetAction();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void HandleLongSwordAction();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AActor* LockTarget(float Radius,TSubclassOf<AActor>ActorClass,bool ResetVariables);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ResetLockTargetVariables();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TArray<AActor*> GetTargetsInRange(float Radius, TSubclassOf<AActor>ActorClass);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ClearCurrentLockTarget();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    AActor* GetClosestActorInRange(float Radius, TSubclassOf<AActor>ActorClass);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	EOwnerType SetLockOnTargetType(AActor*LockActor);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FGameplayTag RandomGetComboActionTag();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void InputBuffer(FGameplayTag BufferAction);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ExecuteInputBuffer();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void GetMotionWarpingLocationAndRotation();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateAttackerToTargetLocationAndRotation();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void MotionWarping();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void MotionWarpingOnGroundAttacker();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ResetMotionWarping();
	UFUNCTION(BlueprintCallable)
	void PlayHitAnim(AActor* Attacker,EWhichDamageType DamageType, bool bIsParry);
	UFUNCTION(BlueprintCallable)
	bool CanLockOnActor(AActor* Target);
	UFUNCTION(BlueprintCallable)
	void MotionWarpingOnGroundHit();
	UFUNCTION(BlueprintCallable)
	void UpdateHitLocationAndRotation(AActor* Attacker);
	UFUNCTION(BlueprintCallable)
	bool CheakCanFinishered(AActor* Attacker);
	UFUNCTION(BlueprintCallable)
	void GetAndSetHitInfoByTag(const FGameplayTag &HitTag);
	UFUNCTION(BlueprintCallable)
	void HandleDeathEvent(AActor*DeadCharacter);
	UFUNCTION(BlueprintCallable)
	void ExecuteDeath(AActor*Killer);
	UFUNCTION(BlueprintCallable)
	void PlayPreFinisherAnim();
	UFUNCTION(BlueprintCallable)
	void ExecuteFinisher(UAction* FinisherAction);
	UFUNCTION(BlueprintCallable)
	void SetCurrentActionInfoByTag(FGameplayTag ActionTag);
	UFUNCTION(BlueprintCallable)
	void UpdateFinisherLocationAndRotation();
	UFUNCTION(BlueprintCallable)
	void EndParry();
	UFUNCTION(BlueprintCallable)
	void SurroundingAIPauseAttack(float Raduis,TSubclassOf<AActor>ActorClass);
	UFUNCTION(BlueprintCallable)
	void SurroundingAIResumeAttack();
	UFUNCTION(BlueprintCallable)
	void SetActorAIAttackState(AActor*Actor,bool AttackState);
	UFUNCTION(BlueprintCallable)
	void UpdateDodgeLocationAndRotation();
	UFUNCTION(BlueprintCallable)
	void MotionWarpingOnGroundDodge();
	UFUNCTION(BlueprintCallable)
	void ChooseDodge();
	UFUNCTION(BlueprintCallable)
	void SetDodgeType(EDodgeType DodgeType);
	UFUNCTION(BlueprintCallable)
	void HandleMeleeHit(UAction* AttackerAction);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	

private:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActionState", meta = (AllowPrivateAccess = "true"))
	EActionState CurrentActionState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActionState",meta = (AllowPrivateAccess = "true"))
	EActionState LastOnGroundActionState;
};
