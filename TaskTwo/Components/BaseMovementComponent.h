// Fill out your copyright notice in the Description page of Project Settings. Сrawl Crouch Default

#pragma once

#include "CoreMinimal.h"

#include "Components/TimelineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CharacterAttributesComponent.h"

#include "BaseMovementComponent.generated.h"

class UAMComponent;
DECLARE_DELEGATE_OneParam(FStaminaParamsChange, FStaminaParams);

using FRotationCustomHandler = std::function<bool (float DeltaTime)>;
using FPhysCustomHandler = std::function<void (float DeltaTime, int32 Iterations)>;
using FCustomMoveChangeHandler = std::function<FStaminaParams (EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)>;
using FAMComponentMap = TMap<uint8, UAMComponent*>;

UENUM(BlueprintType)
enum class ECustomMovementMode : uint8
{
	CMOVE_None UMETA(DisplayName = "None"),
	CMOVE_Mantling UMETA(DisplayName = "Mantling"),
	CMOVE_Sprint UMETA(DisplayName = "Sprint"),
	CMOVE_Crouch UMETA(DisplayName = "Crouch"),
	CMOVE_Max UMETA(Hidden)
};

UCLASS()
class CHARACTERS_API UBaseMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UBaseMovementComponent();

	virtual float GetMaxSpeed() const override;
	
	UAMComponent* GetCurrentAMComponent() const
	{ return CurrentAMComponent.Get(); };

	void SetAdvancedMovementComponents(TArray<UAMComponent*> AMComponents);
	
	const FAMComponentMap& GetAdvancedMovementComponents() const
	{ return AdvancedMovementComponents; };
	
	bool MovementModIs(ECustomMovementMode Mode) const;
	bool MovementModIs(EMovementMode Mode) const;
	
	void Walking(float DeltaTime, int32 Iterations);

	FStaminaParamsChange OnStaminaParamsChange;

	uint8 PreviousCustomMovementMode;

protected:
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

	virtual void PhysCustom(float DeltaTime, int32 Iterations) override;

	virtual void PhysicsRotation(float DeltaTime) override;

private:
	FAMComponentMap AdvancedMovementComponents;

	TWeakObjectPtr<UAMComponent> CurrentAMComponent = nullptr;

	uint8 PreviousFlags = 0;
};

