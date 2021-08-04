#pragma once

class UCapsuleComponent;

struct FLedgeDetectionInfo
{

	FVector Location = FVector::ZeroVector;

	FVector LedgeNormal = FVector::ZeroVector;

	FRotator Rotation = FRotator::ZeroRotator;
	
};

class ULedgeDetector
{

public:
	void Init(UWorld* CurrentWorld, ACharacter* Owner, UCapsuleComponent* Capsule);

	bool DetectLedge(FLedgeDetectionInfo& LedgeDetection);
	
protected:
	UWorld* World = nullptr;

	ACharacter* Character = nullptr;

	UCapsuleComponent* CapsuleComponent = nullptr;
	
	float MinLedgeHeight = 40.f;

	float MaxLedgeHeight = 200.f;

	float ForwardCheckDistance = 100.f;

	FCollisionQueryParams QueryParams;

	FHitResult ForwardCheckHitResult;

	FHitResult DownwardCheckHitResult;
	
	FVector CharacterBottom;

	const float DrawTime = 2.f;

	bool HasClimbForward();

	bool HasClimbTop();

	bool HasCharacterFit();
};
