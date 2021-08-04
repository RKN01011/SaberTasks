#pragma once

#include "CoreMinimal.h"

#include "Types.h"
#include "Animation/AnimInstance.h"
#include "Components/BaseMovementComponent.h"

#include "BaseAnimInstance.generated.h"

UCLASS()
class CHARACTERS_API UBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeBeginPlay() override;
	
	virtual void NativeUpdateAnimation(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="BaseMovement")
	float Direction = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="BaseMovement")
	float Speed = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="BaseMovement")
	bool bIsFatigue = false;
	
	UPROPERTY(VisibleAnywhere, Transient, BlueprintReadOnly, Category="IKSettings")
	FVector LeftBallEffectorLocation = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, Transient, BlueprintReadOnly, Category="IKSettings")
	FVector RightBallEffectorLocation = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, Transient, BlueprintReadOnly, Category="IKSettings")
	float RootBoneOffset = 0.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Aiming")
	bool bIsAiming = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Aiming")
	FTransform ForGripSocketTransform;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MovementModes")
	TEnumAsByte<EMovementMode> MovementMode = MOVE_None;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MovementModes")
	ECustomMovementMode CustomMovementMode = ECustomMovementMode::CMOVE_None;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MovementModes")
	ECustomMovementMode PreviousCustomMovementMode = ECustomMovementMode::CMOVE_None;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Caharacte anim | EquippedItemType")
	EAnimationItemTypes WeaponType = EAnimationItemTypes::EE_None;

	TWeakObjectPtr<class UBaseMovementComponent> CharacterMovement;

	virtual void Update(float DeltaTime);
	
private:
	TWeakObjectPtr<class ABaseCharacter> BaseCharacter;
};
