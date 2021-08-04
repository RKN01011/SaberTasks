// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_Sleep.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERS_API UBTTaskNode_Sleep : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskNode_Sleep();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AI")
	float SleepDuration = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AI")
	int32 PatrolsBeforeRest = 2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AI")
	FBlackboardKeySelector Enemy;

	int32 CurrentPatrolsCount = 0;

	UFUNCTION()
	void WakeUpPawn();

	bool bNeedWakeUp = false;

	FTimerHandle SleepTimer;
};
