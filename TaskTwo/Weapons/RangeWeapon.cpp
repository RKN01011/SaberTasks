// Fill out your copyright notice in the Description page of Project Settings.


#include "RangeWeapon.h"

#include "BaseCharacter.h"
#include "Types.h"
#include "Weapons/Components/WeaponBarrelComponent.h"

#define RELOAD()\
	if (CurrentAmmo == MaxAmmo || AmmoAmount <= 0 || bIsReloading) return;\
	\
	UAnimMontage* ReloadMontage = Character -> IsAiming() ? CharterAimingReloadMontage : CharterReloadMontage;\
	if (IsValid(ReloadMontage))\
	{\
		bIsReloading = true;\
		const float DurationReload = Character -> PlayAnimMontage(ReloadMontage);\
		\
		PlayAnimMontage(WeaponReloadMontage);\
		GetWorld() -> GetTimerManager().SetTimer(ReloadTimer, this, &ARangeWeapon::DoReload,\
			DurationReload, false);\
	}

ARangeWeapon::ARangeWeapon()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponRoot"));
	
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	Mesh -> SetupAttachment(RootComponent);

	WeaponBarrel = CreateDefaultSubobject<UWeaponBarrelComponent>(TEXT("WeaponBarrel"));
	WeaponBarrel -> SetupAttachment(Mesh, SocketWeaponMuzzle);
}


void ARangeWeapon::SetOwner(AActor* NewOwner)
{
	Super::SetOwner(NewOwner);

	if (IsValid(NewOwner))
	{
		Character = StaticCast<ABaseCharacter*>(GetOwner());
	}
	else
		Character = nullptr;
}

void ARangeWeapon::BeginPlay()
{
	Super::BeginPlay();
	CurrentAmmo = MaxAmmo;
}


void ARangeWeapon::Shot()
{
	if (!CanShot())
	{
		if (AmmoAmount == 0 && OnAmmoAmountEnded != nullptr)
		{
			OnAmmoAmountEnded();
		}
		else
		{
			RELOAD();
		}

		return EndFire();
	};

	FVector ViewPoint;
	FRotator ViewRotation;;
	if (Character -> IsPlayerControlled())
	{
		APlayerController* Controller = Character -> GetController<APlayerController>();
		Controller -> GetPlayerViewPoint(ViewPoint, ViewRotation);
	}
	else
	{
		ViewPoint = WeaponBarrel -> GetComponentLocation();
		ViewRotation = Character -> GetBaseAimRotation();
	}

	Character -> PlayAnimMontage(Character -> IsAiming() ? CharterAimingFireMontage : CharterFireMontage);
	PlayAnimMontage(WeaponFireMontage);

	SetAmmo(CurrentAmmo - 1);
	WeaponBarrel -> Shot(ViewPoint, ViewRotation, GetSpreadAngle());
}

void ARangeWeapon::DoReload()
{
	const int32 AmmoToReload = MaxAmmo - CurrentAmmo;
	const int32 ReloadedAmmo = FMath::Min(AmmoAmount, AmmoToReload);

	GEngine -> AddOnScreenDebugMessage(1, 2.f, FColor::Cyan, FString::Printf(TEXT("%i"), ReloadedAmmo));
	bIsReloading = false;
	AmmoAmount -= ReloadedAmmo;
	SetAmmo(ReloadedAmmo + CurrentAmmo);
}

float ARangeWeapon::PlayAnimMontage(UAnimMontage* AnimMontage)
{
	UAnimInstance* WeaponAnimInstance = Mesh -> GetAnimInstance();
	return WeaponAnimInstance -> Montage_Play(AnimMontage);
}

FTransform ARangeWeapon::GetForeGripTransform() const
{
	return Mesh -> GetSocketTransform(SocketFroGrip);
}

void ARangeWeapon::SetAmmo(int32 Ammo)
{	
	CurrentAmmo = Ammo;
	
	if (OnAmmoChangeHandler != nullptr)
		OnAmmoChangeHandler(Ammo, AmmoAmount);
}

void ARangeWeapon::BeginFire()
{
	if (FireMode == EEquippedItemFireMode::EE_None || bIsReloading) return;

	if (FireMode == EEquippedItemFireMode::EE_Single)
	{
		Shot();
		bIsFiring = true;
	}

	else
	{
		GetWorld() -> GetTimerManager().ClearTimer(ShotTimer);
		GetWorld() -> GetTimerManager().SetTimer(ShotTimer, this, &ARangeWeapon::Shot, GetFireInterval(), true);
		bIsFiring = true;
	}
}

void ARangeWeapon::EndFire()
{
	bIsFiring = true;
	
	if (FireMode == EEquippedItemFireMode::EE_None || FireMode == EEquippedItemFireMode::EE_Single)
		return;

	if (FireMode == EEquippedItemFireMode::EE_FullAuto)
		GetWorld() -> GetTimerManager().ClearTimer(ShotTimer);
}

float ARangeWeapon::GetSpreadAngle() const
{
	const float Angle = FMath::DegreesToRadians(Character -> IsAiming() ? AimSpreadAngle : SpreadAngle);
	return FMath::RandRange(.0f, Angle);
}

void ARangeWeapon::Reload()
{
	RELOAD();
}
