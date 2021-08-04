// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "NiagaraCommon.h"
#include "Components/SceneComponent.h"
#include "Weapons/Projectiles/Projectile.h"

#include "WeaponBarrelComponent.generated.h"

class AProjectile;
UENUM(BlueprintType)
enum class FShotType : uint8 { HitScan, Projectile };

USTRUCT(BlueprintType)
struct FDecalInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Decal info")
	UMaterialInterface* DecalMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Decal info")
	FVector DecalSize = FVector{5.f, 5.f, 5.f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Decal info")
	float DecalLifeTime = 10.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Decal info")
	float DecalFadeOutTime = 5.f;
};

USTRUCT(BlueprintType)
struct FShotInfo
{
	GENERATED_BODY()

	FShotInfo() : Location_Mul_10(FVector::ZeroVector), Direction(FVector::ZeroVector) {};
	
	FShotInfo(FVector L, FVector D) : Location_Mul_10(L * 10.f), Direction(D) {};

	UPROPERTY()
	FVector_NetQuantize100 Location_Mul_10;

	UPROPERTY()
	FVector_NetQuantizeNormal Direction;

	FVector GetLocation() const
	{ return Location_Mul_10 * .1f; };
	
	FVector GetDirection() const
	{ return Direction; };
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CHARACTERS_API UWeaponBarrelComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	void Shot(FVector ShotStart, FRotator ShotRotation, float SpreadAngle);
	
	void TakeDamage(FVector ShotVector, FHitResult ShotResult);

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Barell attributes", meta=(ClampMin = 1, UIMin = 1, ClampMax = 8, UIMax = 8))
	int32 BulletsPerShot = 1;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Barell attributes")
	float FiringRange = 10000.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Barell attributes | Damage")
	float DamageAmount = 20.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Barell attributes | VFX")
	UNiagaraSystem* MuzzleFlashFX = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Barell attributes | VFX")
	UNiagaraSystem* TraceFX = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Barell attributes | Decals")
	FDecalInfo DecalInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Barell attributes | Damage")
	TSubclassOf<UDamageType> DamageTypeClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Barell attributes | ShotType")
	FShotType ShotType = FShotType::HitScan;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Barell attributes | ShotType",
		meta=(EditCondition = "ShotType == FShotType::Projectile"))
	TSubclassOf<AProjectile> ProjectileBulletClass = nullptr;

	FBulletHitHandler BulletHitHandler;
	

private:
	FVector GetBulletSpreadOffset(float Angle, FRotator ShotRotation) const;
	
	void SpawnDecal(FHitResult ShotResult);

	void HitScan(const FShotInfo&& ShotInfo);

	void ProjectileLaunch(const FShotInfo&& ShotInfo);

	void ShotInternal(const TArray<FShotInfo>& ShotsInfo);

	TArray<FShotInfo> LastShotsInfo;
};
