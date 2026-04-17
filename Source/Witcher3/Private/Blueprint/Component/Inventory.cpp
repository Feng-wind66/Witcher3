// Fill out your copyright notice in the Description page of Project Settings.


#include "Blueprint/Component/Inventory.h"
#include "UI/Inventory/Backpack/Backpack_Slot.h"
#include "GAS/AllGamePlayEffectAsset.h"
#include "UI/Inventory/Backpack/BackpackGrid.h"
#include "Blueprint/Interface/CharacterInterface.h"
#include "UI/Inventory/Option/Option.h"
#include"Blueprint/Component/RetainInventoryData.h"
#include "FunctionLibrary/CommonFuction.h"
#include "Engine/DataTable.h"
#include "GAS/ASC.h"
#include"GAS/AS.h"
#include "AbilitySystemBlueprintLibrary.h"
UInventory::UInventory()
{
	PrimaryComponentTick.bCanEverTick = true;
}
void UInventory::BeginPlay()
{
	Super::BeginPlay(); 
    InitInventory();
}
void UInventory::InitInventory_Implementation()
{
    InitItemData();
    InitEntryData();
    if ((Owner = GetOwner()) != nullptr)
    {
        ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner);
        if (IsValid(ASC))
        {
            ASC->GetGameplayAttributeValueChangeDelegate(UAS::GetHealthAttribute()).AddUObject(this, &UInventory::HandleOwnerDeathEvent);
        }
    }
}
void UInventory::InitEntryData_Implementation()
{
    if (!EntryDataTable|| EntryDataTable->GetRowStruct() != FEntryDataRow::StaticStruct())
        return;
    const TMap<FName, uint8*>& RowMap = EntryDataTable->GetRowMap();
    for (const auto& Pair : RowMap)
    {
        const uint8* RowData = Pair.Value;
        if (!RowData) 
            continue;
        const FEntryDataRow* Row = reinterpret_cast<const FEntryDataRow*>(RowData);
        if (Row)
        {
            EntryTableMap.Add(Row->ID, *Row);
            switch ((int32)Row->Rarity)
            {
            case 1:LevelOne_EntryTableMap.Add(Row->ID, *Row);LevelOneEntry_IDArray.Add(Row->ID);
                break;
            case 2:LevelTwo_EntryTableMap.Add(Row->ID, *Row);LevelTwoEntry_IDArray.Add(Row->ID);
                break;
            case 3:LevelThree_EntryTableMap.Add(Row->ID, *Row); LevelThreeEntry_IDArray.Add(Row->ID);
                break;
            }
        }
    }
}
void UInventory::InitItemData_Implementation()
{
    if (!ItemDataTable || ItemDataTable->GetRowStruct() != FItemDataRow::StaticStruct())
        return;
    const TMap<FName, uint8*>& RowMap = ItemDataTable->GetRowMap();
    for (const auto& Pair : RowMap)
    {
        const FName RowName = Pair.Key;
        const uint8* RawData = Pair.Value;
        if (!RawData)
            continue;
        const FItemDataRow* Row = reinterpret_cast<const FItemDataRow*>(RawData);
        if (Row)
            ItemTableMap.Add(Row->ItemID, *Row);
    }
}
FEquipmentInfo UInventory::SpawnEquipmentByID_Implementation(int32 ID, int32 Count=1)
{
    FItemDataRow *ItemData=ItemTableMap.Find(ID);
    if (!ItemData)return FEquipmentInfo();
    FEquipmentInfo EquipmentInfo;
    EquipmentInfo.Stack = Count;
    if (IsWeapon(ItemData->ItemType) || IsArmor(ItemData->ItemType))
    {
        EquipmentInfo.Stack = 1;
    }
    ItemDataToEquipmentData(EquipmentInfo, *ItemData);
    return EquipmentInfo;
}
void UInventory::ItemDataToEquipmentData_Implementation(FEquipmentInfo& EquipmentInfo, FItemDataRow &ItemData)
{
    CopyItemBaseInfoToEquipment(EquipmentInfo, ItemData);
    FEquipmentEntryInfo FirstMainEntryInfo;
    FEquipmentEntryInfo SubEntryInfo;
    AddEquipmentMainEntry(EquipmentInfo);
    AddEquipmentSubEntry(ItemData, SubEntryInfo, EquipmentInfo);
}
void UInventory::CopyItemBaseInfoToEquipment_Implementation(FEquipmentInfo& EquipmentInfo, FItemDataRow& ItemData)
{
    PlayerLevel = UCommonFuction::GetPlayerLevel(this);
    EquipmentInfo.ItemHandle = 0;
    EquipmentInfo.EquipmentID = ItemData.ItemID;
    EquipmentInfo.EquipmentLevel = PlayerLevel;
    EquipmentInfo.EquipmentName = ItemData.Name;
    EquipmentInfo.EquipmentDescribe = ItemData.ItemDescribe;
    EquipmentInfo.EquipmentIcon = ItemData.ItemIcon;
    EquipmentInfo.EquipmentType = ItemData.ItemType;
    EquipmentInfo.EquipmentEnemyBPClass = ItemData.ItemEnemyBPClass;
    EquipmentInfo.EquipmentPlayerBPClass = ItemData.ItemPlayerBPClass;
    EquipmentInfo.EquipmentQuality = ItemData.ItemQuality;
    EquipmentInfo.Price = ItemData.Price;
    EquipmentInfo.MaxStack = GetItemMaxStack(EquipmentInfo.EquipmentType);
}
void UInventory::AddEquipmentMainEntry_Implementation(FEquipmentInfo& EquipmentInfo)
{
    if (IsWeapon(EquipmentInfo.EquipmentType) && EntryTextData && GEAsset)
        AddMainEntry(EquipmentInfo, EntryTextData->Attack_Base, GEAsset->GE_Entry_Attack_Base, true);//添加武器主词条
    if (IsArmor(EquipmentInfo.EquipmentType) && EntryTextData && GEAsset)
    {
        AddMainEntry(EquipmentInfo, EntryTextData->ChopResistance, GEAsset->GE_Entry_ChopResistance, false);//劈砍抗性
        //AddMainEntry(EquipmentInfo, EntryTextData->BluntImpactResistance, GEAsset->GE_Entry_BluntImpactResistance, false);//钝击抗性
        AddMainEntry(EquipmentInfo, EntryTextData->PunctureResistance, GEAsset->GE_Entry_PunctureResistance, false);
    }//添加装备主词条
    //EquipmentInfo.UniqueEntry;独特属性
    //不完善还有问题比如衣服可以叠加
}
void UInventory::AddEquipmentSubEntry_Implementation(FItemDataRow& ItemData, FEquipmentEntryInfo& SubEntryInfo, FEquipmentInfo& EquipmentInfo)
{
    int32 SubEntryNum = FMath::RandRange(ItemData.MinEntryCount, ItemData.MaxEntryCount);
    TArray<TSubclassOf<UGameplayEffect>>DeduplicateArray;
    for (int i = 0; i < SubEntryNum; ++i)
    {
        int32 RandIndex = 0, SubEntryLevel = FMath::RandRange(ItemData.MinEntryLevel, ItemData.MaxEntryLevel);
        FEntryDataRow* EntryData;
        switch (SubEntryLevel)
        {
        case 1:
        {
            do{
                RandIndex = FMath::RandRange(0, LevelOneEntry_IDArray.Num() - 1);
                EntryData = LevelOne_EntryTableMap.Find(LevelOneEntry_IDArray[RandIndex]);
            } while (EntryData && DeduplicateArray.Contains(EntryData->Attribute));
            SubEntryInfo.Describe = EntryData->Describe;
            SubEntryInfo.ID = EntryData->ID;
            SubEntryInfo.Level = FMath::RandRange(EntryData->Min, EntryData->Max);
            EquipmentInfo.SubEntry.Add(EntryData->Attribute, SubEntryInfo);
            DeduplicateArray.Add(EntryData->Attribute);
        }
        break;
        case 2:
        {
            do {
                RandIndex = FMath::RandRange(0, LevelTwoEntry_IDArray.Num() - 1);
                EntryData = LevelTwo_EntryTableMap.Find(LevelTwoEntry_IDArray[RandIndex]);
            } while (EntryData && DeduplicateArray.Contains(EntryData->Attribute));
            SubEntryInfo.Describe = EntryData->Describe;
            SubEntryInfo.ID = EntryData->ID;
            SubEntryInfo.Level = FMath::RandRange(EntryData->Min, EntryData->Max);
            EquipmentInfo.SubEntry.Add(EntryData->Attribute, SubEntryInfo);
            DeduplicateArray.Add(EntryData->Attribute);
        }
        break;
        case 3:
        {
            do {
                RandIndex = FMath::RandRange(0, LevelThreeEntry_IDArray.Num() - 1);
                EntryData = LevelThree_EntryTableMap.Find(LevelThreeEntry_IDArray[RandIndex]);
            } while (EntryData && DeduplicateArray.Contains(EntryData->Attribute));
            SubEntryInfo.Describe = EntryData->Describe;
            SubEntryInfo.ID = EntryData->ID;
            SubEntryInfo.Level = FMath::RandRange(EntryData->Min, EntryData->Max);
            EquipmentInfo.SubEntry.Add(EntryData->Attribute, SubEntryInfo);
            DeduplicateArray.Add(EntryData->Attribute);
        }
        break;
        }
    }
}
bool UInventory::IsWeapon_Implementation(EItemType ItemType)
{
    if (ItemType == EItemType::EIT_SteelSword || ItemType == EItemType::EIT_SilverSword || ItemType == EItemType::EIT_LongRangeWeapon || ItemType == EItemType::EIT_WoodenClub)
        return true;
    return false;
}
bool UInventory::IsArmor_Implementation(EItemType ItemType)
{
    if (ItemType == EItemType::EIT_Helmet || ItemType == EItemType::EIT_Clothes || ItemType == EItemType::EIT_Gloves || ItemType == EItemType::EIT_Pants)
        return true;
    return false;
}
int32 UInventory::CalculateResistance_Implementation(FEquipmentInfo& EquipmentInfo)
{
    return FMath::RoundToInt32((25 / (1 + FMath::Pow(2, (-0.025 * (PlayerLevel + (int32)EquipmentInfo.EquipmentQuality - 45))))) * FMath::RandRange(95, 100));
}
int32 UInventory::GetItemMaxStack_Implementation(EItemType ItemType)
{
    switch (ItemType)
    {
    case EItemType::EIT_SteelSword:return 1;
        break;
    case EItemType::EIT_WoodenClub:return 1;
        break;
    case EItemType::EIT_SilverSword:return 1;
        break;
    case EItemType::EIT_Crossbow:return 9999;
        break;
    case EItemType::EIT_LongRangeWeapon:return 1;
        break;
    case EItemType::EIT_Helmet:return 1;
        break;
    case EItemType::EIT_Clothes:return 1;
        break;
    case EItemType::EIT_Gloves:return 1;
        break;
    case EItemType::EIT_Pants:return 1;
        break;
    case EItemType::EIT_Material:return 99;
        break;
    case EItemType::EIT_Draught:return 99;
        break;
    case EItemType::EIT_Food:return 99;
        break;
    case EItemType::EIT_Sundries:return 99;
        break;
    case EItemType::EIT_AlchemyBlueprint:return 99;
        break;
    case EItemType::EIT_CraftingBlueprint:return 99;
        break;
    default:return 1;
        break;
    }
}
int32 UInventory::AddItemByInfo_Implementation(UPARAM(ref)FEquipmentInfo& ItemInfo)
{
    for (auto& Pair : Inventory)
    {
        if (Pair.Value.EquipmentID == ItemInfo.EquipmentID)
        {
            return AddExistingItem(Pair.Value, ItemInfo);
        }
    }
    return AddNewItem(ItemInfo);
}
int32 UInventory::AddNewItem_Implementation(FEquipmentInfo& ItemInfo)
{
    if (FItemDataRow *ItemData=ItemTableMap.Find(ItemInfo.EquipmentID))
    {
        while (ItemInfo.Stack > ItemInfo.MaxStack)
        {
            FEquipmentInfo TempInfo = ItemInfo;
            TempInfo.Stack = TempInfo.MaxStack;
            TempInfo.ItemHandle = UCommonFuction::GetRetainInventoryDataComponnet(this)->GetNewItemHandle();
            TempInfo.EquipmentBurder = ASC->ApplyGameplayEffectToSelf(GEAsset->GE_Entry_Burden->GetDefaultObject<UGameplayEffect>(), ItemData->ItemBurden * 100 * ItemInfo.MaxStack, ASC->MakeEffectContext());
            Inventory.Add(TempInfo.ItemHandle, TempInfo);
            if (Owner && Owner->ActorHasTag(FName(TEXT("Player"))))
            {
                OnInventoryModify.Broadcast(true, TempInfo.ItemHandle, TempInfo);
            }
            ItemInfo.Stack -= ItemInfo.MaxStack;
        }
        ItemInfo.ItemHandle = UCommonFuction::GetRetainInventoryDataComponnet(this)->GetNewItemHandle();
        ItemInfo.EquipmentBurder = ASC->ApplyGameplayEffectToSelf(GEAsset->GE_Entry_Burden->GetDefaultObject<UGameplayEffect>(), ItemData->ItemBurden * 100 * ItemInfo.Stack, ASC->MakeEffectContext());
        Inventory.Add(ItemInfo.ItemHandle, ItemInfo);
        if (Owner && Owner->ActorHasTag(FName(TEXT("Player"))))
        {
            OnInventoryModify.Broadcast(true, ItemInfo.ItemHandle, ItemInfo);
        }
        return ItemInfo.ItemHandle;
    }
    return -1;
}
int32 UInventory::AddExistingItem_Implementation(FEquipmentInfo& ExistingItemInfo, FEquipmentInfo& AddItemInfo)
{
    if (FItemDataRow * ItemData=ItemTableMap.Find(AddItemInfo.EquipmentID))
    {
        FEquipmentInfo& FoundItemInfo = ExistingItemInfo;
        int32 OverFlowStack = AddItemInfo.Stack + FoundItemInfo.Stack - FoundItemInfo.MaxStack;
        if (OverFlowStack <= 0)
        {
            FoundItemInfo.Stack += AddItemInfo.Stack;
            if (FoundItemInfo.EquipmentBurder.IsValid())
            {
                ASC->RemoveActiveGameplayEffect(FoundItemInfo.EquipmentBurder);
            }
            FoundItemInfo.EquipmentBurder = ASC->ApplyGameplayEffectToSelf(GEAsset->GE_Entry_Burden->GetDefaultObject<UGameplayEffect>(), ItemData->ItemBurden * 100 * FoundItemInfo.Stack, ASC->MakeEffectContext());
            OnInventoryModify.Broadcast(true, FoundItemInfo.ItemHandle, FoundItemInfo);
            return AddItemInfo.ItemHandle;
        }
        while (OverFlowStack > 0)
        {
            if (OverFlowStack <= FoundItemInfo.MaxStack)
            {
                AddItemInfo.Stack = OverFlowStack;
                AddItemInfo.ItemHandle = UCommonFuction::GetRetainInventoryDataComponnet(this)->GetNewItemHandle();
                AddItemInfo.EquipmentBurder = ASC->ApplyGameplayEffectToSelf(GEAsset->GE_Entry_Burden->GetDefaultObject<UGameplayEffect>(), ItemData->ItemBurden * 100 * AddItemInfo.Stack, ASC->MakeEffectContext());
                Inventory.Add(AddItemInfo.ItemHandle, AddItemInfo);
                OnInventoryModify.Broadcast(true, AddItemInfo.ItemHandle, AddItemInfo);
                OverFlowStack -= AddItemInfo.Stack;
            }
            else
            {
                AddItemInfo.Stack = AddItemInfo.MaxStack;
                AddItemInfo.ItemHandle = UCommonFuction::GetRetainInventoryDataComponnet(this)->GetNewItemHandle();
                AddItemInfo.EquipmentBurder = ASC->ApplyGameplayEffectToSelf(GEAsset->GE_Entry_Burden->GetDefaultObject<UGameplayEffect>(), ItemData->ItemBurden * 100 * AddItemInfo.Stack, ASC->MakeEffectContext());
                Inventory.Add(AddItemInfo.ItemHandle, AddItemInfo);
                OnInventoryModify.Broadcast(true, AddItemInfo.ItemHandle, AddItemInfo);
                OverFlowStack -= AddItemInfo.MaxStack;
            }
        }
        if (OverFlowStack <= 0) 
        {
            FoundItemInfo.Stack -= OverFlowStack;
             if (FoundItemInfo.EquipmentBurder.IsValid())
             {
                 ASC->RemoveActiveGameplayEffect(FoundItemInfo.EquipmentBurder);
             }
            FoundItemInfo.EquipmentBurder = ASC->ApplyGameplayEffectToSelf(GEAsset->GE_Entry_Burden->GetDefaultObject<UGameplayEffect>(), ItemTableMap.Find(FoundItemInfo.EquipmentID)->ItemBurden * 100 * FoundItemInfo.Stack, ASC->MakeEffectContext());
            OnInventoryModify.Broadcast(true, FoundItemInfo.ItemHandle, FoundItemInfo);
            return AddItemInfo.ItemHandle;
        }
       
    }
    return -1;
}
void UInventory::AddMainEntry_Implementation(FEquipmentInfo& EquipmentInfo, const FText& Describe, TSubclassOf<UGameplayEffect> GE, bool IsWeapon)
{
    FEquipmentEntryInfo MainEntryInfo;
    MainEntryInfo.Describe = Describe;
    MainEntryInfo.ID = 0;
    if (IsWeapon)
        MainEntryInfo.Level = FMath::RoundToInt32(100 * FMath::Pow(1.5, PlayerLevel + (int)EquipmentInfo.EquipmentQuality) * (FMath::RandRange(95, 100)) / 100);
    else
        MainEntryInfo.Level = CalculateResistance(EquipmentInfo);
    EquipmentInfo.MainEntry.Add(GE, MainEntryInfo);
}
void UInventory::WearEquipment_Implementation(int32 ItemHandle)
{
    FEquipmentInfo *EquipmentInfo=Inventory.Find(ItemHandle);
    if (Inventory.Contains(ItemHandle)&&UCommonFuction::GetPlayerLevel(this)>=EquipmentInfo->EquipmentLevel)
    {
        for (auto& Pair : EquipmentInfo->MainEntry)
        {
            if (!IsWeapon(EquipmentInfo->EquipmentType))//武器在拿手上时应用
            {
                Pair.Value.GEHandle = ASC->ApplyGameplayEffectToSelf(Pair.Key->GetDefaultObject<UGameplayEffect>(), Pair.Value.Level, ASC->MakeEffectContext());
            }
        }//应用主词条
        for (auto& Pair : EquipmentInfo->SubEntry)
        {
            Pair.Value.GEHandle = ASC->ApplyGameplayEffectToSelf(Pair.Key->GetDefaultObject<UGameplayEffect>(), Pair.Value.Level, ASC->MakeEffectContext());
        }//应用副词条
        if (EquipmentInfo->EquipmentType == EItemType::EIT_SteelSword)
        {
			EquipItemMap.Remove(EItemType::EIT_Crossbow);
        }
        if (EquipmentInfo->EquipmentType == EItemType::EIT_Crossbow)
        {
            EquipItemMap.Remove(EItemType::EIT_SteelSword);
        }
        EquipItemMap.Add(EquipmentInfo->EquipmentType,*EquipmentInfo);
        OnEquipmentModifyEvent.Broadcast(*EquipmentInfo);
    }
}
void UInventory::UnWearEquipment_Implementation(int32 ItemHandle)
{
    FEquipmentInfo*EquipmentInfo=Inventory.Find(ItemHandle);
    if (Inventory.Contains(ItemHandle))
    {
        for (auto& Pair : EquipmentInfo->MainEntry)//清空主词条
        {
            if (Pair.Key == GEAsset->GE_Entry_Attack_Base)continue;
           ASC->RemoveActiveGameplayEffect(Pair.Value.GEHandle);
           Pair.Value.GEHandle.Invalidate();
        }
        for (auto& Pair : EquipmentInfo->SubEntry)//清空副词条
        {
            ASC->RemoveActiveGameplayEffect(Pair.Value.GEHandle);
           Pair.Value.GEHandle.Invalidate();
        }
		FEquipmentInfo Temp = *EquipmentInfo;
		Temp.EquipmentPlayerBPClass = nullptr;
		Temp.EquipmentEnemyBPClass = nullptr;
        EquipItemMap.Remove(EquipmentInfo->EquipmentType);
        OnEquipmentModifyEvent.Broadcast(Temp);
    }
}
void UInventory::InitCharacterEquipmentByID_Implementation(int32 ItemID)
{
    FEquipmentInfo EquipmentInfo=SpawnEquipmentByID(ItemID);
    int32 ItemHandle = AddItemByInfo(EquipmentInfo);
    if (Owner && Owner->ActorHasTag(FName(TEXT("Player"))))
    {
        if (UBackpackGrid* EquipmentGrid = UCommonFuction::SafeGetEquipmentGridByType(this, EquipmentInfo.EquipmentType))
        {
            if (UBackpack_Slot* OwnerSlot = UCommonFuction::SafeGetBackpackSlotByType(this, EquipmentInfo.EquipmentType))
            {
                if (UBackpackGrid**Grid = OwnerSlot->ItemGridMap.Find(ItemHandle))
                {
                    if (UBackpackGrid * OwnerGrid=*Grid)
                    {
                        UnWearEquipment(EquipmentGrid->ItemInfo.ItemHandle);
                        WearEquipment(OwnerGrid->ItemInfo.ItemHandle);
                        OwnerGrid->SwapGridInfo(EquipmentGrid);
                    }
                }
            }
        }
    }
    else if (IsValid(Owner) && Owner->ActorHasTag(FName(TEXT("Enemy"))))
    {
        UnWearEquipment(ItemHandle);
        WearEquipment(ItemHandle);
    }
}

void UInventory::HandleOwnerDeathEvent(const FOnAttributeChangeData& ChangeData)
{
    if (ChangeData.NewValue > 0) return;
    if (!DropDoOnce.Execute())return;
    int32 RandDrop = FMath::RandRange(MinDropItemCount, MaxDropItemCount);
    int32 FinalDropItemCount = FMath::Min(RandDrop, Inventory.Num());
    TArray<int32> InventoryKeys;
    Inventory.GenerateKeyArray(InventoryKeys);
    for (int32 i = 0; i < FinalDropItemCount; ++i)
    {
        int32 RandomArrayIndex = FMath::RandRange(0, InventoryKeys.Num() - 1);
        int32 ItemHandleToDrop = InventoryKeys[RandomArrayIndex];
        if (FEquipmentInfo* ItemToDrop = Inventory.Find(ItemHandleToDrop))
        {
            DropItemInfos.Add(*ItemToDrop);
        }
        Inventory.Remove(ItemHandleToDrop);
        InventoryKeys.RemoveAt(RandomArrayIndex);
    }
}
void UInventory::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

