// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include"Enum/Enum.h"
#include "Components/ActorComponent.h"
#include "Equipment.generated.h"
class AArmorBase;
class AWeaponBase;
class UAction;
struct FEquipmentInfo;
UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WITCHER3_API UEquipment : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEquipment();
	FTimerHandle BindEquipmentModifyEventTimerHandle;
	FTimerHandle InitComponentTimerHandle;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	/*
	* PROPERTY
	*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<ACharacter>OwnerRef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent>OwnerMeshRef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AArmorBase>CurrentArmorActorRef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent>CurrentArmorMeshRef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AWeaponBase* CurrentFirstWeaponActorRef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AWeaponBase* CurrentSecondWeaponActorRef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AWeaponBase>CurrentMainWeaponRef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDamageSource CurrentMainWeaponType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector PreviousWeaponCollisionStartLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector PreviousWeaponCollisionEndLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HitDetectFrameCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<AActor>>HittedActors;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponType CurrentMainWeaponAnimType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TWeakObjectPtr<class UInventory>OwnerInventoryComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TWeakObjectPtr<class UAbilitySystemComponent>OwnerASC;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UGameplayEffect>RemoveAttackEffect;//À¶Í¼¸³Öµ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UGameplayEffect>AttackEffect;//À¶Í¼¸³Öµ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TWeakObjectPtr<UAction>OwnerActionComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCharacterInfoData* OwnerInfoData;//À¶Í¼¸³Öµ
	UPROPERTY(VisibleAnywhere)
	TWeakObjectPtr<AActor>QuenActor;
	/*
	FUNCTION
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DrawWeapon(EDamageSource WhichWeapon);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EquipentWeapon(AWeaponBase*& WeaponActorRef);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SheatheWeapon(EDamageSource WhichWeapon);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UnloadWeapon(AWeaponBase*& WeaponActorRef);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void InitHitDetectLocations();
	UFUNCTION(BlueprintCallable)
	void HitDetect();
	void ExecuteParry(UAction* HittedActorAction, UEquipment* HittedActorEquipment, FHitResult& Hit);
	UFUNCTION(BlueprintCallable)
	void HandleCannotBlockAttack(AActor* HittedActor, UAction* HittedActorAction, UAction* OwnerAction, FHitResult& Hit);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ClearHittedActors();
	UFUNCTION(BlueprintCallable)
	void InitComponent();
	UFUNCTION(BlueprintCallable)
	bool CanEnterParryState(UAction* HittedActorAction);
protected:
	virtual void BeginPlay() override;
	void BindEquipmentModifyEvent();
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void SpawnArmor(TSubclassOf<AActor>ActorClass,EItemType ItemType);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void HandleEquipmentModifyEvent(FEquipmentInfo EquipmentInfo);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SpawnWeapon(TSubclassOf<AActor> ActorClass,AWeaponBase *&WeaponActorRef, EDamageSource WeaponType);
	void RemoveArmor(EItemType ItemType);
	void RemoveWeapon(AWeaponBase*& WeaponActorRef);
};
