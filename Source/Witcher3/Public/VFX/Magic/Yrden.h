// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VFX/Magic/MagicBase.h"
#include "Yrden.generated.h"
class USphereComponent;
/**
 * 
 */
UCLASS()
class WITCHER3_API AYrden : public AMagicBase
{
	GENERATED_BODY()
public:
	AYrden();
	void ApplySlow(UAbilitySystemComponent* HittedASC,AActor* Hitted);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USphereComponent>SphereComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SlowDuration =1.f;
	UPROPERTY()
	TMap<TWeakObjectPtr<AActor>, FActiveGameplayEffectHandle> ActiveSlowHandles;
	UFUNCTION(BlueprintCallable)
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintCallable)
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
protected:
	void BeginPlay() override;
};
