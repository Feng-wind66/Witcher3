// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/AllGamePlayEffectAsset.h"
#include "GameFramework/Actor.h"
#include"Enum/Enum.h"
#include "Components/BoxComponent.h"
#include "MagicBase.generated.h"
class UNiagaraComponent;
class UBoxComponent;
UCLASS()
class WITCHER3_API AMagicBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AMagicBase();
	FTimerHandle DisableCollisionTimerHandle;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UNiagaraComponent>NiagaraComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UBoxComponent> CollisionBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector BoxSize;
	UPROPERTY(VisibleAnywhere)
	TWeakObjectPtr<AActor>Releaser;
	UPROPERTY(VisibleAnywhere)
	TArray<TWeakObjectPtr<AActor>>HittedActors;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAllGamePlayEffectAsset>AllGE;//À¶Í¼¸³Öµ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BoxLifeSpan;//À¶Í¼¸³Öµ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MagicBaseCost;//À¶Í¼¸³Öµ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ActorLifeSpan;//À¶Í¼¸³Öµ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TWeakObjectPtr<UAbilitySystemComponent>ReleaserASC;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EMagicType, USoundBase*>MagicSoundMap;//À¶Í¼ÖÐ¸³Öµ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMagicType MagicType;//À¶Í¼¸³Öµ
	/*
	FUNCTION
	*/
	UFUNCTION(BlueprintCallable)
	virtual void OnMagicOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintCallable)
	void DisableCollision();
	UFUNCTION(BlueprintCallable)
	float GetStartTimeByMagicType(EMagicType Type);
protected:
	virtual void BeginPlay() override;
public:	
	virtual void Tick(float DeltaTime) override;
};
