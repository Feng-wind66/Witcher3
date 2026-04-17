// Fill out your copyright notice in the Description page of Project Settings.


#include "Blueprint/Component/Equipment.h"
#include "Blueprint/Component/Action.h"
#include"Macro/Macro.h"
#include "NiagaraComponent.h"
#include "GAS/ASC.h"
#include "Equipment/ArmorBase.h"
#include "Equipment/WeaponBase.h"
#include"Data/CharacterInfoData.h"
#include "FunctionLibrary/CommonFuction.h"
#include "Blueprint/Interface/CharacterInterface.h"
#include "Blueprint/Interface/TargettingInterface.h"
#include "Blueprint/Component/Inventory.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
UEquipment::UEquipment()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEquipment::BeginPlay()
{
	Super::BeginPlay();
	if (ACharacter*OwnerCharacter=Cast<ACharacter>(GetOwner()))
	{
		OwnerRef = OwnerCharacter;
		OwnerMeshRef = OwnerRef->GetMesh();
	}
	if(UWorld*World=GetWorld())
	{
		DELAY(World->GetTimerManager(),this, BindEquipmentModifyEventTimerHandle, 0.1f,&UEquipment::BindEquipmentModifyEvent);
		DELAY(World->GetTimerManager(), this, InitComponentTimerHandle, 0.1f, &UEquipment::InitComponent);
	}
}

void UEquipment::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UEquipment::BindEquipmentModifyEvent()
{
	if (UInventory* InventoryComponent = UCommonFuction::GetInventoryComponnet(OwnerRef))
	{
		InventoryComponent->OnEquipmentModifyEvent.AddDynamic(this, &UEquipment::HandleEquipmentModifyEvent);
	}
}

void UEquipment::SpawnArmor_Implementation(TSubclassOf<AActor>ActorClass, EItemType ItemType)
{
	if (!IsValid(ActorClass))
	{
		return;
	}
	if (!ActorClass&&CurrentArmorActorRef)
	{
		RemoveArmor(ItemType);
		return;
	}
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerRef;
	if (CurrentArmorActorRef)
	{
		CurrentArmorActorRef->Destroy();
	}
	if (UWorld* World = GetWorld())
	{
		if (OwnerRef)
		{
			CurrentArmorActorRef = World->SpawnActor<AArmorBase>(ActorClass, OwnerRef->GetActorLocation(), FRotator(), SpawnParams);
		}
	}
	if (CurrentArmorActorRef&&CurrentArmorActorRef->ArmorSkeletalMesh)
	{
		CurrentArmorMeshRef = ICharacterInterface::Execute_GetSkeletalMeshByType(OwnerRef, ItemType);
		if (CurrentArmorMeshRef)
		{
			CurrentArmorMeshRef->SetSkeletalMesh(CurrentArmorActorRef->ArmorSkeletalMesh->GetSkeletalMeshAsset());
		}
		CurrentArmorActorRef->Destroy();
	}
	else
	{
		RemoveArmor(ItemType);
	}
}

void UEquipment::HandleEquipmentModifyEvent_Implementation(FEquipmentInfo EquipmentInfo)
{
	if (OwnerRef->ActorHasTag(FName(TEXT("Player"))))
	{
		if (UCommonFuction::GetInventoryComponnet(OwnerRef)->IsArmor(EquipmentInfo.EquipmentType))
		{
			SpawnArmor(EquipmentInfo.EquipmentPlayerBPClass, EquipmentInfo.EquipmentType);
		}
		if(UCommonFuction::GetInventoryComponnet(OwnerRef)->IsWeapon(EquipmentInfo.EquipmentType))
		{
			switch (EquipmentInfo.EquipmentType)
			{
			case EItemType::EIT_SteelSword:SpawnWeapon(EquipmentInfo.EquipmentPlayerBPClass, CurrentFirstWeaponActorRef, EDamageSource::EDS_PrimaryWeapon); 
			    break;
			case EItemType::EIT_SilverSword:SpawnWeapon(EquipmentInfo.EquipmentPlayerBPClass, CurrentSecondWeaponActorRef, EDamageSource::EDS_SecondaryWeapon);
				break;
			}
		}
	}
	else if (OwnerRef->ActorHasTag(FName(TEXT("Enemy"))))
	{
		if (UCommonFuction::GetInventoryComponnet(OwnerRef)->IsArmor(EquipmentInfo.EquipmentType))
		{
			SpawnArmor(EquipmentInfo.EquipmentEnemyBPClass, EquipmentInfo.EquipmentType);
		}
		if (UCommonFuction::GetInventoryComponnet(OwnerRef)->IsWeapon(EquipmentInfo.EquipmentType))
		{
			switch (EquipmentInfo.EquipmentType)
			{
			case EItemType::EIT_SteelSword:SpawnWeapon(EquipmentInfo.EquipmentEnemyBPClass, CurrentFirstWeaponActorRef, EDamageSource::EDS_PrimaryWeapon);
				break;
			case EItemType::EIT_SilverSword:SpawnWeapon(EquipmentInfo.EquipmentEnemyBPClass, CurrentSecondWeaponActorRef, EDamageSource::EDS_SecondaryWeapon);
				break;
			}
		}
	}
}

