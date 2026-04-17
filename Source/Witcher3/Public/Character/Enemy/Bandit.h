// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Enemy/BasicEnemy.h"
#include "Bandit.generated.h"
class UPhysicsConstraintComponent;
/**
 * 
 */
UCLASS()
class WITCHER3_API ABandit : public ABasicEnemy
{
	GENERATED_BODY()
public:
	ABandit();
	void InitBandit();
	virtual UStaticMeshComponent* GetStaticMeshByType_Implementation(EDamageSource WeaponType)override;
	/*
	PROPERTY
	*/
	FTimerHandle InitBanditEquipmentTimerHandle;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent>Head;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent>WeaponStorePoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent>WeaponSheathRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent>WeaponSheathTarget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr <UPhysicsConstraintComponent>PhysicsConstraint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<USkeletalMesh>>HeadArray;
	/*
	FUNCTION
	*/
protected:
	virtual void BeginPlay() override;
	void InitBanditEquipment();
};
