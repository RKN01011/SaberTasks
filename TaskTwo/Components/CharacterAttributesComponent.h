// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterAttributesComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FFullOfStamina);
DECLARE_MULTICAST_DELEGATE(FOutOfStamina);

USTRUCT(BlueprintType)
struct FStaminaParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SprintStaminaConsumptionVelocity = 60.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bStaminaExpending = false;

	operator bool() const
	{ return bStaminaExpending; }
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CHARACTERS_API UCharacterAttributesComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCharacterAttributesComponent();

	bool IsFatigue() const
	{ return bIsFatigue; };

	float GetCurrentStamina() const
	{ return CurrentStamina; };

	float GetMaxStamina() const
	{ return MaxStamina; };

	float GetStaminaPercent() const
	{ return CurrentStamina / MaxStamina; };

	float GetCurrentHealth() const
	{ return CurrentHealth; };

	float GetMaxHealth() const
	{ return MaxHealth; };

	float GetHealthPercent() const
	{ return CurrentHealth / MaxHealth; };

	FOutOfStamina OutOfStamina;
	
	FFullOfStamina FullOfStamina;

protected:
	virtual void BeginPlay() override;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	TWeakObjectPtr<class ABaseCharacter> Character = nullptr;
	
	TWeakObjectPtr<class UBaseMovementComponent> MovementComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float StaminaRestoreVelocity = 60.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxStamina = 100.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxHealth = 100.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FatigueSpeed = 200.f;

	UFUNCTION()
	void StaminaParamsChangeHandler(FStaminaParams StaminaParams);

	UFUNCTION()
	void TakeAnyDamageHandler(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
		class AController* InstigatedBy, AActor* DamageCauser);

private:
	bool bIsFatigue = false;

	bool bStaminaExpending = false;
	
	float StaminaConsumptionVelocity = 1.f;

	float DefaultWalkSpeed = 600.f;
	
	float CurrentStamina = 100.f;

	float CurrentHealth = 100.f;
};
