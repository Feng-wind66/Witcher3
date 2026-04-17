// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/UI/ProgressBarUI.h"
#include "UI/Component/MainUIComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
void UProgressBarUI::InitBar()
{
	Owner = GetOwningPlayerPawn();
	if (!Owner.IsValid())return;
	ASCComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner.Get());
	if (!ASCComponent.IsValid())return;
	CurrentAttributeValue = ASCComponent->GetNumericAttribute(Attribute);
	MaxAttributeValue = ASCComponent->GetNumericAttribute(MaxAttribute);
	AttributeChangeDelegateHandle = ASCComponent->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(this, &UProgressBarUI::OnAttributeChangeEvent);
	MaxAttributeChangeDelegateHandle = ASCComponent->GetGameplayAttributeValueChangeDelegate(MaxAttribute).AddUObject(this, &UProgressBarUI::OnMaxAttributeChangeEvent);
	UpdatePercent();
}
float UProgressBarUI::GetPercent()
{
	if (MaxAttributeValue != 0)
	{
		return CurrentAttributeValue / MaxAttributeValue;
	}
	else
	{
		return 0.0f;
	}
}
void UProgressBarUI::UpdatePercent()
{
	bUseTick = true;
}
void UProgressBarUI::OnAttributeChangeEvent(const FOnAttributeChangeData& ChangeData)
{
	CurrentAttributeValue = ChangeData.NewValue;
	UpdatePercent();
}
void UProgressBarUI::OnMaxAttributeChangeEvent(const FOnAttributeChangeData& ChangeData)
{
	MaxAttributeValue = ChangeData.NewValue;
	UpdatePercent();
}

void UProgressBarUI::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);
	if (!bUseTick)return;
	float TargetPercent = GetPercent();
	if (!FMath::IsNearlyEqual(CurrentAttributePercent, TargetPercent))
	{
		//float InterpSpeed = FMath::Max(FMath::Abs(TargetPercent - CurrentAttributePercent) * 10, 3);
		CurrentAttributePercent = FMath::FInterpTo(CurrentAttributePercent, TargetPercent,DeltaTime,3);
		if (IsValid(BarMID))
		{
			BarMID->SetScalarParameterValue(FName("Percent"), CurrentAttributePercent);
		}
	}
	else
	{
		CurrentAttributePercent =TargetPercent;
		if (IsValid(BarMID))
		{
			BarMID->SetScalarParameterValue(FName("Percent"), CurrentAttributePercent);
		}
		bUseTick = false;
	}
}

void UProgressBarUI::NativeDestruct()
{
	if (ASCComponent.IsValid())
	{
		ASCComponent->GetGameplayAttributeValueChangeDelegate(Attribute).Remove(AttributeChangeDelegateHandle);
		ASCComponent->GetGameplayAttributeValueChangeDelegate(MaxAttribute).Remove(MaxAttributeChangeDelegateHandle);
	}
	Super::NativeDestruct();
}
