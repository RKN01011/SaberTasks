// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAttributesComponent.h"

#include "BaseCharacter.h"
#include "BaseMovementComponent.h"
#include "Net/UnrealNetwork.h"

#define DEFAULT_STAMINA_EXTENDING 1.f
#define MIN_HEALTH 0.f

UCharacterAttributesComponent::UCharacterAttributesComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCharacterAttributesComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Character = StaticCast<ABaseCharacter*>(GetOwner());
	MovementComponent = StaticCast<UBaseMovementComponent*>(Character -> GetMovementComponent());

	Character -> OnTakeAnyDamage.AddDynamic(this, &UCharacterAttributesComponent::TakeAnyDamageHandler);
	MovementComponent -> OnStaminaParamsChange.BindUObject(this, &UCharacterAttributesComponent::StaminaParamsChangeHandler);
}

void UCharacterAttributesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bStaminaExpending)
	{
		CurrentStamina = FMath::Clamp(CurrentStamina -= StaminaConsumptionVelocity * DeltaTime, 0.0f, MaxStamina);

		if (CurrentStamina <= 0.f)
		{
			bStaminaExpending = false;
			bIsFatigue = true;
			MovementComponent -> MaxWalkSpeed = FatigueSpeed;

			if (OutOfStamina.IsBound())
				OutOfStamina.Broadcast();
		}
	}
	
	else if (CurrentStamina != MaxStamina)
	{
		CurrentStamina = FMath::Clamp(CurrentStamina += StaminaRestoreVelocity * DeltaTime, 0.0f, MaxStamina);

		if (CurrentStamina == MaxStamina)
		{
			MovementComponent -> MaxWalkSpeed = DefaultWalkSpeed;
			bIsFatigue = false;

			if (FullOfStamina.IsBound())
				FullOfStamina.Broadcast();
		}
	}
}

void UCharacterAttributesComponent::StaminaParamsChangeHandler(FStaminaParams StaminaParams)
{
	bStaminaExpending = !!StaminaParams;
	StaminaConsumptionVelocity = bStaminaExpending ?
		StaminaParams.SprintStaminaConsumptionVelocity : DEFAULT_STAMINA_EXTENDING;
}

void UCharacterAttributesComponent::TakeAnyDamageHandler(AActor* DamagedActor, float Damage,
    const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, MIN_HEALTH, MaxHealth);
	
	if (CurrentHealth == MIN_HEALTH)
	{
		Character -> Death();
	}
}

