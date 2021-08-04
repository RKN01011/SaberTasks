// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Types.h"
#include "CoreMinimal.h"

#include "RangeWeapon.generated.h"


class ABaseCharacter;
class UAnimMontage;

/**
 * 
 */
UCLASS(Blueprintable)
class CHARACTERS_API ARangeWeapon : public AActor
{
	GENERATED_BODY()

public:
	ARangeWeapon();

	virtual void SetOwner(AActor* NewOwner) override;

	void SubscribeOnChangeAmmo(FAmmoChangeHandler&& AmmoChangeHandler)
	{ (OnAmmoChangeHandler = std::move(AmmoChangeHandler))(CurrentAmmo, AmmoAmount); };
	void UnsubscribeOnChangeAmmo()
	{ OnAmmoChangeHandler = nullptr; };

	void SubscribeOnAmmoAmountEnded(FAmmoAmountEndedHandler&& AmmoAmountEndedHandler)
	{ OnAmmoAmountEnded = std::move(AmmoAmountEndedHandler); };
	void UnsubscribeAmmoAmountEnded()
	{ OnAmmoAmountEnded = nullptr; };
	
	void SetPickUpNow(bool Value)
	{ bPickUp = Value; };
	
	EAnimationItemTypes GetAnimationType() const
	{ return AnimationType; }

	EEquippedItemFireMode GetFireMode() const
	{ return FireMode; };
	
	int32 GetAmmo() const
	{ return CurrentAmmo; };

	void SetAmmo(int32 Ammo);

	void BeginFire();

	void EndFire();

	void Reload();
	
	bool IsReloading() const
	{ return bIsReloading; };

	bool IsFiring() const
	{ return bIsFiring; };

	FTransform GetForeGripTransform() const;

protected:
	virtual void BeginPlay() override;

	bool CanShot() const
	{ return CurrentAmmo > 0; };

	void Shot();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	class UWeaponBarrelComponent* WeaponBarrel = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	class USkeletalMeshComponent* Mesh = nullptr;
	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animations | Weapon")
	UAnimMontage* WeaponFireMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animations | Weapon")
	UAnimMontage* WeaponReloadMontage = nullptr;
	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item parametrs | Animation")
	EAnimationItemTypes AnimationType = EAnimationItemTypes::EE_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animations | Charter")
	UAnimMontage* CharterReloadMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animations | Charter")
	UAnimMontage* CharterAimingReloadMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animations | Charter")
	UAnimMontage* CharterFireMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animations | Charter")
	UAnimMontage* CharterAimingFireMontage = nullptr;
	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Fire parametrs")
	EEquippedItemFireMode FireMode = EEquippedItemFireMode::EE_None;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Fire parametrs", meta=(ClampMin = 1.f, UMin = 1.f))
	float RateOfFire = 600.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Fire parametrs", meta=(ClampMin = 1, UMin = 1, ClampMax = 120, UMax = 120))
	int32 MaxAmmo = 30;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Fire parametrs", meta=(ClampMin = 0, UMin = 0))
	int32 AmmoAmount = 180;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Fire parametrs", meta=(ClampMin = 0.f, UMin = 0.f, ClampMax = 2.f, UMax = 2.f))
	float SpreadAngle = 1.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Fire parametrs", meta=(ClampMin = 0.f, UMin = 0.f, ClampMax = 0.5f, UMax = 0.5f))
	float AimSpreadAngle = .5f;

private:
	TWeakObjectPtr<ABaseCharacter> Character = nullptr;

	bool bPickUp = false;
	
	bool bIsReloading = false;
	
	bool bIsFiring = false;
	
	FTimerHandle ShotTimer;
	
	FTimerHandle ReloadTimer;
	
	int32 CurrentAmmo = 0;
	
	FAmmoChangeHandler OnAmmoChangeHandler;
	FAmmoAmountEndedHandler OnAmmoAmountEnded;
	
	float GetFireInterval() const
	{ return 60.f / RateOfFire; };
	
	float PlayAnimMontage(UAnimMontage* AnimMontage);
	
	float GetSpreadAngle() const;
	
	UFUNCTION()
	void DoReload();
};
