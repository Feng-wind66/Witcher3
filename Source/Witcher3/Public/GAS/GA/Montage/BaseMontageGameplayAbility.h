// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/GA/BaseGameplayAbility.h"
#include "BaseMontageGameplayAbility.generated.h"
class UASC;
class UAction;
/**
 * 
 */
UCLASS()
class WITCHER3_API UBaseMontageGameplayAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()
public:
    /*
    PROPERTY
    */
    UPROPERTY(EditAnywhere,BlueprintReadWrite)
    TObjectPtr<AActor> Owner;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UAbilitySystemComponent>ASCRef;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UAction>ActionRef;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UAnimMontage>Montage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CoolDown;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PlayRate;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGameplayTag CoolDownTag;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Cost;
    /*
    FUNCTION
    */
    void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,const FGameplayEventData* TriggerEventData)override;
    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
    UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
    void Init();
    UFUNCTION(BlueprintCallable)
	void OnMontageEnded();
    UFUNCTION(BlueprintCallable)
    void OnMontageQuit();
};
