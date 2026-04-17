#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include"Struct/InventoryStruct.h"
#include"Data/EntryTextData.h"
#include "GAS/AllGamePlayEffectAsset.h"
#include"Table/ItemDataRow.h"
#include"Table/EntryDataRow.h"
#include"Macro/Macro.h"
#include "Inventory.generated.h"
class UAbilitySystemComponent;
class ICharacterInterface;
struct FItemDataRow;
struct FEntryDataRow;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnInventoryModify, bool,IsAdd,int32,ItemHandle,FEquipmentInfo,ItemInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquipmentModify,FEquipmentInfo, ItemInfo);
UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WITCHER3_API UInventory : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventory();
	void HandleOwnerDeathEvent(const FOnAttributeChangeData& ChangeData);
	FDoOnce DropDoOnce;
	ICharacterInterface* CharacterInterface;
	UPROPERTY(BlueprintAssignable)
	FOnInventoryModify OnInventoryModify;
	UPROPERTY(BlueprintAssignable)
	FOnEquipmentModify OnEquipmentModifyEvent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	UAbilitySystemComponent* ASC;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reference")
	AActor* Owner;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UDataTable* ItemDataTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TMap<int32, FItemDataRow>ItemTableMap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TMap<int32, FEquipmentInfo>Inventory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TMap<EItemType, FEquipmentInfo>EquipItemMap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entry")
	UDataTable* EntryDataTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entry")
	TMap<int32, FEntryDataRow>EntryTableMap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entry")
	TMap<int32, FEntryDataRow>LevelOne_EntryTableMap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entry")
	TMap<int32, FEntryDataRow>LevelTwo_EntryTableMap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entry")
	TMap<int32, FEntryDataRow>LevelThree_EntryTableMap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entry")
	TArray<int32>LevelOneEntry_IDArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entry")
	TArray<int32>LevelTwoEntry_IDArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entry")
	TArray<int32>LevelThreeEntry_IDArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo")
	int32 PlayerLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GEAsset")
	UAllGamePlayEffectAsset* GEAsset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GEAsset")
	UEntryTextData* EntryTextData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxDropItemCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MinDropItemCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FEquipmentInfo>DropItemInfos;
	/*
	FUNCTION
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsWeapon(EItemType ItemType);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsArmor(EItemType ItemType);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void WearEquipment(int32 ItemHandle);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UnWearEquipment(int32 ItemHandle);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int32 AddItemByInfo(UPARAM(ref)FEquipmentInfo& ItemInfo);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int32 AddExistingItem(FEquipmentInfo &ExistingItemInfo,FEquipmentInfo &AddItemInfo);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int32 AddNewItem(FEquipmentInfo& ItemInfo);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void InitCharacterEquipmentByID(int32 ItemID);
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void InitInventory();
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void InitEntryData();
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void InitItemData();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FEquipmentInfo SpawnEquipmentByID(int32 ID,int32 Count=1);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ItemDataToEquipmentData(FEquipmentInfo& EquipmentInfo, FItemDataRow &ItemData);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void CopyItemBaseInfoToEquipment(FEquipmentInfo& EquipmentInfo, FItemDataRow& ItemData);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AddEquipmentMainEntry(FEquipmentInfo& EquipmentInfo);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AddEquipmentSubEntry(FItemDataRow& ItemData, FEquipmentEntryInfo& SubEntryInfo, FEquipmentInfo& EquipmentInfo);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int32 CalculateResistance(FEquipmentInfo& EquipmentInfo);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int32 GetItemMaxStack(EItemType ItemType);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AddMainEntry(FEquipmentInfo& EquipmentInfo, const FText& Describe, TSubclassOf<UGameplayEffect> GE,bool IsWeapon);
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
