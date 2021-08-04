// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"


#include "DrawDebugHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/CharacterEquipmentComponent.h"
#include "Controller/BasePlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/SpringArmComponent.h"

#define CAMERA_INTERP_SPEED 5.f

APlayerCharacter::APlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bUseControllerRotationPitch = bUseControllerRotationYaw = bUseControllerRotationRoll = false;
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent -> SetupAttachment(RootComponent);
	SpringArmComponent -> bUsePawnControlRotation = true;
	SpringArmComponent -> bEnableCameraLag = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent -> SetupAttachment(SpringArmComponent);
	CameraComponent -> bUsePawnControlRotation = false;
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	InputComponent -> BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	InputComponent -> BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	InputComponent -> BindAxis("LookUp", this, &APlayerCharacter::LookUp);
	InputComponent -> BindAxis("Turn", this, &APlayerCharacter::Turn);
	
	InputComponent -> BindAction("Jump", IE_Pressed, this, &APlayerCharacter::Jump);
	InputComponent -> BindAction("Fire", IE_Pressed, this, &APlayerCharacter::BeginFire);
	InputComponent -> BindAction("Fire", IE_Released, this, &APlayerCharacter::EndFire);
	InputComponent -> BindAction("RemoveWeapon", IE_Pressed, this, &APlayerCharacter::RemoveWeapon);
	InputComponent -> BindAction("PickOneHandWeapon", IE_Pressed, this, &APlayerCharacter::PickOneHandWeapon);
	InputComponent -> BindAction("PickTwoHandWeapon", IE_Pressed, this, &APlayerCharacter::PickTwoHandWeapon);
	InputComponent -> BindAction("Aiming", IE_Pressed, this, &APlayerCharacter::Aiming);
	InputComponent -> BindAction("Reload", IE_Pressed, this, &APlayerCharacter::Reload);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	DefaultCameraLocation = CameraComponent -> GetRelativeLocation();
	DefaultFOVCamera = CameraComponent -> FieldOfView;
	
	if (IsValid(AimingCameraCurve))
	{
		FOnTimelineFloat ProgressFunction{};
		
		ProgressFunction.BindUFunction(this, FName("AimingCameraTimelineHandler"));
		AimingCameraTimeline.AddInterpFloat(AimingCameraCurve, ProgressFunction, FName{TEXT("AimingCameraTimeline")});
	}
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (AimingCameraTimeline.IsPlaying())
		AimingCameraTimeline.TickTimeline(DeltaSeconds);
}

float APlayerCharacter::GetAngleCameraToCharacter() const
{
	const FVector ToCharacter = CameraComponent -> GetComponentRotation().Vector().GetSafeNormal2D();
	const float ForwardCosAngle = FVector::DotProduct(GetActorForwardVector(), ToCharacter);
	const float DirectionModifer = FVector::DotProduct(GetActorRightVector(), ToCharacter) < 0 ? -1.f : 1.f;

	return FMath::RadiansToDegrees(FMath::Acos(ForwardCosAngle)) * DirectionModifer;
}

FPlayerCameraRotationInfo APlayerCharacter::CalculatingAngleRotationAimingAndHead(float DeltaTime,
	FPlayerCameraRotationInfo CurrentInfo)
{
	FPlayerCameraRotationInfo PlayerCameraRotationInfo;
	
	const float AngleFull = GetAngleCameraToCharacter();
    const float CurrentHeadAngle = FMath::Abs(AngleFull) > 80.f ? 0.f : FMath::Clamp(AngleFull, -60.f, 60.f);

    PlayerCameraRotationInfo.HeadRotationYawAngle = FMath::FInterpTo(CurrentInfo.HeadRotationYawAngle,
    	CurrentHeadAngle, DeltaTime, 3.f);
    
    const FVector AimDirectionWorld = GetBaseAimRotation().Vector();
    const FVector AimDirectionLocal = GetTransform().InverseTransformVectorNoScale(AimDirectionWorld);
    FRotator AimRotation = AimDirectionLocal.ToOrientationRotator();
    AimRotation.Yaw = AngleFull;
	PlayerCameraRotationInfo.AimRotation = AimRotation;

	return PlayerCameraRotationInfo;
}

