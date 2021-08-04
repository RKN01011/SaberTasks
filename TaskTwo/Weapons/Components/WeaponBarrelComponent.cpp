// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponBarrelComponent.h"

#include "DrawDebugHelpers.h"
#include "Types.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Niagara/Public/NiagaraFunctionLibrary.h"
#include "Niagara/Public/NiagaraComponent.h"
void UWeaponBarrelComponent::BeginPlay()
{
	Super::BeginPlay();
	
	BulletHitHandler = [this](const FHitResult& HitResult, const FVector& Direction)
		{ TakeDamage(Direction, HitResult); SpawnDecal(HitResult); };
}

void UWeaponBarrelComponent::Shot(FVector ShotStart, FRotator ShotRotation, float SpreadAngle)
{
	TArray<FShotInfo> Result;
	for (int i = 0; i < BulletsPerShot; i++)
		Result.Emplace(
			ShotStart, ShotRotation.RotateVector(FVector::ForwardVector) +
						  GetBulletSpreadOffset(SpreadAngle, ShotRotation)
		);

	ShotInternal(Result);
}

void UWeaponBarrelComponent::ShotInternal(const TArray<FShotInfo>& ShotsInfo)
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), MuzzleFlashFX,
	GetComponentLocation(), GetComponentRotation());
	
	for (auto SI : ShotsInfo)
		switch (ShotType)
		{
			case FShotType::Projectile:
				{ ProjectileLaunch(std::move(SI)); break; };
					
			default:
				HitScan(std::move(SI));
		}
}

void UWeaponBarrelComponent::HitScan(const FShotInfo&& ShotInfo)
{
	const FVector ShotStart = ShotInfo.GetLocation();
	const FVector ShotVector = ShotInfo.GetDirection();
	const FVector ShotEnd = ShotStart + FiringRange * ShotVector;

	FHitResult ShotResult;
	if (GetWorld() -> LineTraceSingleByChannel(ShotResult, ShotStart, ShotEnd, ECC_Bullet))
		BulletHitHandler(ShotResult, ShotVector);
		
	
	const FVector MuzzleLocation = GetComponentLocation();
	UNiagaraComponent* TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX,
		MuzzleLocation, GetComponentRotation());
	
	TraceFXComponent -> SetVectorParameter(FXParamTraceEnd, ShotResult.ImpactPoint);
}

void UWeaponBarrelComponent::ProjectileLaunch(const FShotInfo&& ShotInfo)
{
	const FVector ShotStart = ShotInfo.GetLocation();
	const FVector ShotVector = ShotInfo.GetDirection();
	AProjectile* Projectile = GetWorld() -> SpawnActor<AProjectile>(ProjectileBulletClass,
		ShotStart, ShotVector.ToOrientationRotator());

	checkf(IsValid(ProjectileBulletClass), TEXT("Projectile not spawned"));
	Projectile -> LaunchProjectile(ShotVector, BulletHitHandler);
}

FVector UWeaponBarrelComponent::GetBulletSpreadOffset(float Angle, FRotator ShotRotation) const
{
	float SpreadSize = FMath::Tan(Angle);
	float RotationAngle = FMath::RandRange(0.f, 2 * PI);

	float SpreedZ = FMath::Sin(RotationAngle);
	float SpreedY = FMath::Cos(RotationAngle);

	FVector Result = ShotRotation.RotateVector(FVector::UpVector) * SpreedZ;
	Result += ShotRotation.RotateVector(FVector::RightVector) * SpreedY;
	Result *= SpreadSize;

	return Result;
}

void UWeaponBarrelComponent::TakeDamage(FVector ShotVector, FHitResult ShotResult)
{
	APlayerController* Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	AActor* HitActor = ShotResult.Actor.Get();
	
	if (IsValid(HitActor))
	{
		FPointDamageEvent DamageEvent;
		DamageEvent.HitInfo = ShotResult;
		DamageEvent.ShotDirection = ShotVector;
		DamageEvent.DamageTypeClass = DamageTypeClass;
		HitActor -> TakeDamage(DamageAmount, DamageEvent, Controller, Controller -> GetOwner());
	}
}

void UWeaponBarrelComponent::SpawnDecal(FHitResult ShotResult)
{
	UDecalComponent* DecalComponent = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), DecalInfo.DecalMaterial,
		DecalInfo.DecalSize, ShotResult.ImpactPoint, ShotResult.ImpactNormal.ToOrientationRotator());

	if (IsValid(DecalComponent))
	{
		DecalComponent -> SetFadeOut(DecalInfo.DecalLifeTime, DecalInfo.DecalFadeOutTime);
		DecalComponent -> SetFadeScreenSize(.001f);
	}
}