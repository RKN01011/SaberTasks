// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/BaseMovementComponent.h"

#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

USTRUCT(BlueprintType)
struct FPlayerCameraRotationInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	float HeadRotationYawAngle = 0.f;

	UPROPERTY(BlueprintReadOnly)
	FRotator AimRotation = FRotator::ZeroRotator;
};

UCLASS(Blueprintable)
class CHARACTERS_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void MoveForward(float Value) override;
	
	virtual void MoveRight(float Value) override;
	
	virtual void LookUp(float Value) override;
	
	virtual void Turn(float Value) override;
	
	virtual void DoAiming() override;
	
	virtual void Jump() override;

	virtual void BeginFire() override;

	virtual void EndFire() override;

	virtual void RemoveWeapon() override;

	virtual void PickOneHandWeapon() override;

	virtual void PickTwoHandWeapon() override;

	float GetAngleCameraToCharacter() const;
	
	UFUNCTION(BlueprintCallable)
	FPlayerCameraRotationInfo CalculatingAngleRotationAimingAndHead(float DeltaTime,
		FPlayerCameraRotationInfo CurrentInfo);
	
	USpringArmComponent* GetSpringArmComponent() const
	{ return SpringArmComponent; };

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UCameraComponent* CameraComponent = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USpringArmComponent* SpringArmComponent = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AimingParametrs")
	UCurveFloat* AimingCameraCurve = nullptr;

private:
	float DefaultFOVCamera = 70.f;
	
	FVector DefaultCameraLocation = FVector::ZeroVector;

	FTimeline AimingCameraTimeline;
	
	UFUNCTION()
	void AimingCameraTimelineHandler() const;
};
