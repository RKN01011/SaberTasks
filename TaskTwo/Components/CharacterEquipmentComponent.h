// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "BaseCharacter.h"
#include "Components/ActorComponent.h"
#include "Weapons/RangeWeapon.h"

#include "CharacterEquipmentComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FWeaponAmmoAmountEnded)

class AProjectile;
enum class EAnimationItemTypes : uint8;

using FWeaponClass = TSubclassOf<ARangeWeapon>;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCurrentWeaponAmmoChange, int32, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPickUpWeapon, bool);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CHARACTERS_API UCharacterEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCharacterEquipmentComponent();
	
	FOnCurrentWeaponAmmoChange OnCurrentWeaponAmmoChangeEvent;

	FWeaponAmmoAmountEnded OnWeaponAmmoAmountEnded;

	FOnPickUpWeapon OnEquipWeapon;

	void BeginFire();

	void EndFire();

	void RemoveWeapon();

	void PickOneHandWeapon();

	void PickTwoHandWeapon();

	bool HasEquipWeapon() const
	{ return CurrentWeapon.IsValid(); }
	
	ARangeWeapon* GetCurrentEquipWeapon() const
	{ return CurrentWeapon.Get(); }
	
protected:
	virtual void BeginPlay() override;

	void CreateLoadout();

	void SubscribeWeaponEvents();

	void UnsubscribeWeaponEvents();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Loadout")
	TSubclassOf<ARangeWeapon> ClassOneHandItem;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Loadout")
	TSubclassOf<ARangeWeapon> ClassTwoHandItem;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Loadout")
	EAnimationItemTypes AutoEquipWeapon = EAnimationItemTypes::EE_None;

private:
	TWeakObjectPtr<ABaseCharacter> Character = nullptr;

	TWeakObjectPtr<ARangeWeapon> OneHandWeapon = nullptr;
	
	TWeakObjectPtr<ARangeWeapon> TwoHandWeapon = nullptr;
	
	TWeakObjectPtr<ARangeWeapon> CurrentWeapon = nullptr;

	EAnimationItemTypes CurrentWeaponType = EAnimationItemTypes::EE_None;

	void AttachItemTo(ARangeWeapon* Item, FName SocketName);
	
	ARangeWeapon* InitItem(FWeaponClass ItemClass, FName SocketName);

	FAmmoChangeHandler AmmoChangeHandler;
	FAmmoAmountEndedHandler AmmoAmountEndedHandler;
};
