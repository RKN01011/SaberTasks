// Fill out your copyright notice in the Description page of Project Settings.


#include "BlastComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

void UBlastComponent::Explode(AController* Controller, bool bDestroyItemAfterBlast)
{
	ApplyRadialDamageWithFalloff(Controller);
	
	if (IsValid(ExplosionVFX))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionVFX, GetOwner() -> GetActorLocation());
	}
	
	if (OnExplosion.IsBound())
		OnExplosion.Broadcast();

	if (bDestroyItemAfterBlast)
		DestroyItem();
}

void UBlastComponent::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld() -> GetTimerManager().SetTimer(ExplodeTimer, [this]() {Explode(nullptr, true);},
		DestroyItemTimeDelay, false);
}

void UBlastComponent::ApplyRadialDamageWithFalloff(AController* Controller)
{
	TArray<AActor*> IgnoredActors = {GetOwner()};
	UGameplayStatics::ApplyRadialDamageWithFalloff(
		GetWorld(),
		MaxDamage,
		MinDamage,
		GetOwner() -> GetActorLocation(),
		InnerRadius,
		OuterRadius,
		DamageFalloff,
		DamageTypeClass,
		IgnoredActors,
		GetOwner(),
		Controller,
		ECC_Visibility
	);
}

void UBlastComponent::DestroyItem()
{
	GetWorld() -> DestroyActor(GetOwner());
}

