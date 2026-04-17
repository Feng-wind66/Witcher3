// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/ASC.h"
#include"GAS/AS.h"
#include "GAS/AllGamePlayEffectAsset.h"
#include"Macro/Macro.h"
#include "Blueprint/Interface/CharacterInterface.h"
#include "Blueprint/Component/Inventory.h"
#include "Blueprint/Component/Action.h"
#include "Blueprint/Component/Equipment.h"
#include"Table/StatsTableRow.h"
#include "AbilitySystemBlueprintLibrary.h"
UASC::UASC()
{
	FString DataTablePath = TEXT("/Game/Maple/Table/StatsDataTable.StatsDataTable");
	CharacterDataTable = LoadObject<UDataTable>(nullptr, *DataTablePath);
	AS = UAS::StaticClass();
}
void UASC::BeginPlay()
{
	Super::BeginPlay();
	Init();
}
void UASC::Init_Implementation()
{
	if (AS && CharacterDataTable)
	{
		InitStats(AS, CharacterDataTable);
	}
	for (TSubclassOf<UGameplayAbility>Ability : BaseAbility)
	{
		GiveAbility(FGameplayAbilitySpec(Ability,0,-1));
	}
	FGameplayEffectContextHandle Context = MakeEffectContext();
	Context.AddInstigator(GetAvatarActor(), GetAvatarActor());
	if (IsValid(AllGE))
	{
		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingSpec(AllGE->GE_EnergyRecovery, 1.0f, Context);
		if (SpecHandle.Data.IsValid())
		{
			ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
		}
	}
	Owner = GetOwner();
	DELAY(GetWorld()->GetTimerManager(), this, InitComponentHandle, 0.1, &UASC::InitComponent);
}
bool UASC::DealWithDamage(AActor* HittedActor, FGameplayTag DamageType, EDamageSource DamageSource)
{
	if (!IsValid(HittedActor)||!ActionComponent.IsValid())return false;
	UAbilitySystemComponent* HittedASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HittedActor);
	if (!IsValid(HittedASC))return false;
	float DamageMultiplier = ActionComponent->CurrentActionInfo.DamageMultiplier;
	float DamageCorrection = 1;
	if (HittedActor->ActorHasTag((FName(TEXT("Bandit")))) && DamageSource == EDamageSource::EDS_SecondaryWeapon)
	{
		DamageCorrection = 0.1;
	}
	if (HittedActor->ActorHasTag(FName(TEXT("Monster"))) && DamageSource == EDamageSource::EDS_PrimaryWeapon)
	{
		DamageCorrection = 0.1;
	}
	FGameplayEffectContextHandle Context = MakeEffectContext();
	Context.AddInstigator(GetAvatarActor(), GetAvatarActor());
	if (DamageSource == EDamageSource::EDS_Magic)//并没有判断是否是火焰伤害
	{
		if (AllGE && AllGE->GE_Burn)
		{
			FGameplayEffectSpecHandle SpecHandle = MakeOutgoingSpec(AllGE->GE_Burn, 1.0f, Context);
			if (SpecHandle.Data)
			{
				SpecHandle.Data->SetStackCount(1);
				ApplyGameplayEffectSpecToTarget(*SpecHandle.Data, HittedASC);
			}
		}
	}
	FGameplayEffectSpecHandle SpecHandle = MakeOutgoingSpec(AllGE->GE_ApplyDamage, 1.0f,Context);
	if (SpecHandle.Data.IsValid())
	{
		SpecHandle.Data->AddDynamicAssetTag(DamageType);
		AddDamgaeTag(DamageSource,GetWeaponType(DamageSource), *SpecHandle.Data);
		SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName(TEXT("Fight.Damage"))),GetNumericAttribute(UAS::GetAttackAttribute()) * DamageMultiplier* DamageCorrection);
		ApplyGameplayEffectSpecToTarget(*SpecHandle.Data, HittedASC);
	}
	return false;
}//由造成伤害者调用
void UASC::InitComponent()
{
	if (!Owner.IsValid()||!Cast<ICharacterInterface>(Owner))return;
	ActionComponent = ICharacterInterface::Execute_GetActionComponent(Owner.Get());
	InventoryComponent = ICharacterInterface::Execute_GetInventoryComponent(Owner.Get());
	EquipmentComponent= ICharacterInterface::Execute_GetEquipmentComponent(Owner.Get());
}

void UASC::AddDamgaeTag(EDamageSource DamageSource, EItemType WeaponType, FGameplayEffectSpec& Spec)
{
	if (DamageSource == EDamageSource::EDS_PrimaryWeapon && WeaponType == EItemType::EIT_SteelSword)
	{
		Spec.AddDynamicAssetTag(FGameplayTag::RequestGameplayTag(FName(TEXT("Fight.DamageType.Chop"))));
	}
	if (DamageSource == EDamageSource::EDS_PrimaryWeapon && WeaponType == EItemType::EIT_WoodenClub)
	{
		Spec.AddDynamicAssetTag(FGameplayTag::RequestGameplayTag(FName(TEXT("Fight.DamageType.LuntImpact"))));
	}
	if (DamageSource == EDamageSource::EDS_SecondaryWeapon)
	{
		Spec.AddDynamicAssetTag(FGameplayTag::RequestGameplayTag(FName(TEXT("Fight.DamageType.Chop"))));
		Spec.AddDynamicAssetTag(FGameplayTag::RequestGameplayTag(FName(TEXT("Fight.DamageType.Mithril"))));
	}
	if (DamageSource == EDamageSource::EDS_Magic)
	{
		Spec.AddDynamicAssetTag(FGameplayTag::RequestGameplayTag(FName(TEXT("Fight.DamageType.Magic"))));
	}
}

EItemType UASC::GetWeaponType(EDamageSource DamageSource)
{
	if (!InventoryComponent.IsValid())return EItemType::EIT_SteelSword;
	if (DamageSource == EDamageSource::EDS_PrimaryWeapon)
	{
		return InventoryComponent->EquipItemMap.Find(EItemType::EIT_SteelSword) ? EItemType::EIT_SteelSword : EItemType::EIT_WoodenClub;
	}
	if (DamageSource == EDamageSource::EDS_SecondaryWeapon)
	{
		return EItemType::EIT_SilverSword;
	}
	else
	{
		return EItemType::EIT_SteelSword;
	}
}
