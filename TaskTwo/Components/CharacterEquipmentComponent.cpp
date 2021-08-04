// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterEquipmentComponent.h"
#include "BaseCharacter.h"


UCharacterEquipmentComponent::UCharacterEquipmentComponent()
{
	AmmoChangeHandler = [this](int32 CurrentAmmo, int32 CurrentAmmoAmount)
	{
		if (OnCurrentWeaponAmmoChangeEvent.IsBound())
			OnCurrentWeaponAmmoChangeEvent.Broadcast(CurrentAmmo, CurrentAmmoAmount);
	};

	AmmoAmountEndedHandler = [this]()
	{
		if (OnWeaponAmmoAmountEnded.IsBound())
			OnWeaponAmmoAmountEnded.Broadcast();
	};
}

void UCharacterEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = StaticCast<ABaseCharacter*>(GetOwner());
	CreateLoadout();
}

void UCharacterEquipmentComponent::CreateLoadout()
{
	if (IsValid(ClassOneHandItem))
		OneHandWeapon = InitItem(ClassOneHandItem, SocketEquippedOneHandItem);

	if (IsValid(ClassTwoHandItem))
		TwoHandWeapon = InitItem(ClassTwoHandItem, SocketEquippedTwoHandItem);

	switch (AutoEquipWeapon)
	{
		case EAnimationItemTypes::EE_OneHand:
			return PickOneHandWeapon();

		case EAnimationItemTypes::EE_TwoHand:
			return PickTwoHandWeapon();

		default:
			break;
	}
}

ARangeWeapon* UCharacterEquipmentComponent::InitItem(FWeaponClass ItemClass, FName SocketName)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = Character.Get();

	ARangeWeapon* Weapon = GetWorld() -> SpawnActor<ARangeWeapon>(ItemClass, SpawnParameters);
	AttachItemTo(Weapon, SocketName);

	return Weapon;
}

void UCharacterEquipmentComponent::AttachItemTo(ARangeWeapon* Item, FName SocketName)
{
	const FAttachmentTransformRules TransformRules = FAttachmentTransformRules::KeepRelativeTransform;
	USkeletalMeshComponent* Mesh = Character -> GetMesh();

	Item -> AttachToComponent(Mesh, TransformRules, SocketName);
}

void UCharacterEquipmentComponent::SubscribeWeaponEvents()
{
	CurrentWeapon -> SetPickUpNow(true);
	CurrentWeapon -> SubscribeOnAmmoAmountEnded(std::move(AmmoAmountEndedHandler));
	CurrentWeapon -> SubscribeOnChangeAmmo(std::move(AmmoChangeHandler));

	if (OnEquipWeapon.IsBound())
		OnEquipWeapon.Broadcast(true);
}

void UCharacterEquipmentComponent::UnsubscribeWeaponEvents()
{
	CurrentWeapon -> SetPickUpNow(false);
	CurrentWeapon -> UnsubscribeAmmoAmountEnded();
	CurrentWeapon -> UnsubscribeOnChangeAmmo();
	
	if (OnEquipWeapon.IsBound())
		OnEquipWeapon.Broadcast(false);
}

void UCharacterEquipmentComponent::BeginFire()
{
	if (CurrentWeapon.IsValid())
		CurrentWeapon -> BeginFire();
}

void UCharacterEquipmentComponent::EndFire()
{
	if (CurrentWeapon.IsValid())
		CurrentWeapon -> EndFire();
}

void UCharacterEquipmentComponent::RemoveWeapon()
{
	if (CurrentWeaponType == EAnimationItemTypes::EE_None) return;

	const FName SocketName = CurrentWeaponType == EAnimationItemTypes::EE_OneHand ?
		SocketEquippedOneHandItem : SocketEquippedTwoHandItem;
	CurrentWeaponType = EAnimationItemTypes::EE_None;
	
	AttachItemTo(CurrentWeapon.Get(), SocketName);
	UnsubscribeWeaponEvents();
	
	CurrentWeapon = nullptr;
}

void UCharacterEquipmentComponent::PickOneHandWeapon()
{
	if (CurrentWeaponType == EAnimationItemTypes::EE_OneHand || !OneHandWeapon.IsValid()) return;
		
	if (CurrentWeapon.IsValid())
	{
		TwoHandWeapon -> SetPickUpNow(false);
		AttachItemTo(TwoHandWeapon.Get(), SocketEquippedTwoHandItem);
	}

	CurrentWeaponType = EAnimationItemTypes::EE_OneHand;
	CurrentWeapon = OneHandWeapon;

	OneHandWeapon -> SetPickUpNow(true);
	AttachItemTo(OneHandWeapon.Get(), SocketPickUpItem);
	
	SubscribeWeaponEvents();
}

void UCharacterEquipmentComponent::PickTwoHandWeapon()
{
	if (CurrentWeaponType == EAnimationItemTypes::EE_TwoHand || !TwoHandWeapon.IsValid()) return;
		
	if (CurrentWeapon.IsValid())
	{
		OneHandWeapon -> SetPickUpNow(false);
		AttachItemTo(OneHandWeapon.Get(), SocketEquippedOneHandItem);
	}
	
	CurrentWeaponType = EAnimationItemTypes::EE_TwoHand;
	CurrentWeapon = TwoHandWeapon;

	TwoHandWeapon -> SetPickUpNow(true);
	AttachItemTo(TwoHandWeapon.Get(), SocketPickUpItem);
	
	SubscribeWeaponEvents();
}
