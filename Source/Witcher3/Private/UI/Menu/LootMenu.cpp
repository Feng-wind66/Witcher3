// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/LootMenu.h"
#include "Components/Button.h"
#include "UI/Button/LootItemButton.h"
#include "Components/ScrollBox.h"
#include "Blueprint/Component/Inventory.h"
#include "Blueprint/Interface/InteractableInterface.h"
#include "Blueprint/Interface/CharacterInterface.h"
void ULootMenu::NativeConstruct()
{
	if (IsValid(BackgroundButton))
	{
		BackgroundButton->OnClicked.AddDynamic(this, &ULootMenu::QuitMenu);
	}
}

void ULootMenu::UpdateDropItemInfo_Implementation(const TArray<FEquipmentInfo>& DropItemArray,AActor* Interactor,AActor* ActiveActor)
{
	if (!IsValid(ScrollBox))return;
	DropItems = DropItemArray;
	WeakActiveActor = ActiveActor;
	WeakInractor = Interactor;
	ScrollBox->ClearChildren();
	for (const FEquipmentInfo& ItemInfo : DropItems)
	{
		ULootItemButton*LootItemButton=CreateWidget<ULootItemButton>(GetWorld(),LootItemButtonClass);
		LootItemButton->UpdateItemInfo(ItemInfo);
		LootItemButton->WeakLootMenuParent =this;
		ScrollBox->AddChild(LootItemButton);
	}
}

void ULootMenu::QuitMenu()
{
	if (WeakActiveActor.IsValid()&&Cast<IInteractableInterface>(WeakActiveActor.Get())&& WeakInractor.Get())
	{
		IInteractableInterface::Execute_EndInteract(WeakActiveActor.Get(), WeakInractor.Get());
	}
}

void ULootMenu::ChildOnClicked(ULootItemButton* ClickedButton)
{
	if (!IsValid(ClickedButton)||!WeakInractor.IsValid()||!WeakActiveActor.IsValid()||!Cast<ICharacterInterface>(WeakInractor.Get())||!Cast<ICharacterInterface>(WeakActiveActor.Get()))return;
	UInventory* ActiveInventory = ICharacterInterface::Execute_GetInventoryComponent(WeakActiveActor.Get());
	UInventory* InteractorInventory = ICharacterInterface::Execute_GetInventoryComponent(WeakInractor.Get());
	if (IsValid(ActiveInventory) && IsValid(InteractorInventory))
	{
		ActiveInventory->DropItemInfos.RemoveSingle(ClickedButton->ItemInfo);
		InteractorInventory->AddItemByInfo(ClickedButton->ItemInfo);
		ScrollBox->RemoveChild(ClickedButton);
		if (!ScrollBox->HasAnyChildren())
		{
			QuitMenu();
		}
	}
	
}

void ULootMenu::LootAllItems()
{
	if (!IsValid(ScrollBox)) return;
	TArray<UWidget*> AllChildren = ScrollBox->GetAllChildren();
	for (int32 i = AllChildren.Num() - 1; i >= 0; --i)
	{
		if (ULootItemButton* Button = Cast<ULootItemButton>(AllChildren[i]))
		{
			ChildOnClicked(Button);
		}
	}
}

TOptional<FUIInputConfig> ULootMenu::GetDesiredInputConfig() const
{
	return FUIInputConfig(ECommonInputMode::All, EMouseCaptureMode::CaptureDuringMouseDown, false);
}
