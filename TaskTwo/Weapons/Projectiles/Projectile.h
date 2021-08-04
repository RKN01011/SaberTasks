// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Projectile.generated.h"

using FBulletHitHandler = std::function<void(const FHitResult& HitResult, const FVector& Direction)>;

UCLASS()
class CHARACTERS_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:
	AProjectile();

	UFUNCTION(BlueprintCallable)
	void LaunchProjectile(FVector Direction);
	
	void LaunchProjectile(FVector Direction, FBulletHitHandler Callback);

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USphereComponent* CollisionComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UProjectileMovementComponent* ProjectileMovementComponent;

	FBulletHitHandler LandingCallback;

	UFUNCTION()
	void CollisionHitHandler(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit)
	{
		if (LandingCallback != nullptr)
			LandingCallback(Hit, ProjectileMovementComponent -> Velocity);

		OnBulletHit();
	};

	UFUNCTION(BlueprintImplementableEvent)
	void OnBulletHit();
};


