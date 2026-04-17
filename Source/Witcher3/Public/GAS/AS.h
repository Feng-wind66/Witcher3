// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AS.generated.h"

/**
 * 
 */
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WITCHER3_API UAS : public UAttributeSet
{
	GENERATED_BODY()
public:
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue)const override;
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData Level;
	ATTRIBUTE_ACCESSORS(UAS, Level);

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAS, Health);

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAS, MaxHealth);

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData HealthRecovery;
	ATTRIBUTE_ACCESSORS(UAS, HealthRecovery);

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData Attack;
	ATTRIBUTE_ACCESSORS(UAS, Attack);

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData SpellPower;
	ATTRIBUTE_ACCESSORS(UAS, SpellPower);

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData PhysicsDamage;
	ATTRIBUTE_ACCESSORS(UAS, PhysicsDamage);

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData MagicDamage;
	ATTRIBUTE_ACCESSORS(UAS, MagicDamage);

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData AttackSpeed;
	ATTRIBUTE_ACCESSORS(UAS, AttackSpeed);

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(UAS, MoveSpeed);

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData CritRate;
	ATTRIBUTE_ACCESSORS(UAS, CritRate);

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData CritDamage;
	ATTRIBUTE_ACCESSORS(UAS, CritDamage);

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData PunctureResistance;
	ATTRIBUTE_ACCESSORS(UAS, PunctureResistance);

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData ChopResistance;
	ATTRIBUTE_ACCESSORS(UAS, ChopResistance);

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData LuntImpactResistance;
	ATTRIBUTE_ACCESSORS(UAS, LuntImpactResistance);

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData Toxin;
	ATTRIBUTE_ACCESSORS(UAS, Toxin);

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData MaxToxin;
	ATTRIBUTE_ACCESSORS(UAS, MaxToxin);

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData Energy;
	ATTRIBUTE_ACCESSORS(UAS, Energy);

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData MaxEnergy;
	ATTRIBUTE_ACCESSORS(UAS, MaxEnergy);

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData EnergyRecovery;
	ATTRIBUTE_ACCESSORS(UAS, EnergyRecovery);

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData EnergyRecoveryBonus;
	ATTRIBUTE_ACCESSORS(UAS, EnergyRecoveryBonus);

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData Adrenaline;
	ATTRIBUTE_ACCESSORS(UAS, Adrenaline);

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData MaxAdrenaline;
	ATTRIBUTE_ACCESSORS(UAS, MaxAdrenaline);

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData AdrenalineRecoveryBonus;
	ATTRIBUTE_ACCESSORS(UAS, AdrenalineRecoveryBonus);

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData BreakingDefenseValue;
	ATTRIBUTE_ACCESSORS(UAS, BreakingDefenseValue);

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData MaxBreakingDefenseValue;
	ATTRIBUTE_ACCESSORS(UAS, MaxBreakingDefenseValue);

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData Burden;
	ATTRIBUTE_ACCESSORS(UAS, Burden);

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData MaxBurden;
	ATTRIBUTE_ACCESSORS(UAS, MaxBurden);

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData ExecuctionChance;
	ATTRIBUTE_ACCESSORS(UAS, ExecuctionChance);

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData MonsterExperienceAcquisition;
	ATTRIBUTE_ACCESSORS(UAS, MonsterExperienceAcquisition);

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData ShopSellingPrice;
	ATTRIBUTE_ACCESSORS(UAS, ShopSellingPrice);

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UAS, Damage);
};
