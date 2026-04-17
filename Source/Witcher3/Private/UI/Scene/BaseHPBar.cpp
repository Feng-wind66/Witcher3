// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Scene/BaseHPBar.h"
#include"GAS/AS.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
void UBaseHPBar::InitHPBar(AActor* Owner)
{
	if (!IsValid(Owner))return;
	OwnerRef = Owner;
	ASCComponent= UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner);
	if (!ASCComponent.IsValid())return;
	CurrentHealth = ASCComponent->GetNumericAttribute(UAS::GetHealthAttribute());
	CurrentMaxHealth = ASCComponent->GetNumericAttribute(UAS::GetMaxHealthAttribute());
	OnHealthChangeDelegateHandle = ASCComponent->GetGameplayAttributeValueChangeDelegate(UAS::GetHealthAttribute()).AddUObject(this, &UBaseHPBar::OnHealthChange);
	OnMaxHealthChangeDelegateHandle = ASCComponent->GetGameplayAttributeValueChangeDelegate(UAS::GetMaxHealthAttribute()).AddUObject(this, &UBaseHPBar::OnMaxHealthChange);
	UpdateHPBar();
}

void UBaseHPBar::UpdateHPBar()
{
	bUseTick = true;
}

float UBaseHPBar::GetHPPercent()
{
	if (CurrentMaxHealth == 0)return 0;
	return CurrentHealth / CurrentMaxHealth;
}

void UBaseHPBar::OnHealthChange(const FOnAttributeChangeData& ChangeData)
{
	CurrentHealth = ChangeData.NewValue;
	UpdateHPBar();
}

void UBaseHPBar::OnMaxHealthChange(const FOnAttributeChangeData& ChangeData)
{
	CurrentMaxHealth = ChangeData.NewValue;
	UpdateHPBar();
}

void UBaseHPBar::NativeConstruct()
{
	Super::NativeConstruct();
	CurrentHealth = ASCComponent->GetNumericAttribute(UAS::GetHealthAttribute());
	CurrentMaxHealth = ASCComponent->GetNumericAttribute(UAS::GetMaxHealthAttribute());
	OnHealthChangeDelegateHandle = ASCComponent->GetGameplayAttributeValueChangeDelegate(UAS::GetHealthAttribute()).AddUObject(this, &UBaseHPBar::OnHealthChange);
	OnMaxHealthChangeDelegateHandle = ASCComponent->GetGameplayAttributeValueChangeDelegate(UAS::GetMaxHealthAttribute()).AddUObject(this, &UBaseHPBar::OnMaxHealthChange);
	UpdateHPBar();
}

void UBaseHPBar::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);
	if (!bUseTick||!IsValid(HPBar))return;
	float TargetPercent = GetHPPercent();
	if (!FMath::IsNearlyEqual(CurrentHPBarPercent, TargetPercent))
	{
		float InterpSpeed =FMath::Max(FMath::Abs(TargetPercent - CurrentHPBarPercent) * 10,3);
		CurrentHPBarPercent = FMath::FInterpTo(CurrentHPBarPercent, TargetPercent, DeltaTime, InterpSpeed);
		HPBar->SetPercent(CurrentHPBarPercent);
	}
	else
	{
		CurrentHPBarPercent = TargetPercent;
		HPBar->SetPercent(CurrentHPBarPercent);
		bUseTick = false;
	}
}
void UBaseHPBar::NativeDestruct()
{
	if (ASCComponent.IsValid())
	{
		ASCComponent->GetGameplayAttributeValueChangeDelegate(UAS::GetHealthAttribute()).Remove(OnHealthChangeDelegateHandle);
		ASCComponent->GetGameplayAttributeValueChangeDelegate(UAS::GetMaxHealthAttribute()).Remove(OnMaxHealthChangeDelegateHandle);
	}
	Super::NativeDestruct();
}