void UEquipment::SpawnWeapon_Implementation(TSubclassOf<AActor> ActorClass, AWeaponBase*& WeaponActorRef,EDamageSource WeaponType)
{
	if (!IsValid(ActorClass)&&WeaponActorRef)
	{
		RemoveWeapon(WeaponActorRef);
		return;
	}
	if (WeaponActorRef)
	{
		RemoveWeapon(WeaponActorRef);
	}
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerRef;
	if (UWorld* World = GetWorld())
	{
		if (OwnerRef)
		{
			WeaponActorRef=World->SpawnActor<AWeaponBase>(ActorClass, OwnerRef->GetActorLocation(), FRotator(), SpawnParams);
			if (WeaponActorRef)
			{
				FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld,true);
				UStaticMeshComponent* StaticMesh = ICharacterInterface::Execute_GetStaticMeshByType(OwnerRef, WeaponType);
				WeaponActorRef->AttachToComponent(StaticMesh, AttachRules);
				WeaponActorRef->WeaponSheath->SetSimulatePhysics(true);
				CurrentMainWeaponAnimType = WeaponActorRef->WeaponInfo.WeaponAnimType;
			}
		}
	}
}

void UEquipment::DrawWeapon_Implementation(EDamageSource WhichWeapon)
{
	switch (WhichWeapon)
	{
	case EDamageSource::EDS_PrimaryWeapon:EquipentWeapon(CurrentFirstWeaponActorRef);
		break;
	case EDamageSource::EDS_SecondaryWeapon:EquipentWeapon(CurrentSecondWeaponActorRef);
		break;
	default:
		break;
	}
}

void UEquipment::EquipentWeapon_Implementation(AWeaponBase*& WeaponActorRef)
{
	if (WeaponActorRef&&OwnerInventoryComponent.IsValid()&&OwnerASC.IsValid())
	{
		WeaponActorRef->MainWeapon->AttachToComponent(OwnerMeshRef, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true),FName("SOCKET_RightHandWeapon"));
		FEquipmentInfo *EquipmentInfo=OwnerInventoryComponent->EquipItemMap.Find(WeaponActorRef->WeaponType == EDamageSource::EDS_PrimaryWeapon ? EItemType::EIT_SteelSword : EItemType::EIT_SilverSword);
		if (EquipmentInfo)
		{
			for (auto& Pair : EquipmentInfo->MainEntry)
			{
				if (IsValid(Pair.Key))
				{
					Pair.Value.GEHandle = OwnerASC->ApplyGameplayEffectToSelf(Pair.Key->GetDefaultObject<UGameplayEffect>(), Pair.Value.Level, OwnerASC->MakeEffectContext());
				}
			}//应用主词条
		}
		CurrentMainWeaponRef = WeaponActorRef;
		CurrentMainWeaponType = WeaponActorRef->WeaponType;
	}//没加棍棒
}

void UEquipment::SheatheWeapon_Implementation(EDamageSource WhichWeapon)
{
	switch (WhichWeapon)
	{
	case EDamageSource::EDS_PrimaryWeapon:UnloadWeapon(CurrentFirstWeaponActorRef);
		break;
	case EDamageSource::EDS_SecondaryWeapon:UnloadWeapon(CurrentSecondWeaponActorRef);
		break;
	default:
		break;
	}
}

void UEquipment::UnloadWeapon_Implementation(AWeaponBase*& WeaponActorRef)
{
	if (WeaponActorRef && OwnerInventoryComponent.IsValid() && OwnerASC.IsValid())
	{
		WeaponActorRef->MainWeapon->AttachToComponent(WeaponActorRef->WeaponSheath, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true));
		if (CurrentMainWeaponRef==WeaponActorRef)
		{
			FEquipmentInfo* EquipmentInfo = OwnerInventoryComponent->EquipItemMap.Find(WeaponActorRef->WeaponType == EDamageSource::EDS_PrimaryWeapon ? EItemType::EIT_SteelSword : EItemType::EIT_SilverSword);
			if (EquipmentInfo)
			{
				for (auto& Pair : EquipmentInfo->MainEntry)
				{
					if (IsValid(AttackEffect) && Pair.Key && Pair.Key == AttackEffect && IsValid(RemoveAttackEffect))
					{
						OwnerASC->ApplyGameplayEffectToSelf(RemoveAttackEffect.GetDefaultObject(), Pair.Value.Level, OwnerASC->MakeEffectContext());
					}
					if (Pair.Value.GEHandle.IsValid())
					{
						OwnerASC->RemoveActiveGameplayEffect(Pair.Value.GEHandle);
						Pair.Value.GEHandle.Invalidate();
					}
				}
			}
			CurrentMainWeaponRef = nullptr;
			CurrentMainWeaponType = EDamageSource::EDS_Item;//空手
		}
	}
}

