// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include"Enum/Enum.h"
#include "CommonActivatableWidget.h"
#include "MainUI.generated.h"
class UWidgetSwitcher;
class UMenu;
class ADarke;
/**
 * 
 */
UCLASS()
class WITCHER3_API UMainUI : public UCommonActivatableWidget
{
	GENERATED_BODY()
public:
	FTimerHandle BeginPlayDelayHandle;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher>WidgetSwitcher;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<ADarke>DarkeRef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UMenu>WBP_MenuClass;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UMenu>WBP_Menu;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UMainState>WBP_MainState;
	UFUNCTION(BlueprintImplementableEvent)
	void SetInteractButtonVisibility(bool bIsVisible,AActor*TriggerActor=nullptr,const FText& ShowText = FText());
protected:
	virtual void NativeConstruct() override;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void BindOpenMenuEvent();
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void HandleOpenMenuEvent(EMenuType MenuType);
	
};
