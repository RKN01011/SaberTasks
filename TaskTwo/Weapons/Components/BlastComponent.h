// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BlastComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExplosion);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CHARACTERS_API UBlastComponent : public UActorComponent
{
	GENERATED_BODY()

	public:
	UFUNCTION(BlueprintCallable)
	void Explode(AController* Controller, bool bDestroyItemAfterBlast = false);

	UPROPERTY(BlueprintAssignable)
	FOnExplosion OnExplosion;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Exposion | Damage")
	float MaxDamage = 75.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Exposion | Damage")
	float MinDamage = 30.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Exposion | Damage")
	float DamageFalloff = 1.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Exposion | Damage")
	TSubclassOf<UDamageType> DamageTypeClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Exposion | Damage | Radius")
	float InnerRadius = 300.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Exposion | Damage | Radius")
	float OuterRadius = 500.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Exposion | Damage | VFX")
	UParticleSystem* ExplosionVFX = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Exposion | Damage | Timer")
	float DestroyItemTimeDelay = 10.f;	
	
	void ApplyRadialDamageWithFalloff(AController* Controller);
	
	void DestroyItem();

	private:
	FTimerHandle ExplodeTimer;	
};
