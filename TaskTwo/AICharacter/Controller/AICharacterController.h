// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AICharacterController.generated.h"

class UAISense;
class AAICharacter;
/**
 * 
 */
UCLASS()
class CHARACTERS_API AAICharacterController : public AAIController
{
	GENERATED_BODY()

public:
	AAICharacterController();
	
	virtual void SetPawn(APawn* InPawn) override;

	virtual void ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors) override;

	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement")
	float TargetReachRadius = 100.f;
	
	AActor* GetClosestSensedActor(TSubclassOf<UAISense> SenseClass) const;

private:
	TWeakObjectPtr<AAICharacter> BaseCharacter = nullptr;

	bool bIsPatrolling = false;

	void TryMoveToNextTarget();

	bool IsTargetReached(FVector TargetLocation) const;

	UFUNCTION()
	void WeaponAmmoAmountEnded();
	
};
