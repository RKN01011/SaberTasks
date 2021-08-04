#include "BaseMovementComponent.h"

#include "BaseCharacter.h"
#include "DrawDebugHelpers.h"
#include "AdvancedMovementComponents/AMComponent.h"
#include "GameFramework/SpringArmComponent.h"


UBaseMovementComponent::UBaseMovementComponent()
{
	bOrientRotationToMovement = 1;
	RotationRate = FRotator(0.0f, 540.0f, 0.0f);
}

float UBaseMovementComponent::GetMaxSpeed() const
{
	if (MovementModIs(MOVE_Custom))
		return CurrentAMComponent -> GetMaxSpeed();
		
	return Super::GetMaxSpeed();
}

bool UBaseMovementComponent::MovementModIs(ECustomMovementMode Mode) const
{
	return UpdatedComponent && MovementMode == MOVE_Custom && CustomMovementMode == static_cast<uint8>(Mode);
}
bool UBaseMovementComponent::MovementModIs(EMovementMode Mode) const
{
	return UpdatedComponent && MovementMode == Mode;
}

void UBaseMovementComponent::SetAdvancedMovementComponents(TArray<UAMComponent*> AMComponents)
{
	for (auto AMComponent : AMComponents)
		AdvancedMovementComponents.Add((uint8)AMComponent -> GetMovementMode(), AMComponent);
}

void UBaseMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);
	if (CharacterOwner -> GetLocalRole() < ROLE_AutonomousProxy) return;
	
	PreviousCustomMovementMode = PreviousCustomMode;
	if (PreviousMovementMode == MOVE_Custom && CurrentAMComponent.IsValid())
	{
		CurrentAMComponent -> Off(MovementMode, (ECustomMovementMode)CustomMovementMode);
		CurrentAMComponent = nullptr;
	}
	
	if (MovementMode == MOVE_Custom)
	{
		if (AdvancedMovementComponents.Find(CustomMovementMode) != nullptr)
		{
			CurrentAMComponent = AdvancedMovementComponents[CustomMovementMode];
			
			OnStaminaParamsChange.ExecuteIfBound(CurrentAMComponent-> On(PreviousMovementMode,
				(ECustomMovementMode)PreviousCustomMode));
		}
		
		return;
	}

	OnStaminaParamsChange.ExecuteIfBound(FStaminaParams{});
}

void UBaseMovementComponent::PhysCustom(float DeltaTime, int32 Iterations)
{
	Super::PhysCustom(DeltaTime, Iterations);
	CurrentAMComponent -> PhysicsProcess(DeltaTime, Iterations);
}

void UBaseMovementComponent::PhysicsRotation(float DeltaTime)
{
	if (MovementModIs(MOVE_Custom) && !CurrentAMComponent -> RotationProcess(DeltaTime)) return;
	Super::PhysicsRotation(DeltaTime);
}

void UBaseMovementComponent::Walking(float DeltaTime, int32 Iterations)
{
	// TODO
	PhysWalking(DeltaTime, Iterations);
}