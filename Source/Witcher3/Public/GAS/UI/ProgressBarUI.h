// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AbilitySystemComponent.h"
#include "Components/ProgressBar.h"
#include "ProgressBarUI.generated.h"
struct FGameplayAttribute;
class UMainUIComponent;
/**
 * 
 */
UCLASS()
class WITCHER3_API UProgressBarUI : public UUserWidget
{
	GENERATED_BODY()
public:
	void OnAttributeChangeEvent(const FOnAttributeChangeData& ChangeData);
	void OnMaxAttributeChangeEvent(const FOnAttributeChangeData& ChangeData);
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;
	virtual void NativeDestruct() override;
	FDelegateHandle AttributeChangeDelegateHandle;
	FDelegateHandle MaxAttributeChangeDelegateHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Width;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Height;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UMaterialInstanceDynamic>BarMID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayAttribute Attribute;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayAttribute MaxAttribute;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentAttributeValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxAttributeValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TWeakObjectPtr<AActor>Owner;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TWeakObjectPtr<UAbilitySystemComponent>ASCComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentAttributePercent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseTick;
	/*
	FUNCTION
	*/
	UFUNCTION(BlueprintCallable)
	void InitBar();//À¶Í¼µ÷ÓÃ
	UFUNCTION(BlueprintCallable)
	float GetPercent();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void UpdateColor(FLinearColor Color);
	UFUNCTION(BlueprintCallable)
	void UpdatePercent();
};
