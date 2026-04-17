// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AN/ReleaseMagic.h"
#include "AbilitySystemComponent.h"
#include "Blueprint/Interface/CharacterInterface.h"
#include "Blueprint/Component/Action.h"
#include "AbilitySystemBlueprintLibrary.h"
void UReleaseMagic::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	AActor* Owner = IsValid(MeshComp) ? MeshComp->GetOwner() : nullptr;
	UWorld* World = IsValid(Owner) ? Owner->GetWorld() : nullptr;
	UAbilitySystemComponent* ReleaserASC =UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner);
	if (!Cast<ICharacterInterface>(Owner) || !IsValid(World)||!IsValid(ReleaserASC))return;
	UAction* ActionComponent = ICharacterInterface::Execute_GetActionComponent(Owner);
	if (!IsValid(ActionComponent))return;
	EMagicType  MagicType = ActionComponent->CurrentMagicType;
	if (MagicType == EMagicType::EMT_None)return;
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = Owner;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AMagicBase* Magic = World->SpawnActor<AMagicBase>(GetMagicClassByMagicType(MagicType), MeshComp->GetSocketLocation(FName("hand_l")), Owner->GetActorRotation(), SpawnParameters);
}

TSubclassOf<AMagicBase> UReleaseMagic::GetMagicClassByMagicType(EMagicType MagicType)
{
	switch (MagicType)
	{
	case EMagicType::EMT_Aard:return Aard;
		break;
	case EMagicType::EMT_Axii:return Axii;
		break;
	case EMagicType::EMT_Igni:return Igni;
		break;
	case EMagicType::EMT_Quen:return Quen;
		break;
	case EMagicType::EMT_Yrden:return Yrden;
		break;
	}
	return nullptr;
}
