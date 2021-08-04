#include "BaseCharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "Components/BaseMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CharacterEquipmentComponent.h"
#include "Components/AdvancedMovementComponents/AMComponent.h"

const float Interp_Speed = 20.0f;

ABaseCharacter::ABaseCharacter(const FObjectInitializer& ObjectInitializer) :
	Super(
		ObjectInitializer
			.SetDefaultSubobjectClass<UBaseMovementComponent>(CharacterMovementComponentName)
	)
{
	AttributesComponent = CreateDefaultSubobject<UCharacterAttributesComponent>(TEXT("CharacterAttributesComponent"));
	EquipmentComponent = CreateDefaultSubobject<UCharacterEquipmentComponent>(TEXT("CharacterEquipmentComponent"));
	
	MovementComponent = StaticCast<UBaseMovementComponent*>(GetCharacterMovement());
	
	USkeletalMeshComponent* MeshComponent = GetMesh();
	MeshComponent -> CastShadow = true;
	MeshComponent -> bCastDynamicShadow = true;
	
	IKScale = GetActorScale3D().Z;
	IKDistance = GetCapsuleComponent() -> GetScaledCapsuleHalfHeight() * IKScale + 60.0f;
	DefaultRelativeMeshZOffset = GetMesh() -> GetRelativeLocation().Z;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	TeamId = (uint8)Team;
	RegisterAMComponents(InputComponent);
}

void ABaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AAIController* AIController = Cast<AAIController>(NewController);
	if (IsValid(AIController))
	{
		const FGenericTeamId TeamID{(uint8)Team};
		AIController -> SetGenericTeamId(TeamID);
	}
}

void ABaseCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	
	MovementComponent -> DisableMovement();
	GetWorld() -> GetTimerManager().SetTimer(LandedTimer,
		[this]() { MovementComponent -> SetDefaultMovementMode(); }, 1.f, false);
}

void ABaseCharacter::Aiming()
{
	if (!EquipmentComponent -> HasEquipWeapon()) return;
	bIsAiming = !bIsAiming;
	
	DoAiming();
}

void ABaseCharacter::Reload()
{
	if (!EquipmentComponent -> HasEquipWeapon()) return;
	
	ARangeWeapon* Weapon = EquipmentComponent -> GetCurrentEquipWeapon();
	if (IsValid(Weapon))
		Weapon -> Reload();
}

void ABaseCharacter::Death()
{
	USkeletalMeshComponent* MeshComponent = GetMesh();

	MovementComponent -> DisableMovement();
	MeshComponent -> SetSimulatePhysics(true);
	MeshComponent -> SetAllBodiesSimulatePhysics(true);
	MeshComponent -> SetCollisionProfileName(FName("Ragdoll"));
}

bool ABaseCharacter::SelectCustomMovementMode(ECustomMovementMode Mode)
{
	const FAMComponentMap& AMComponentMap = MovementComponent -> GetAdvancedMovementComponents();

    if (AMComponentMap.Find((uint8)Mode) != nullptr)
    {
    	AMComponentMap[(uint8)Mode] -> Select();
    	return true;
    }

    return false;
}

float ABaseCharacter::GetIKOffsetForASocket(const FName& SocketName, float Offset, float DeltaTime) const
{
	FVector const SocketLocation = GetMesh() -> GetSocketLocation(SocketName);
	FVector const TraceStart(SocketLocation.X, SocketLocation.Y, GetActorLocation().Z + 5.f);
	FVector const TraceEnd = TraceStart - IKDistance * FVector::UpVector;
	
	FHitResult HitResult;
	const FCollisionShape CollisionShape = FCollisionShape::MakeBox(FVector(10.f, 5.f, 5.f));
	const FQuat CollisionShapeRotation = GetActorRotation().Quaternion();
	if (GetWorld() -> SweepSingleByChannel(HitResult, TraceStart, TraceEnd, CollisionShapeRotation,
		ECC_Visibility, CollisionShape))
	{
		const float TargetOffset = (TraceEnd.Z - HitResult.Location.Z) / IKScale + 60.0f;
		return FMath::FInterpTo(Offset, TargetOffset, DeltaTime, Interp_Speed);
	}
		
	return 0.f;
}

void ABaseCharacter::RegisterAMComponents(UInputComponent* PlayerInputComponent)
{
	TArray<UAMComponent*> AMList;
	GetComponents<UAMComponent>(AMList, false);

	if (IsValid(PlayerInputComponent))
		for (auto AmComponent : AMList)
			AmComponent -> RegisterPlayerInput(PlayerInputComponent);
	
	MovementComponent -> SetAdvancedMovementComponents(AMList);
}
