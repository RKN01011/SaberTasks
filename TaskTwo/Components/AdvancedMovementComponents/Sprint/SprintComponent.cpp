#include "SprintComponent.h"

#include "BaseCharacter.h"
#include "Components/CharacterEquipmentComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerCharacter/PlayerCharacter.h"

USprintComponent::USprintComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USprintComponent::RegisterPlayerInput(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent -> BindAction("Sprint", IE_Pressed, this, &USprintComponent::Select);
}

void USprintComponent::Select()
{
	if (CanSprint())
	{
		if (Character -> GetEquipmentComponent() -> HasEquipWeapon())
			Character -> GetEquipmentComponent() -> RemoveWeapon();
		
		Character -> PlayAnimMontage(SprintImpulse);
		SprintCameraTimeLine.PlayFromStart();
		
		bIsSprinting = true;
		MovementComponent -> SetMovementMode(MOVE_Custom, (uint8)ECustomMovementMode::CMOVE_Sprint);

		Super::Select();
	}
}

void USprintComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsValid(SprintCameraCurveFloat))
	{
		FOnTimelineFloat ProgressFunction{};
		
		ProgressFunction.BindUFunction(this, FName("CameraTimelineHandler"));
		SprintCameraTimeLine.AddInterpFloat(SprintCameraCurveFloat, ProgressFunction, FName{TEXT("EFFECTFADE")});
	}
}

void USprintComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (SprintCameraTimeLine.IsPlaying())
		SprintCameraTimeLine.TickTimeline(DeltaTime);
}

void USprintComponent::Off(EMovementMode NewMovementMode, ECustomMovementMode NewCustomMode)
{
	StopSprint();
}

bool USprintComponent::CanSprint()
{
	return MovementComponent -> MovementModIs(MOVE_Walking) && !Character -> GetAttributesComponent() -> IsFatigue()
		&& MovementComponent -> Velocity.Size() > 0.f;
}

void USprintComponent::StopSprint()
{
	SprintCameraTimeLine.Reverse();
	bIsSprinting = false;
}

void USprintComponent::PhysicsProcess(float DeltaTime, int32 Iterations)
{   
	if (FMath::IsNearlyZero(MovementComponent -> Velocity.Size()))
	{
		MovementComponent -> SetDefaultMovementMode();
		return StopSprint();
	}
	
	MovementComponent -> Walking(DeltaTime, Iterations);
}

void USprintComponent::CameraTimelineHandler(float CurrentValue)
{
	//TODO
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Character);
	if (!PlayerCharacter) return;
	
	USpringArmComponent* SpringArmComponent = PlayerCharacter -> GetSpringArmComponent();
	if (SprintCameraTimeLine.IsReversing())
		SpringArmComponent -> TargetArmLength = FMath::Lerp(
			SpringArmComponent -> GetClass() -> GetDefaultObject<USpringArmComponent>() -> TargetArmLength,
			SpringArmComponent -> TargetArmLength, CurrentValue
		);
	else
		SpringArmComponent -> TargetArmLength = FMath::Lerp(
			SpringArmComponent -> TargetArmLength, CameraSprintOffset, CurrentValue
		);
}

void USprintComponent::OutOfStaminaHandler()
{
	if (MovementComponent -> MovementModIs(ECustomMovementMode::CMOVE_Sprint))
	{
		StopSprint();
		MovementComponent -> SetDefaultMovementMode();
	}
}