void UEquipment::InitHitDetectLocations_Implementation()
{
	if (CurrentMainWeaponRef)
	{
		PreviousWeaponCollisionStartLocation=CurrentMainWeaponRef->MainWeapon->GetSocketLocation(FName(TEXT("SOCKET_WeaponTraceStart")));
		PreviousWeaponCollisionEndLocation = CurrentMainWeaponRef->MainWeapon->GetSocketLocation(FName(TEXT("SOCKET_WeaponTraceEnd")));
	}
	else
	{
		PreviousWeaponCollisionStartLocation = FVector::ZeroVector;
		PreviousWeaponCollisionEndLocation = FVector::ZeroVector;
	}
}

void UEquipment::HitDetect()
{
	if (!CurrentMainWeaponRef || !CurrentMainWeaponRef->MainWeapon)return;
	FVector CurrentStartLocation, CurrentEndLocation;
	TArray<FHitResult>HitResult;
	CurrentStartLocation = CurrentMainWeaponRef->MainWeapon->GetSocketLocation(FName(TEXT("SOCKET_WeaponTraceStart")));
	CurrentEndLocation = CurrentMainWeaponRef->MainWeapon->GetSocketLocation(FName(TEXT("SOCKET_WeaponTraceEnd")));
	ETraceTypeQuery WeaponTraceQuery = UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel4);
	for (int i = 1; i <= HitDetectFrameCount; ++i)
	{
		float Alpha = (float)i / (HitDetectFrameCount);
		bool bIsHit = UKismetSystemLibrary::SphereTraceMulti(this, FMath::Lerp(PreviousWeaponCollisionStartLocation, PreviousWeaponCollisionEndLocation, Alpha), FMath::Lerp(CurrentStartLocation, CurrentEndLocation, Alpha), 30, WeaponTraceQuery,false , HittedActors, EDrawDebugTrace::None, HitResult, true);
		if (!bIsHit)continue;
		for (FHitResult& Hit : HitResult)
		{
			AActor* HittedActor = Hit.GetActor();
			UAction* HittedActorAction;
			UAction* OwnerAction;
			UEquipment* HittedActorEquipment;
			if (!HittedActor || !Cast<ITargettingInterface>(HittedActor) || !Cast<ICharacterInterface>(HittedActor))continue;
			if (!ITargettingInterface::Execute_IsCombatDamageble(Hit.GetActor())||HittedActors.Contains(HittedActor))continue;
			if (!Cast<ICharacterInterface>(OwnerRef))continue;
			HittedActorEquipment = ICharacterInterface::Execute_GetEquipmentComponent(HittedActor);
			HittedActorAction = ICharacterInterface::Execute_GetActionComponent(HittedActor);
			OwnerAction = ICharacterInterface::Execute_GetActionComponent(OwnerRef);
			if (!HittedActorAction || !OwnerAction || !HittedActorEquipment)continue;
			//if (HittedActorAction->GetCurrentActionState() == EActionState::EAS_OutOfCombat || HittedActorAction->GetCurrentActionState() == EActionState::EAS_InCombat||HittedActorAction->getcurrent)
			//{
				HittedActors.AddUnique(HittedActor);
				if (HittedActorEquipment->CurrentMainWeaponRef&&CanEnterParryState(HittedActorAction))
				{
					ExecuteParry(HittedActorAction, HittedActorEquipment, Hit);
				}
				else
				{
					HandleCannotBlockAttack(HittedActor, HittedActorAction, OwnerAction, Hit);
					//可以自己去添加关于龙的肉的颤动
				}
			//}
		}
	}
	PreviousWeaponCollisionStartLocation = CurrentStartLocation;
	PreviousWeaponCollisionEndLocation = CurrentEndLocation;
}

void UEquipment::ExecuteParry(UAction* HittedActorAction, UEquipment* HittedActorEquipment, FHitResult& Hit)
{
	HittedActorAction->PlayHitAnim(OwnerRef, EWhichDamageType::EWDT_MeleeWeapon, true);
	if ((CurrentMainWeaponAnimType == EWeaponType::EWT_LongSword || CurrentMainWeaponAnimType == EWeaponType::EWT_OneHandedSword) && (HittedActorEquipment->CurrentMainWeaponAnimType == EWeaponType::EWT_LongSword || HittedActorEquipment->CurrentMainWeaponAnimType == EWeaponType::EWT_OneHandedSword))
	{
		USoundBase* PlaySound = CurrentMainWeaponRef->WeaponInfo.WeaponParrySoundMetal;
		UGameplayStatics::PlaySoundAtLocation(this, PlaySound, Hit.Location);
	}
}

