// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

AProjectile::AProjectile()
{
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent -> InitSphereRadius(5.f);
	CollisionComponent -> SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent -> SetCollisionResponseToChannels(ECR_Block);
	SetRootComponent(CollisionComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->InitialSpeed = 800.f;
	ProjectileMovementComponent->bShouldBounce = true;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	CollisionComponent -> OnComponentHit.AddDynamic(this, &AProjectile::CollisionHitHandler);
}

void AProjectile::LaunchProjectile(FVector Direction)
{
	ProjectileMovementComponent -> Velocity = Direction * ProjectileMovementComponent -> InitialSpeed;
	CollisionComponent -> IgnoreActorWhenMoving(GetOwner(), true);
}

void AProjectile::LaunchProjectile(FVector Direction, FBulletHitHandler Callback)
{
	LaunchProjectile(Direction);
	LandingCallback = Callback;
}