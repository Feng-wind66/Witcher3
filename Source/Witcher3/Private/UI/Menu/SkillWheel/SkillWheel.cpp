// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/SkillWheel/SkillWheel.h"
#include "Character/Darke.h"
#include "UI/Main/MainUI.h"
#include "UI/Component/MainUIComponent.h"
#include "GAS/UI/MainState.h"
#include "UI/Menu/SkillWheel/SkillIcon.h"
#include "FunctionLibrary/CommonFuction.h"
#include "Kismet/GameplayStatics.h"

void USkillWheel::NativeConstruct()
{
	Super::NativeConstruct();
	DarkeRef=Cast<ADarke>(GetOwningPlayerPawn());
	MainUIComponent=UCommonFuction::GetMainUIComponent(this);
	UMainUI* MainUIRef = MainUIComponent.IsValid() ? MainUIComponent->MainUIRef : nullptr;
	WeakMainState = IsValid(MainUIRef) ? MainUIRef->WBP_MainState : nullptr;
}

void USkillWheel::OpenWheel()
{
	SetVisibility(ESlateVisibility::Visible);
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.1f);
	if (DarkeRef.IsValid())
	{
		DarkeRef->OnRadialInputUpdateEvent.RemoveDynamic(this, &USkillWheel::UpdateWheel);
		DarkeRef->OnRadialInputUpdateEvent.AddDynamic(this, &USkillWheel::UpdateWheel);
	}
}

void USkillWheel::CloseWheel()
{
	SetVisibility(ESlateVisibility::Collapsed);
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
	if (DarkeRef.IsValid())
	{
		DarkeRef->OnRadialInputUpdateEvent.RemoveDynamic(this, &USkillWheel::UpdateWheel);
		DarkeRef->VirtualCursorPos=AdjustAngle().Size()==0? AdjustAngle(): DarkeRef->VirtualCursorPos;
		DarkeRef->SetCurrentMagicType(CurrentMagic);
		if (AdjustedAngle >= 0)
		{
			SkillWheelMID->SetScalarParameterValue(FName("ActiveRotation"), AdjustedAngle);
			SkillWheelMID->SetScalarParameterValue(FName("SelectedRotation"), AdjustedAngle);
		}
	}
	if (WeakMainState.IsValid())
	{
		WeakMainState->ChangeMagicImageByType(CurrentMagic);
	}
}

void USkillWheel::UpdateWheel(FVector2D CursorPos, float Angle)
{
	if (!IsValid(Aard) || !IsValid(Igni) || !IsValid(Yrden) || !IsValid(Quen) || !IsValid(Axii))return;
	SkillWheelMID->SetScalarParameterValue(FName("ActiveRotation"), Angle);
	SkillWheelMID->SetScalarParameterValue(FName("SelectedRotation"), Angle);
	if ((Angle >= 0&&Angle < 22.5)||(Angle>=337.5))
	{
		HoverSkill(Aard);
		CurrentMagic = EMagicType::EMT_Aard;
	}
	else if (Angle >= 22.5 && Angle <= 67.5)
	{
		HoverSkill(Axii);
		CurrentMagic = EMagicType::EMT_Axii;
	}
	else if (Angle > 67.5 && Angle <= 112.5)
	{
		HoverSkill(Igni);
		CurrentMagic = EMagicType::EMT_Igni;
	}
	else if (Angle > 112.5 && Angle <= 157.5)
	{
		HoverSkill(Quen);
		CurrentMagic = EMagicType::EMT_Quen;
	}
	else if (Angle >157.5&& Angle <= 202.5)
	{
		HoverSkill(Yrden);
		CurrentMagic = EMagicType::EMT_Yrden;
	}
	else
	{
		if (IsValid(LastHoveredSkill))
		{
			LastHoveredSkill->UnHovered();
		}
		LastHoveredSkill = nullptr;
		CurrentMagic =EMagicType::EMT_None;
	}
}

void USkillWheel::HoverSkill(USkillIcon* HoveredSkill)
{
	HoveredSkill->OnHovered();
	if (IsValid(LastHoveredSkill) && LastHoveredSkill != HoveredSkill)
	{
		LastHoveredSkill->UnHovered();
	}
	LastHoveredSkill = HoveredSkill;
}

FVector2D USkillWheel::AdjustAngle()
{
	switch (CurrentMagic)
	{
	case EMagicType::EMT_None:AdjustedAngle = -1; return FVector2D(0,0);
		break;
	case EMagicType::EMT_Aard:AdjustedAngle = 0; return FVector2D(1,0);
		break;
	case EMagicType::EMT_Axii:AdjustedAngle = 45; return FVector2D(1, 1);
		break;
	case EMagicType::EMT_Igni:AdjustedAngle = 90; return FVector2D(0,1);
		break;
	case EMagicType::EMT_Quen:AdjustedAngle = 135; return FVector2D(-1,1);
		break;
	case EMagicType::EMT_Yrden:AdjustedAngle = 180; return FVector2D(-1,0);
		break;
	default:AdjustedAngle = -1; return FVector2D(0,0);
		break;
	}
}
