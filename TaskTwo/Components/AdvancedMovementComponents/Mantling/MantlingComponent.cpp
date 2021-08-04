#include "MantlingComponent.h"

#include "BaseCharacter.h"
#include "LedgeDetector.h"
#include "Curves/CurveVector.h"
#include "GameFramework/Character.h"

void UMantlingComponent::BeginPlay()
{
	Super::BeginPlay();
	LDComponent.Init(GetWorld(), Character.Get(), Character -> GetCapsuleComponent());
}

void UMantlingComponent::RegisterPlayerInput(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent -> BindAction("Mantle", IE_Pressed, this, &UMantlingComponent::Select);
}

void UMantlingComponent::Select()
{
	FLedgeDetectionInfo LedgeDetection;
	if (!MovementComponent -> MovementModIs(MOVE_Falling) && LDComponent.DetectLedge(LedgeDetection))
	{
		FVector CharacterLocation = Character -> GetActorLocation();
		float MHeight = (LedgeDetection.Location - CharacterLocation).Z;

		FMantlingSettings MSettings = MHeight <= LowMantleSettings.MaxHeight ? LowMantleSettings : HighMantleSettings;
		
		float MinRange, MaxRange; float PlayRate = 1.f;
		MSettings.MantlingCurve -> GetTimeRange(MinRange, MaxRange);
		FVector2D SRange(MSettings.MinHeight, MSettings.MaxHeight);
		FVector2D TRange(MSettings.MinHeightStartTime, MSettings.MaxHeightStartTime);
		UAnimInstance* AnimInstance = Character -> GetMesh() -> GetAnimInstance();
			
		FMantlingMovementParameters MParameters;
		CurrentMantlingParameters.MantlingCurve = MSettings.MantlingCurve;
		CurrentMantlingParameters.InitialLocation = CharacterLocation;
		CurrentMantlingParameters.InitialRotation = Character -> GetActorRotation();
		CurrentMantlingParameters.TargetLocation = LedgeDetection.Location;
		CurrentMantlingParameters.TargetRotation = LedgeDetection.Rotation;
		CurrentMantlingParameters.Duration = MaxRange - MinRange;
		CurrentMantlingParameters.StartTime = FMath::GetMappedRangeValueClamped(SRange, TRange, MHeight);
		CurrentMantlingParameters.InitialAnimationLocation =
            CurrentMantlingParameters.TargetLocation -
                (MSettings.AnimationCorrectionZ * FVector::UpVector) +
                    (MSettings.AnimationCorrectionXY * LedgeDetection.LedgeNormal);
		
		MovementComponent -> SetMovementMode(MOVE_Custom, (uint8)ECustomMovementMode::CMOVE_Mantling);
		AnimInstance -> Montage_Play(MSettings.MantlingMontage, PlayRate,
			EMontagePlayReturnType::Duration, MParameters.StartTime);
	}
}

FStaminaParams UMantlingComponent::On(EMovementMode PreviousMovementMode, ECustomMovementMode PreviousCustomMode)
{
	GetWorld() -> GetTimerManager().SetTimer(MantlingTimer, this, &UMantlingComponent::EndMantle,
        CurrentMantlingParameters.Duration, false);

	return StaminaParams;
}

void UMantlingComponent::PhysicsProcess(float DeltaTime, int32 Iterations)
{
	FHitResult Hit;
	float ElapsedTime = GetWorld() -> GetTimerManager().GetTimerElapsed(MantlingTimer) + CurrentMantlingParameters.StartTime;

	FVector MantlingCurveValue = CurrentMantlingParameters.MantlingCurve -> GetVectorValue(ElapsedTime);
	float PositionAlpha = MantlingCurveValue.X;
	float XYCorrectionAlpha = MantlingCurveValue.Y;
	float ZCorrectionAlpha = MantlingCurveValue.Z;
	FVector CorrectedInitialLocation = FMath::Lerp(CurrentMantlingParameters.InitialLocation, CurrentMantlingParameters.InitialAnimationLocation, XYCorrectionAlpha);
	CorrectedInitialLocation.Z = FMath::Lerp(CurrentMantlingParameters.InitialLocation.Z, CurrentMantlingParameters.InitialAnimationLocation.Z, ZCorrectionAlpha);
		
	FVector NewLocation = FMath::Lerp(CorrectedInitialLocation, CurrentMantlingParameters.TargetLocation, PositionAlpha);
	FRotator NewRotation = FMath::Lerp(CurrentMantlingParameters.InitialRotation, CurrentMantlingParameters.TargetRotation, PositionAlpha);

	FVector Delta = NewLocation - Character -> GetActorLocation();
	MovementComponent -> Velocity = Delta / DeltaTime;
		
	MovementComponent -> SafeMoveUpdatedComponent(Delta, NewRotation, false, Hit);
}
