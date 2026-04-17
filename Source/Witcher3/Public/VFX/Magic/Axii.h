// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VFX/Magic/MagicBase.h"
#include "Axii.generated.h"

/**
 * 
 */
UCLASS()
class WITCHER3_API AAxii : public AMagicBase
{
	GENERATED_BODY()
public:
	virtual void Destroyed() override;
	FTimerHandle AxiiEndTimerHandle;
	UPROPERTY()
	TWeakObjectPtr<AActor>TargetActor;
	UPROPERTY()
	TWeakObjectPtr<class UAction>TargetActorAction;
	UPROPERTY()
	TWeakObjectPtr<class UBlackboardComponent>TargetActorBlackboard;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UNiagaraSystem>TargetNiagara;//À¶Í¼¸³Öµ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UNiagaraComponent>AxiiNiagara;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<AActor>EnemyBaseClass;//À¶Í¼¸³Öµ
	UFUNCTION(BlueprintCallable)
	void AxiiEnd();
protected:
	void BeginPlay() override;
};
