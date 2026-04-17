// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include"Enum/Enum.h"
#include "CommonUserWidget.h"
#include "SkillWheel.generated.h"
class USkillIcon;
class UMainUIComponent;
/**
 * 
 */
UCLASS()
class WITCHER3_API USkillWheel : public UCommonUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct()override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillWheel")
	TObjectPtr<UMaterialInstanceDynamic>SkillWheelMID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TWeakObjectPtr<class ADarke>DarkeRef;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USkillIcon>Aard;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USkillIcon>Igni;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USkillIcon>Yrden;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USkillIcon>Quen;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USkillIcon>Axii;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<USkillIcon>LastHoveredSkill;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMagicType CurrentMagic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AdjustedAngle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TWeakObjectPtr<UMainUIComponent>MainUIComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TWeakObjectPtr<class UMainState>WeakMainState;
	/*
	FUNCTION
	*/
	UFUNCTION(BlueprintCallable)
	void OpenWheel();
	UFUNCTION(BlueprintCallable)
	void CloseWheel();
	UFUNCTION(BlueprintCallable)
	void UpdateWheel(FVector2D CursorPos, float Angle);
	UFUNCTION(BlueprintCallable)
	void HoverSkill(USkillIcon* HoveredSkill);
	UFUNCTION(BlueprintCallable)
	FVector2D AdjustAngle();
};
