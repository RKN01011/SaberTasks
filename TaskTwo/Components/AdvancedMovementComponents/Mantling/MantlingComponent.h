#pragma once

#include "CoreMinimal.h"

#include "../AMComponent.h"
#include "LedgeDetector.h"

#include "MantlingComponent.generated.h"

struct FMantlingMovementParameters
{
	FVector InitialLocation = FVector::ZeroVector;
	FRotator InitialRotation = FRotator::ZeroRotator;

	FVector TargetLocation = FVector::ZeroVector;
	FRotator TargetRotation = FRotator::ZeroRotator;
	
	FVector InitialAnimationLocation = FVector::ZeroVector;

	float Duration = 1.f;
	float StartTime = 0.f;

	class UCurveVector* MantlingCurve;
};

USTRUCT(BlueprintType)
struct FMantlingSettings
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UAnimMontage* MantlingMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UCurveVector* MantlingCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin = 0.0f, UIMin = 0.0f))
	float AnimationCorrectionXY = 65.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin = 0.0f, UIMin = 0.0f))
	float AnimationCorrectionZ = 200.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin = 0.0f, UIMin = 0.0f))
	float MaxHeight = 200.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin = 0.0f, UIMin = 0.0f))
	float MinHeight = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin = 0.0f, UIMin = 0.0f))
	float MaxHeightStartTime = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin = 0.0f, UIMin = 0.0f))
	float MinHeightStartTime = .5f;
};

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CHARACTERS_API UMantlingComponent : public UAMComponent
{
	GENERATED_BODY()

public:
	virtual void RegisterPlayerInput(UInputComponent* PlayerInputComponent) override;

	virtual void Select() override;
	
	virtual FStaminaParams On(EMovementMode PreviousMovementMode, ECustomMovementMode PreviousCustomMode) override;

	virtual void PhysicsProcess(float DeltaTime, int32 Iterations) override;

	virtual bool RotationProcess(float DeltaTime) override
	{ return false; };

	virtual ECustomMovementMode GetMovementMode() override
	{ return ECustomMovementMode::CMOVE_Mantling; };
	
	void EndMantle() const
	{ MovementComponent -> SetDefaultMovementMode(); };

protected:
	virtual void BeginPlay() override;
	
	ULedgeDetector LDComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="MantleSettings")
	FMantlingSettings HighMantleSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="MantleSettings")
	FMantlingSettings LowMantleSettings;
	
	FMantlingMovementParameters CurrentMantlingParameters;
	
	FTimerHandle MantlingTimer;
};