void UEquipment::HandleCannotBlockAttack(AActor* HittedActor, UAction* HittedActorAction, UAction* OwnerAction, FHitResult& Hit)
{
	UASC* OwnerASCRef = Cast<UASC>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwnerRef));
	if (!Cast<ICharacterInterface>(HittedActor))return;
	UEquipment* HittedEquipment = ICharacterInterface::Execute_GetEquipmentComponent(HittedActor);
	UAbilitySystemComponent* HittedASCRef = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HittedActor);
	if (!IsValid(OwnerASCRef)||!IsValid(OwnerAction)||!IsValid(HittedActor)|| !IsValid(HittedASCRef)||!IsValid(HittedEquipment))return;
	if (HittedASCRef->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName(TEXT("State.Buff.Quen")))))
	{
		if (HittedEquipment->QuenActor.IsValid())
		{
			HittedEquipment->QuenActor->Destroy();
		}
		OwnerAction->PlayHitAnim(HittedActor, EWhichDamageType::EWDT_Magic_Quen, false);
	}
	else
	{
		OwnerASCRef->DealWithDamage(HittedActor, HittedActorAction->CurrentActionType, CurrentMainWeaponType);
		if (HittedActorAction->OwnerType != EOwnerType::EOT_Dragon)
		{
			HittedActorAction->PlayHitAnim(OwnerRef, EWhichDamageType::EWDT_MeleeWeapon, false);
		}
		USoundBase* PlaySound = OwnerAction->CurrentActionInfo.ActionType.MatchesTag(FGameplayTag::RequestGameplayTag(FName(TEXT("Action.Combat.Combo.LightAttack")))) ? CurrentMainWeaponRef->WeaponInfo.WeaponLightAttackHitSound : CurrentMainWeaponRef->WeaponInfo.WeaponHeavyAttackHitSound;
		UGameplayStatics::PlaySoundAtLocation(this, PlaySound, Hit.Location);
	}
}

void UEquipment::ClearHittedActors_Implementation()
{
	HittedActors.Empty();
}
void UEquipment::RemoveWeapon(AWeaponBase*& WeaponActorRef)
{
	if (!WeaponActorRef) return;
	if (CurrentMainWeaponRef == WeaponActorRef)
	{
		CurrentMainWeaponRef = nullptr;
		CurrentMainWeaponType = EDamageSource::EDS_Item;
	}
	WeaponActorRef->Destroy();
	WeaponActorRef = nullptr;
}

void UEquipment::RemoveArmor(EItemType ItemType)
{
	CurrentArmorMeshRef = nullptr;
	if (OwnerRef && OwnerRef->GetClass()->ImplementsInterface(UCharacterInterface::StaticClass()))
	{
		if (USkeletalMeshComponent* SkeletalMesh = ICharacterInterface::Execute_GetSkeletalMeshByType(OwnerRef, ItemType))
		{
			SkeletalMesh->SetSkeletalMesh(nullptr);
		}
	}
}
void UEquipment::InitComponent()
{
	if (!IsValid(OwnerRef)||!Cast<ICharacterInterface>(OwnerRef))return;
	OwnerInventoryComponent = ICharacterInterface::Execute_GetInventoryComponent(OwnerRef);
	OwnerASC= UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwnerRef);
	OwnerActionComponent = ICharacterInterface::Execute_GetActionComponent(OwnerRef);
}

bool UEquipment::CanEnterParryState(UAction* HittedActorAction)
{
	if (!IsValid(OwnerRef)||!OwnerActionComponent.IsValid()||!IsValid(OwnerInfoData)||!IsValid(HittedActorAction)||!IsValid(HittedActorAction->OwnerRef))return false;
	if (HittedActorAction->OwnerRef->ActorHasTag(FName(TEXT("Player"))))
	{
		return FMath::Abs(UKismetMathLibrary::FindRelativeLookAtRotation(HittedActorAction->OwnerRef->GetTransform(), OwnerRef->GetActorLocation()).Yaw) <= 110 && HittedActorAction->bIsInParryState;
	}
	return FMath::Abs(UKismetMathLibrary::FindRelativeLookAtRotation(HittedActorAction->OwnerRef->GetTransform(), OwnerRef->GetActorLocation()).Yaw) <= 110&&
		!HittedActorAction->bIsInParryCD&&
		FMath::RandRange(0, 100) <= OwnerInfoData->ParryPercentage&&
		!HittedActorAction->OwnerRef->ActorHasTag("Dragon");
}
