// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "BasicEnemy.generated.h"
class UBehaviorTree;
class ADarkeController;
/**
 * 
 */
UCLASS()
class WITCHER3_API ABasicEnemy : public ABaseCharacter
{
	GENERATED_BODY()
public:
	FTimerHandle BeginPlayTimerHandle;
	virtual void Interact_Implementation(AActor* Interactor)override;
	virtual void EndInteract_Implementation(AActor* Interactor)override;
	virtual bool CanBeInteracted_Implementation(AActor* Interactor)const override;
	virtual FText GetInteractionText_Implementation() const override;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UBehaviorTree>BehaviorTreeAsset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TWeakObjectPtr<ADarkeController>WeakDarkeController;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText InteractionText;
protected:
	void BeginPlay()override;
};
