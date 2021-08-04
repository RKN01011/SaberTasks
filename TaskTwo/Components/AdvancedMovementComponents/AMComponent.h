// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/BaseMovementComponent.h"

#include "AMComponent.generated.h"

UCLASS(Abstract)
class CHARACTERS_API UAMComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	virtual void RegisterPlayerInput(UInputComponent* PlayerInputComponent) {};
	
	virtual void Select() {};

	// MovementMode
	virtual ECustomMovementMode GetMovementMode() { return ECustomMovementMode::CMOVE_None; };

	// On/Off process
	virtual FStaminaParams On(EMovementMode PreviousMovementMode, ECustomMovementMode PreviousCustomMode) { return StaminaParams; };
	virtual void Off(EMovementMode NewMovementMode, ECustomMovementMode NewCustomMode) {};

	// Process
	virtual void PhysicsProcess(float DeltaTime, int32 Iterations) { };
	virtual bool RotationProcess(float DeltaTime) { return true; };

	// Speed parameters
	virtual float GetSpeedRatio() { return 0.f; };
	float GetMaxSpeed() const { return MaxSpeed; };

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0", UIMin="0"))
	float MaxSpeed = 200.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FStaminaParams StaminaParams;
	
	TWeakObjectPtr<class ABaseCharacter> Character = nullptr;
	
	TWeakObjectPtr<class UBaseMovementComponent> MovementComponent = nullptr;
};
