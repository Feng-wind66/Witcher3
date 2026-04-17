// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/AllGamePlayEffectAsset.h"
#include"Macro/Macro.h"
#include "GameFramework/Character.h"
#include "Blueprint/Interface/TargettingInterface.h"
#include "Blueprint/Interface/InteractableInterface.h"
#include "GameplayEffectTypes.h"
#include "Blueprint/Interface/CharacterInterface.h"
#include "BaseCharacter.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterDeath, AActor*, DeadCharacter);
class UCharacterInfoData;
class UASC;
class UMotionWarpingComponent;
class UAction;
class UEquipment;
class UInventory;
class UInputMappingContext;
class UAnimNotifyState;
class AController;
class AGameMode_1;
class ULimbMeshComponent;
struct FInputActionValue;
UCLASS()
class WITCHER3_API ABaseCharacter : public ACharacter,public ITargettingInterface,public ICharacterInterface,public IInteractableInterface
{
	GENERATED_BODY()

public:
	FTimerHandle InitHPBarTimerHandle;
	ABaseCharacter();
	virtual bool IsCombatTargettable_Implementation()override;
	virtual void DisableCollisionToPawn_Implementation()override;
	virtual EOwnerType GetOwnerType_Implementation()override;
	virtual bool NotHasAnimNotify_Implementation(TSubclassOf<UAnimNotifyState>NotifyState, bool Not)override;
	virtual UEquipment* GetEquipmentComponent_Implementation()override;
	virtual UAction* GetActionComponent_Implementation()override;
	virtual UInventory* GetInventoryComponent_Implementation()override;
	virtual USkeletalMeshComponent* GetSkeletalMeshByType_Implementation(EItemType ItemType)override;
	virtual bool IsCombatDamageble_Implementation()override;
	virtual void ToggleHPBarVisibility_Implementation(bool Visbility)override;
	virtual void SetCombatTargettable_Implementation(bool Targettable)override;
	virtual void SetDodgeType_Implementation(EDodgeType DodgeType)override;
	virtual void SetCurrentLocomotionState_Implementation(ELocomotionState LocomotionType)override;
	virtual void ApplyEnergyCostAPI_Implementation(float EnergyCost)override;
	/*
	PROPERTY
	*/
	AGameMode_1* GameMode;
	FDoOnce ExcuteDeathOnce;
	UPROPERTY(BlueprintAssignable)
	FOnCharacterDeath OnCharacterDeathEvent;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	USkeletalMeshComponent* Helmet;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* ChestArmor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* ArmArmor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* BottomArmor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UASC* ASC;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMotionWarpingComponent* MotionWarping;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAction* Action;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UEquipment* Equipment;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInventory* Inventory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UWidgetComponent>HPBar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EOwnerType OwnerType;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float DefaultMaxWalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DefaultMaxRunSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DefaultMaxSprintSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	bool bJumpPressed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	bool bLockOn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	float MoveForwardValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	bool bMoveStyleFlip;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	ELocomotionState LocomotionState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ELocomotionState LastLocomotionState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsCombatTargettable=true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsCanDamageble=true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRestoreCameraFinish = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TWeakObjectPtr<AActor>KillerOwnerRef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DefaultCapsuleHalfHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RollingCapsuleHalfHeight;//À¶Í¼¸³Öµ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RollingStepHeight;//À¶Í¼¸³Öµ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DefaultStepHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DefaultMeshRelativeZ;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDodgeType CurrentDodgeType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAllGamePlayEffectAsset>AllGE;//À¶Í¼¸³Öµ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentMaxWalkSpeed;
	/*
	FUNCTION
	*/
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION(BlueprintCallable)
	void HandleDeath(AActor* Killer);
	UFUNCTION(BlueprintCallable)
	void PreDismemberment(FName BoneName);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetLocomotionStyle(ELocomotionState LocomotionStyle);
	UFUNCTION(BlueprintCallable)
	virtual void Rolling(float Alpha);
	UFUNCTION(BlueprintCallable)
	virtual void EndRoll();
	UFUNCTION(BlueprintCallable)
	void SetCurrentMagicType(EMagicType MagicType);
	UFUNCTION(BlueprintCallable)
	float GetMagicBaseCost(EMagicType MagicType);
	UFUNCTION(BlueprintCallable)
	void ApplyEnergyCost(float EnergyCost);
protected:
	/*
	PROPERTY
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCharacterInfoData* CharacterInfoData;
	/*
	FUNCTION
	*/
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Init")
	void InitBaseMesh();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Init")
	void InitCharacterBaseInfo(UCharacterInfoData* Data);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Init")
	void InitCustomComponent();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool ANSBool(TSubclassOf<UAnimNotifyState>NotifyState,bool Not);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "State")
	bool IsNormalState();
	UFUNCTION(BlueprintCallable)
	void InitHPBar();
	UFUNCTION(BlueprintImplementableEvent)
	void Dismemberment(FName BoneName, FVector HitDirection);
};
