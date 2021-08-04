#pragma once

#include "CoreMinimal.h"

#include "../AMComponent.h"
#include "Components/TimelineComponent.h"

#include "SprintComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CHARACTERS_API USprintComponent : public UAMComponent
{
	GENERATED_BODY()

public:	
	USprintComponent();

	virtual void RegisterPlayerInput(UInputComponent* PlayerInputComponent) override;
	
	virtual void Select() override;

	virtual void PhysicsProcess(float DeltaTime, int32 Iterations) override;

	virtual void Off(EMovementMode NewMovementMode, ECustomMovementMode NewCustomMode) override;

	virtual ECustomMovementMode GetMovementMode() override
	{ return ECustomMovementMode::CMOVE_Sprint; };

protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Camera")
	UCurveFloat* SprintCameraCurveFloat = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Camera")
	float CameraSprintOffset = 600.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* SprintImpulse = nullptr;

	bool bIsSprintRequested = false;

	FTimeline SprintCameraTimeLine;
	
	bool bIsSprinting = false;

	UFUNCTION()
	void CameraTimelineHandler(float CurrentValue);

	void OutOfStaminaHandler();
	
	void StopSprint();
	
	bool CanSprint();
};
