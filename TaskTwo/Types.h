#pragma once

#define ECC_Climbing ECC_GameTraceChannel1
#define ECC_Bullet ECC_GameTraceChannel5

const FName FXParamTraceEnd = FName{"TraceEnd"}; 
const FName SocketPickUpItem = FName{"PickUpItemSocket"};
const FName SocketEquippedOneHandItem = FName{"EquippedOneHandItemSocket"};
const FName SocketEquippedTwoHandItem = FName{"EquippedTwoHandItemSocket"};

const FName SocketWeaponMuzzle = FName{"MuzzleSocket"};
const FName SocketFroGrip = FName{"FroGripSocket"};

const FName BB_IsAmmoEnded = FName{"IsAmmoEnded"};
const FName BB_CurrentTarget = FName{"CurrentTarget"};
const FName BB_NextLocation = FName{"NextLocation"};
const FName BB_EscapeLocation = FName{"EscapeLocation"};

using FAmmoChangeHandler = std::function<void (int32, int32)>;
using FAmmoAmountEndedHandler = std::function<void ()>;

UENUM(BlueprintType)
enum class ETeams : uint8
{
	Player, FrendlyNPC, EnemyNPC
};

UENUM(BlueprintType)
enum class EAnimationItemTypes : uint8
{
	EE_None UMETA(DisplayName = "None"),
	EE_OneHand UMETA(DisplayName = "OneHand"),
	EE_TwoHand UMETA(DisplayName = "TwoHand")
};

UENUM(BlueprintType)
enum class EEquippedItemFireMode: uint8
{
	EE_None UMETA(DisplayName = "No shot"),
	EE_Single UMETA(DisplayName = "Single"),
	EE_FullAuto UMETA(DisplayName = "FullAuto")
};