void APlayerCharacter::AimingCameraTimelineHandler() const
{
	ABasePlayerController* BasePlayerController = StaticCast<ABasePlayerController*>(Controller);
	APlayerCameraManager* CameraManager = BasePlayerController -> PlayerCameraManager;
	const FVector CameraRelativeLocation = CameraComponent -> GetRelativeLocation();
	const float DeltaTime = GetWorld() -> DeltaTimeSeconds;
	
	FVector NewLocation;
	float NewFOV;
	
	if (AimingCameraTimeline.IsReversing() || !EquipmentComponent -> HasEquipWeapon())
	{
		NewLocation = FMath::VInterpTo(CameraRelativeLocation, DefaultCameraLocation,
			DeltaTime, CAMERA_INTERP_SPEED);
		NewFOV = FMath::FInterpTo(CameraManager -> GetFOVAngle(), DefaultFOVCamera,
			DeltaTime, CAMERA_INTERP_SPEED);
	}
	else
	{
		NewLocation = FMath::VInterpTo(CameraRelativeLocation, FVector{150.f, 50.f, -20.f},
			DeltaTime, CAMERA_INTERP_SPEED);
		NewFOV = FMath::FInterpTo(CameraManager -> GetFOVAngle(), 60.f,
			DeltaTime, CAMERA_INTERP_SPEED);
	}
		
	CameraManager -> SetFOV(NewFOV);
	CameraComponent -> SetRelativeLocation(NewLocation);
}

void APlayerCharacter::MoveForward(float Value)
{
	if (!FMath::IsNearlyZero(Value, 1e-6f))
	{
		FRotator const YawRotator(0.0f, GetControlRotation().Yaw, 0.0f);
		AddMovementInput(YawRotator.RotateVector(FVector::ForwardVector), Value);
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if (!FMath::IsNearlyZero(Value, 1e-6f))
	{
		FRotator const YawRotator(0.0f, GetControlRotation().Yaw, 0.0f);
		AddMovementInput(YawRotator.RotateVector(FVector::RightVector), Value);
	}
}

void APlayerCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void APlayerCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void APlayerCharacter::DoAiming()
{
	if (bIsAiming)
	{
		bUseControllerRotationYaw = true;
		AimingCameraTimeline.Play();
	}
	
	else
	{
		bUseControllerRotationYaw = false;
		AimingCameraTimeline.Reverse();
	}
}

void APlayerCharacter::Jump()
{
	if (MovementComponent -> MovementModIs(MOVE_Walking) ||
		MovementComponent -> MovementModIs(ECustomMovementMode::CMOVE_Sprint)
	)
	{
		return Super::Jump();
	}

	SelectCustomMovementMode((ECustomMovementMode)MovementComponent -> CustomMovementMode);
}

void APlayerCharacter::BeginFire()
{
	EquipmentComponent -> BeginFire();
}

void APlayerCharacter::EndFire()
{
	EquipmentComponent -> EndFire();
}

void APlayerCharacter::RemoveWeapon()
{
	if (EquipmentComponent -> HasEquipWeapon())
	{
		EquipmentComponent -> RemoveWeapon();

		//TODO
		if (bIsAiming)
		{
			AimingCameraTimeline.Reverse();
			bUseControllerRotationYaw = false;
			bIsAiming = false;
		}
	}
}

void APlayerCharacter::PickOneHandWeapon()
{
	EquipmentComponent -> PickOneHandWeapon();
}

void APlayerCharacter::PickTwoHandWeapon()
{
	EquipmentComponent -> PickTwoHandWeapon();
}
