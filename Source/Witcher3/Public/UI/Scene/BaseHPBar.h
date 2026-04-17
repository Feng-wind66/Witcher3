// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameplayEffectTypes.h"
#include "Components/ProgressBar.h"
#include "BaseHPBar.generated.h"
/**
 * 
 */
UCLASS()
class WITCHER3_API UBaseHPBar : public UCommonUserWidget
{
	GENERATED_BODY()
public:
	void OnHealthChange(const FOnAttributeChangeData& ChangeData);
	void OnMaxHealthChange(const FOnAttributeChangeData& ChangeData);
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;
	virtual void NativeDestruct() override;
	FDelegateHandle OnHealthChangeDelegateHandle;
	FDelegateHandle OnMaxHealthChangeDelegateHandle;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar>HPBar;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float CurrentHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentMaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TWeakObjectPtr<AActor>OwnerRef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TWeakObjectPtr<class UAbilitySystemComponent>ASCComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseTick;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentHPBarPercent;
	UFUNCTION(BlueprintCallable)
	void InitHPBar(AActor* Owner);
	UFUNCTION(BlueprintCallable)
	void UpdateHPBar();
	UFUNCTION(BlueprintCallable)
	float GetHPPercent();
};
