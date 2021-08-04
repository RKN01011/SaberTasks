#include "BaseAnimInstance.h"

#include "BaseCharacter.h"
#include "Components/CharacterEquipmentComponent.h"

void UBaseAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	BaseCharacter = StaticCast<ABaseCharacter*>(TryGetPawnOwner());
	checkf(BaseCharacter.IsValid(), TEXT("Character is nullptr in UBaseAnimInstance"));
	
	CharacterMovement = StaticCast<UBaseMovementComponent*>(BaseCharacter -> GetCharacterMovement());
}

void UBaseAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (!CharacterMovement.IsValid()) return;

	Update(DeltaTime);
}

void UBaseAnimInstance::Update(float DeltaTime)
{
	Direction = CalculateDirection(CharacterMovement -> Velocity, BaseCharacter -> GetActorRotation());
	Speed = CharacterMovement -> Velocity.Size();

	RightBallEffectorLocation = FVector(-BaseCharacter -> GetLeftBallOffset(), 0.f, 0.f);
	LeftBallEffectorLocation = FVector(BaseCharacter -> GetRightBallOffset(), 0.f, 0.f);
	RootBoneOffset = -FMath::Max(FMath::Abs(RightBallEffectorLocation.X), FMath::Abs(LeftBallEffectorLocation.X));

	MovementMode = CharacterMovement -> MovementMode;
	CustomMovementMode = (ECustomMovementMode)CharacterMovement -> CustomMovementMode;
	PreviousCustomMovementMode = (ECustomMovementMode)CharacterMovement -> PreviousCustomMovementMode;
	
	bIsFatigue = BaseCharacter -> GetAttributesComponent() -> IsFatigue();
	bIsAiming = BaseCharacter -> IsAiming();

	UCharacterEquipmentComponent* CharacterEquipmentComponent = BaseCharacter -> GetEquipmentComponent();
	if (CharacterEquipmentComponent -> HasEquipWeapon())
	{
		ARangeWeapon* EquipmentItem = CharacterEquipmentComponent -> GetCurrentEquipWeapon();

		WeaponType = EquipmentItem -> GetAnimationType();
		ForGripSocketTransform = EquipmentItem -> GetForeGripTransform();
	}
	else
	{
		WeaponType = EAnimationItemTypes::EE_None;
	}
}
