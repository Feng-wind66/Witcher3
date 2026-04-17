// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Main/MainUI.h"
#include "UI/Menu/Menu.h"
#include "Blueprint/Component/Action.h"
#include "Components/WidgetSwitcher.h"
#include "Character/Darke.h"
#include"Macro/Macro.h"
#include "FunctionLibrary/CommonFuction.h"
void UMainUI::NativeConstruct()
{
	Super::NativeConstruct();
	DELAY(GetWorld()->GetTimerManager(), this, BeginPlayDelayHandle,0.2,&UMainUI::BindOpenMenuEvent);
}

void UMainUI::BindOpenMenuEvent_Implementation()
{
	if (DarkeRef = UCommonFuction::GetDarkePtr(this))
	{
		DarkeRef->OpenMenuEvent.AddDynamic(this, &UMainUI::HandleOpenMenuEvent);
	}
}

void UMainUI::HandleOpenMenuEvent_Implementation(EMenuType MenuType)
{
	if (WidgetSwitcher)
	{
		if (MenuType == EMenuType::EMT_Close)
		{
			WidgetSwitcher->SetActiveWidgetIndex(0);
			if (GetWorld() && GetWorld()->GetFirstPlayerController())
			{
				GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
				GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
			}
			DarkeRef->Action->SetCurrentActionState(DarkeRef->Action->LastActionState);
			WBP_Menu->DeactivateWidget();
			DeactivateWidget();
		}
		else
		{
			WidgetSwitcher->SetActiveWidgetIndex(1);
			if (GetWorld() && GetWorld()->GetFirstPlayerController())
			{
				GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
				GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
			}
			DarkeRef->Action->SetCurrentActionState(EActionState::EAS_InUIMenu);
			ActivateWidget();
			WBP_Menu->ActivateWidget();
		}
	}
}
