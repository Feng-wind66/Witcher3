// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include"Macro/Macro.h"
#include "Character/BaseCharacter.h"
#include "Darke.generated.h"
class UPhysicsConstraintComponent;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UEnhancedInputLocalPlayerSubsystem;
class UMainUIComponent;
class UInteract;
struct FEquipmentInfo;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOpenMenuEvent,EMenuType,MenuType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRadialInputUpdateEvent, FVector2D, CursorPos, float, Angle);
UCLASS()
class WITCHER3_API ADarke : public ABaseCharacter
{
	GENERATED_BODY()	
public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;
	virtual void Tick(float DeltaTime) override;
	virtual UStaticMeshComponent* GetStaticMeshByType_Implementation(EDamageSource WeaponType)override;
	virtual bool IsCombatTargettable_Implementation()override;
	virtual void Rolling(float Alpha)override;
	virtual void EndRoll()override;
	ADarke();
	FTimerHandle LockTargetTimerHandle;
	UPROPERTY(BlueprintAssignable)
	FOnOpenMenuEvent OpenMenuEvent;
	UPROPERTY(BlueprintAssignable)
	FOnRadialInputUpdateEvent OnRadialInputUpdateEvent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimerHandle BeginPlayDelayTimerHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USpringArmComponent>SpringArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCameraComponent>Camera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UInteract>InteractComponent;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	USkeletalMeshComponent* Arms;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* Body;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* Leg;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* Hands;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* Hair;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> FirstWeaponStorePoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent>FirstWeaponSheathRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent>FirstWeaponSheathTarget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr <UPhysicsConstraintComponent> FirstPhysicsConstraint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent>SecondWeaponStorePoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent>SecondWeaponSheathRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent>SecondWeaponSheathTarget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr <UPhysicsConstraintComponent> SecondPhysicsConstraint;
	UPROPERTY()
	TWeakObjectPtr<UMainUIComponent>WeakMainUIComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_MoveForward;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_MoveRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_Jump;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_Look;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_SingleTapLeftThumbKey;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_ToggleMoveStyle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_Dodge;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_ChangeWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_EquipIronSword;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_EquipSilverSword;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_LightAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_ReleaseMagic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_SummonHorse;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_ToggleLockOnTarget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_ToggleParry;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_ChooseMagic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_Interactive;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_Back;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_Comsumables_1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_Comsumables_2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_Comsumables_3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_Comsumables_4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* IA_InventoryPickUp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_InventoryPickUpAll;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_OpenBigMap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_ToggleInventoryUI;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|RadialMenu")
	UInputAction* IA_RadialMenuLook;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UEnhancedInputLocalPlayerSubsystem>EnhanceInputSystem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputMappingContext>IMC_RadialMenu;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentDeltaTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DefaultSpringArmLength=170;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector DefaultSpringArmSocketOffset=FVector(0,60,0);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CameraLockSpringArmLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DefaultSpringArmZ;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimNotifyState>DisableLockOnRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D VirtualCursorPos;
protected:
	virtual void BeginPlay() override;
	virtual void Landed(const FHitResult& Hit) override;
	
	/*
	PROPERTY
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector StartFallingLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MaxFallingHigh=1000;
	/*
	FUNCTION
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void InitDarkeMesh();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Init();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void InitCamera();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnControllerChanged(AController* NewController);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Input")
	void MoveForward(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Input")
	void MoveRight(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Input")
	void OnJump(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Input")
	void Look(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Input")
	void SingleTapLeftThumbKey(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Input")
	void ToggleMoveStyle(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Input")
	void Dodge(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Input")
	void ChangeWeapon(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Input")
	void LightAttack(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Input")
	void SummonHorse(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Input")
	void ToggleLockOnTarget(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable,Category = "Input")
	void ToggleParry();
	UFUNCTION(BlueprintCallable, Category = "Input")
	void EndParry();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Input")
	void Interactive(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Input")
	void Back(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Input")
	void Comsumables_1(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Input")
	void Comsumables_2(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Input")
	void Comsumables_3(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Input")
	void Comsumables_4(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Input")
	void InventoryPickUp(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Input")
	void InventoryPickUpAll(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Input")
	void OpenBigMap(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Input")
	void ToggleInventoryUI(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable)
	void HandleRadialInput(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Input")
	bool CanOpenInventory();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Movement")
	void IsFallingDeath();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DrawFirstWeapon();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DrawSecondWeapon();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void LockCameraSystem();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void LockTarget();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AdjustCameraInLock();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	float GetSpringArmLengthByTarget(EOwnerType TargetType);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FVector GetSpringArmSocketTargetOffsetByDodgeType(EDodgeType DodgeType);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void RestoreCameraSet();
	UFUNCTION(BlueprintCallable)
	void ChooseMagic();
	UFUNCTION(BlueprintCallable)
	void ChooseMagicComplete();
	UFUNCTION(BlueprintCallable)
	void ReleaseMagic();
	UFUNCTION(BlueprintCallable, Category = "Input")
	FORCEINLINE void MoveForwardComplete() { MoveForwardValue = 0; };
	UFUNCTION(BlueprintCallable, Category = "Input")
	FORCEINLINE void MoveRightComplete() { MoveForwardValue = 0; };

};
