// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AS.h"
#include "Character/BaseCharacter.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Blueprint/Component/Action.h"
#include "Blueprint/Interface/CharacterInterface.h"
void UAS::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		AActor* SourceActor = Data.EffectSpec.GetContext().GetInstigator();
		if (UAbilitySystemComponent*KillerASC= UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceActor))
		{
			SourceActor = KillerASC->GetAvatarActor();
		}
		ABaseCharacter* TargetChar = Cast<ABaseCharacter>(Data.Target.AbilityActorInfo->AvatarActor.Get());
		if (IsValid(TargetChar))
		{
			bool bIsDead = TargetChar->ActorHasTag(FName("Death"));
			if (GetHealth() <= 0.0f && !bIsDead)
			{
				if (UAction* ActionComp = ICharacterInterface::Execute_GetActionComponent(TargetChar))
				{
					FGameplayTagContainer AssetTags;
					Data.EffectSpec.GetAllAssetTags(AssetTags);
					if (AssetTags.HasTag(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Combustion"))))
					{
						ActionComp->LethalDamageType = EWhichDamageType::EWDT_Magic_Igni;
					}
				}
				TargetChar->HandleDeath(SourceActor);
			}
		}
	}
}
void UAS::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	if (Attribute == GetEnergyAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxEnergy());
	}
}

void UAS::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	else if (Attribute == GetEnergyAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxEnergy());
	}
}
