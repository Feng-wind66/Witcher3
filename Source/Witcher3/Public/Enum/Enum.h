#pragma once
#include "CoreMinimal.h"
#include "Enum.generated.h"
UENUM(BlueprintType)
enum class EActionState :uint8
{
    EAS_OutOfCombat UMETA(DisplayName = "OutOfCombat"),
    EAS_InCombat UMETA(DisplayName = "InCombat"),
    EAS_InAir UMETA(DisplayName = "InAir"),
    EAS_CombatFinisher_Attacker UMETA(DisplayName = "CombatFinisher_Attacker"),
    EAS_CombatFinisher_Receiver UMETA(DisplayName = "CombatFinisher_Receiver"),
    EAS_InUIInteraction_Dialogue UMETA(DisplayName = "InUIInteraction_Dialogue"),
    EAS_Dialogue UMETA(DisplayName = "Dialogue"),
    EAS_Death UMETA(DisplayName = "Death"),
    EAS_InUIMenu UMETA(DisplayName = "InUIMenu"),
    EAS_InUIInteraction_Loot UMETA(DisplayName = "InUIInteraction_Loot"),
    EAS_InUIInteraction_MountHorse UMETA(DisplayName = "InUIInteraction_MountHorse"),
    EAS_Rading UMETA(DisplayName = "Rading"),
};
UENUM(BlueprintType)
enum class ELocomotionState :uint8
{
    ELS_Walk UMETA(DisplayName = "Walk"),
    ELS_Run UMETA(DisplayName = "Run"),
    ELS_Sprint UMETA(DisplayName = "Sprint"),
};
UENUM(BlueprintType)
enum class EOwnerType :uint8
{
    EOT_Player UMETA(DisplayName = "Player"),
    EOT_NPC UMETA(DisplayName = "NPC"),
    EOT_Dragon  UMETA(DisplayName = "Dragon"),
};
UENUM(BlueprintType)
enum class EMonsterType :uint8
{
    EMT_Dragon  UMETA(DisplayName = "Dragon"),
    EMT_Soldier UMETA(DisplayName = "NPC"),
};
UENUM(BlueprintType)
enum class EItemQuality :uint8
{
    EIQ_Ordinary  UMETA(DisplayName = "Ordinary"),
    EIQ_Rare UMETA(DisplayName = "Rare"),
    EIQ_Exquisite UMETA(DisplayName = "Exquisite"),
    EIQ_Epic UMETA(DisplayName = "Epic"),
    EIQ_UniqueLegend UMETA(DisplayName = "UniqueLegend"),
};
UENUM(BlueprintType)
enum class EItemType :uint8
{
    EIT_SteelSword  UMETA(DisplayName = "SteelSword"),
    EIT_WoodenClub UMETA(DisplayName = "WoodenClub"),
    EIT_SilverSword UMETA(DisplayName = "SilverSword"),
    EIT_Crossbow UMETA(DisplayName = "Crossbow"),
    EIT_LongRangeWeapon UMETA(DisplayName = "LongRangeWeapon"),
    EIT_Helmet UMETA(DisplayName = "Helmet"),
    EIT_Clothes UMETA(DisplayName = "Clothes"),
    EIT_Gloves UMETA(DisplayName = "Gloves"),
    EIT_Pants UMETA(DisplayName = "Pants"),
    EIT_Material UMETA(DisplayName = "Material"),
    EIT_Draught UMETA(DisplayName = "Draught"),//药水
    EIT_Food UMETA(DisplayName = "Food"),
    EIT_Sundries UMETA(DisplayName = "Sundries"),//杂物
    EIT_AlchemyBlueprint UMETA(DisplayName = "AlchemyBlueprint"),
    EIT_CraftingBlueprint UMETA(DisplayName = "CraftingBlueprint"),
};
UENUM(BlueprintType)
enum class EEquipmentCategory :uint8
{
    EEC_None UMETA(DisplayName = "None"),
    EEC_FirstWeaponSlot UMETA(DisplayName = "FirstWeaponSlot"),
    EEC_SilverSword UMETA(DisplayName = "SilverSword"),
    EEC_Crossbow UMETA(DisplayName = "Crossbow"),
    EEC_LongRangeWeapon UMETA(DisplayName = "LongRangeWeapon"),
    EEC_Helmet UMETA(DisplayName = "Helmet"),
    EEC_Clothes UMETA(DisplayName = "Clothes"),
    EEC_Gloves UMETA(DisplayName = "Gloves"),
    EEC_Pants UMETA(DisplayName = "Pants"),
    EEC_Consumable UMETA(DisplayName = "Consumable"),
    //EEC_HorseEquipment
    //EEC_UniqueItem
};
UENUM(BlueprintType)
enum class EColor :uint8
{
    EC_White  UMETA(DisplayName = "White"),
    EC_Green UMETA(DisplayName = "Green"),
    EC_Blue UMETA(DisplayName = "Blue"),
    EC_Purple UMETA(DisplayName = "Purple"),
    EC_Golden UMETA(DisplayName = "Golden"),
};
UENUM(BlueprintType)
enum class EGridScene :uint8
{
    EGS_InTransaction_Trader  UMETA(DisplayName = "InTransaction_Trader"),//交易
    EGS_InTransaction_Buyer  UMETA(DisplayName = "InTransaction_Buyer"),//交易
    EGS_InInventory UMETA(DisplayName = "InInventory"),
    EGS_InEquipmentSlot UMETA(DisplayName = "InEquipmentSlot"),
    EGS_InSpoils UMETA(DisplayName = "InSpoils"),
};
UENUM(BlueprintType)
enum class EMenuType :uint8
{
    EMT_MainMenu  UMETA(DisplayName = "MainMenu"),//交易
    EMT_Workmanship  UMETA(DisplayName = "Workmanship"),//交易
    EMT_Alchemy UMETA(DisplayName = "Alchemy"),
    EMT_Inventory UMETA(DisplayName = "Inventory"),
    EMT_Map UMETA(DisplayName = "Map"),
    EMT_Task UMETA(DisplayName = "Task"),
    EMT_Meditation UMETA(DisplayName = "Meditation"),
    EMT_Close UMETA(DisplayName = "Close"),
};
UENUM(BlueprintType)
enum class EDamageSource :uint8
{
    EDS_PrimaryWeapon  UMETA(DisplayName = "PrimaryWeapon"),
    EDS_SecondaryWeapon  UMETA(DisplayName = "SecondaryWeapon"),
    EDS_Crossbow UMETA(DisplayName = "Crossbow"),
    EDS_Magic UMETA(DisplayName = "Magic"),
    EDS_Item UMETA(DisplayName = "Item"),
    EDS_Environment UMETA(DisplayName = "Environment"),
    EDS_Monster UMETA(DisplayName = "Monster"),
};
UENUM(BlueprintType)
enum class EPriority :uint8
{
    EP_NotApplicable=0  UMETA(DisplayName = "NotApplicable"),
    EP_VeryLow=1  UMETA(DisplayName = "VeryLow"),
    EP_Low=2 UMETA(DisplayName = "Low"),
    EP_Medium=3 UMETA(DisplayName = "Medium"),
    EP_High=4 UMETA(DisplayName = "High"),
    EP_VeryHigh=5 UMETA(DisplayName = "VeryHigh"),
    EP_Supreme=6 UMETA(DisplayName = "Supreme"),
};
UENUM(BlueprintType)
enum class EWhichMotionWarping :uint8
{
    EWMW_MeleeAttack  UMETA(DisplayName = "MeleeAttack"),
    EWMW_Hit  UMETA(DisplayName = "Hit"),
    EWMW_NoWarping UMETA(DisplayName = "NoWarping"),
    EWMW_Dodge UMETA(DisplayName = "Dodge"),
    EWMW_Roll UMETA(DisplayName = "Roll"),
    EWMW_MountHorse_L UMETA(DisplayName = "MountHorse_L"),
    EWMW_MountHorse_R UMETA(DisplayName = "MountHorse_R"),
    EWMW_MountHorse_B UMETA(DisplayName = "MountHorse_B"),
    EWMW_MountHorse_FrontR UMETA(DisplayName = "MountHorse_FrontR"),
    EWMW_MountHorse_FrontL UMETA(DisplayName = "MountHorse_FrontL"),
    EWMW_Dismount_L UMETA(DisplayName = "Dismount_L"),
    EWMW_Dismount_R UMETA(DisplayName = "Dismount_R"),
    EWMW_DragonRotation UMETA(DisplayName = "DragonRotation"),
    EWMW_DailyLifeAction UMETA(DisplayName = "DailyLifeAction"),
};
UENUM(BlueprintType)
enum class EWarpWhat :uint8
{
    EWW_LocationOnly  UMETA(DisplayName = "LocationOnly"),
    EWW_RotationOnly  UMETA(DisplayName = "RotationOnly"),
    EWW_Both UMETA(DisplayName = "Both"),
    EWW_Both_RotateOnce UMETA(DisplayName = "Both_RotateOnce"),
    EWW_Both_ReverseTotateOnce UMETA(DisplayName = "Both_ReverseTotateOnce"),
};
UENUM(BlueprintType)
enum class EWeaponType :uint8
{
    EWT_LongSword  UMETA(DisplayName = "LongSword"),
    EWT_OneHandedSword  UMETA(DisplayName = "OneHandedSword"),
    EWT_Club UMETA(DisplayName = "Club"),
    EWT_Crossbow_RotateOnce UMETA(DisplayName = "Crossbow_RotateOnce"),
};
UENUM(BlueprintType)
enum class EDodgeType :uint8
{
	EDT_Left  UMETA(DisplayName = "Left"),
	EDT_Right  UMETA(DisplayName = "Right"),
};
UENUM(BlueprintType)
enum class EWhichDamageType :uint8
{
    EWDT_MeleeWeapon UMETA(DisplayName = "MeleeWeapon"),
    EWDT_RangedWeapon UMETA(DisplayName = "RangedWeapon"),
    EWDT_Magic_Igni UMETA(DisplayName = "Magic_Igni"),
    EWDT_Magic_Quen UMETA(DisplayName = "Magic_Quen"),
    EWDT_Magic_Aard UMETA(DisplayName = "Magic_Aard"),
    EWDT_Horse UMETA(DisplayName = "Horse"),
    EWDT_Monster UMETA(DisplayName = "Monster"),
};
UENUM(BlueprintType)
enum class EMagicType :uint8
{
   EMT_None UMETA(DisplayName = "None"),
   EMT_Aard UMETA(DisplayName = "Aard"),
   EMT_Axii UMETA(DisplayName = "Axii"),
   EMT_Igni UMETA(DisplayName = "Igni"),
   EMT_Quen UMETA(DisplayName = "Quen"),
   EMT_Yrden UMETA(DisplayName = "Yrden"),
};