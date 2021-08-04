#pragma once

#include "CoreMinimal.h"

#include "Types.h"
#include "GenericTeamAgentInterface.h"
#include "Components/BaseMovementComponent.h"
#include "Components/CharacterAttributesComponent.h"
#include "GameFramework/Character.h"

#include "BaseCharacter.generated.h"


enum class EInteractiveActor : uint8;

class UIActorMovementComponent;
class AInteractiveActor;
class UCharacterAttributesComponent;
class UCharacterEquipmentComponent;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnAiming, bool, uint8);

UCLASS(Abstract, NotBlueprintable)
class CHARACTERS_API ABaseCharacter : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void PossessedBy(AController* NewController) override;

	virtual void Landed(const FHitResult& Hit) override;
	
	virtual void MoveForward(float Value) {};
	
	virtual void MoveRight(float Value) {};
	
	virtual void Turn(float Value) {};
	
	virtual void LookUp(float Value) {};

	virtual void BeginFire() {};

	virtual void EndFire() {};

	virtual void RemoveWeapon() {};

	virtual void PickOneHandWeapon() {};

	virtual void PickTwoHandWeapon() {};
	
	virtual void Death();
	
	void Aiming();
	virtual void DoAiming() {};
	
	void Reload();

	bool IsAiming() const
	{ return bIsAiming; };

	UFUNCTION(BlueprintCallable)
	bool SelectCustomMovementMode(ECustomMovementMode Mode);
	
	UCharacterAttributesComponent* GetAttributesComponent() const
	{ return AttributesComponent; }

	UCharacterEquipmentComponent* GetEquipmentComponent() const
	{ return EquipmentComponent; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetLeftBallOffset() const
	{ return FMath::Abs<float>(LeftBallOffset); };

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetRightBallOffset() const
	{ return FMath::Abs<float>(RightBallOffset); };

/** IGenericTeamAgentInterface **/
	virtual FGenericTeamId GetGenericTeamId() const override
	{ return TeamId; };
	
/** IGenericTeamAgentInterface **/

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="IK settings")
	FName LeftBallName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="IK settings")
	FName RightBallName;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCharacterAttributesComponent* AttributesComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCharacterEquipmentComponent* EquipmentComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Team")
	ETeams Team = ETeams::Player;
	
	TWeakObjectPtr<class UBaseMovementComponent> MovementComponent = nullptr;

	FVector PointFallStart;

	float CurrentFallDamage = 0.f;

	float LeftBallOffset;
	
	float RightBallOffset;
	
	float IKScale = 0.0f;

	float IKDistance = 0.0f;

	float DefaultRelativeMeshZOffset = 0.0f;
	
	bool bIsAiming = false;

	FTimerHandle LandedTimer;

	FGenericTeamId TeamId;

	float GetIKOffsetForASocket(const FName& SocketName, float Offset, float DeltaTime) const;
	
private:
	void RegisterAMComponents(UInputComponent* PlayerInputComponent);
};


