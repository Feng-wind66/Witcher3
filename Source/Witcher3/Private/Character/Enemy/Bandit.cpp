// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/Bandit.h"
#include"Enum/Enum.h"
#include"Macro/Macro.h"
#include "Blueprint/Component/Inventory.h"
#include "Blueprint/Component/Action.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h" 
ABandit::ABandit()
{
	InitBandit();
}

void ABandit::InitBandit()
{
	Head = CreateDefaultSubobject<USkeletalMeshComponent>(FName("Head"));
	Head->SetupAttachment(GetMesh());
	WeaponStorePoint = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponStorePoint"));
	WeaponStorePoint->SetupAttachment(GetMesh(), FName(TEXT("pelvis")));
	WeaponSheathRoot = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponSheathRoot"));
	WeaponSheathRoot->SetupAttachment(WeaponStorePoint);
	PhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhysicsConstraint"));
	PhysicsConstraint->SetupAttachment(WeaponSheathRoot);
	WeaponSheathTarget = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponSheathTarget"));
	WeaponSheathTarget->SetupAttachment(WeaponStorePoint);
}

UStaticMeshComponent* ABandit::GetStaticMeshByType_Implementation(EDamageSource WeaponType)
{
	return WeaponSheathTarget;
}

void ABandit::BeginPlay()
{
	Super::BeginPlay();
	GetMesh()->SetWorldScale3D(FVector(FMath::RandRange(0.99, 1.05)));
	int32 HeadIndex = FMath::RandRange(0, 2);
	if (HeadArray.Num() > HeadIndex && IsValid(HeadArray[HeadIndex]))
	{
		Head->SetSkeletalMeshAsset(HeadArray[HeadIndex]);
	}
	DELAY(GetWorld()->GetTimerManager(), this, InitBanditEquipmentTimerHandle,1.f, &ABandit::InitBanditEquipment);
}

void ABandit::InitBanditEquipment()
{
	if (Inventory)
	{
		Inventory->InitCharacterEquipmentByID(100001);
		Inventory->InitCharacterEquipmentByID(500001);
		Inventory->InitCharacterEquipmentByID(800001);
		Inventory->InitCharacterEquipmentByID(600001);
		Inventory->InitCharacterEquipmentByID(700001);
	}
	if (Action)
	{
		Action->TriggerAction(FGameplayTag::RequestGameplayTag("Action.Combat.Misc.DrawPrimaryWeapon"), EPriority::EP_Medium);
	}
}
