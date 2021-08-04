#include "LedgeDetector.h"

#include "Types.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"


void ULedgeDetector::Init(UWorld* CurrentWorld, ACharacter* Owner, UCapsuleComponent* Capsule)
{
	World = CurrentWorld;
	Character = Owner;
	CapsuleComponent = Capsule;
	QueryParams.bTraceComplex = true;
	QueryParams.AddIgnoredActor(Character);
}

bool ULedgeDetector::DetectLedge(FLedgeDetectionInfo& LedgeDetection)
{
	const FVector BottomZOffset = (CapsuleComponent -> GetScaledCapsuleHalfHeight() - 2.f) * FVector::UpVector;
	CharacterBottom = Character -> GetActorLocation() - BottomZOffset;
	
	if (HasClimbForward() && HasClimbTop() && HasCharacterFit())
	{
		const FVector Rotation = FVector(-1.f, -1.f, 0.f);
		LedgeDetection.Location = DownwardCheckHitResult.ImpactPoint + (CapsuleComponent -> GetScaledCapsuleHalfHeight() + 2.f) * FVector::UpVector;
		LedgeDetection.Rotation = (ForwardCheckHitResult.ImpactNormal * Rotation).ToOrientationRotator();
		LedgeDetection.LedgeNormal = ForwardCheckHitResult.ImpactNormal;
	
		return true;
	};

	return false;
}

bool ULedgeDetector::HasClimbForward()
{
	const float ForwardCheckCapsuleRadius = CapsuleComponent -> GetScaledCapsuleRadius();
	const float ForwardCheckCapsuleHalfHeight = (MaxLedgeHeight - MinLedgeHeight) * .5f;

	const FCollisionShape ForwardCapsuleShape = FCollisionShape::MakeCapsule(ForwardCheckCapsuleRadius, ForwardCheckCapsuleHalfHeight);
	const FVector ForwardStartLocation = CharacterBottom + (MinLedgeHeight + ForwardCheckCapsuleHalfHeight) * FVector::UpVector;
	const FVector ForwardEndLocation = ForwardStartLocation + Character -> GetActorForwardVector() * ForwardCheckDistance;

	const bool Result = World -> SweepSingleByChannel(
        ForwardCheckHitResult, ForwardStartLocation, ForwardEndLocation,
        FQuat::Identity, ECC_Climbing, ForwardCapsuleShape, QueryParams
    );
	
	return Result;
}

bool ULedgeDetector::HasClimbTop()
{
	const float DownwardSphereCheckRadius = CapsuleComponent -> GetScaledCapsuleRadius();
	const FCollisionShape DownwardCollisionShape = FCollisionShape::MakeSphere(DownwardSphereCheckRadius);
 
	const float DownwardCheckDepthOffset = 10.f;
	FVector DownwardStartLocation = ForwardCheckHitResult.ImpactPoint - ForwardCheckHitResult.ImpactNormal * DownwardCheckDepthOffset;
	DownwardStartLocation.Z = CharacterBottom.Z + MaxLedgeHeight + DownwardSphereCheckRadius;
	const FVector DownwardEndLocation{DownwardStartLocation.X, DownwardStartLocation.Y, CharacterBottom.Z};
 
	const bool Result = World -> SweepSingleByChannel(
    DownwardCheckHitResult, DownwardStartLocation, DownwardEndLocation,
    FQuat::Identity, ECC_Climbing, DownwardCollisionShape, QueryParams
    );
 
	return Result;
}

bool ULedgeDetector::HasCharacterFit()
{
	const float OverlapCapsuleRadius = CapsuleComponent -> GetScaledCapsuleRadius();
	const float OverlapCapsuleHalfHeight = CapsuleComponent -> GetScaledCapsuleHalfHeight();
	const FCollisionShape OverlapCapsuleShape = FCollisionShape::MakeCapsule(OverlapCapsuleRadius, OverlapCapsuleHalfHeight);
	const FVector OverlapLocation = DownwardCheckHitResult.ImpactPoint + (OverlapCapsuleHalfHeight + 2.f) * FVector::UpVector;
	
	return !World -> OverlapBlockingTestByProfile(OverlapLocation, FQuat::Identity, FName("Pawn"), OverlapCapsuleShape, QueryParams);
}


