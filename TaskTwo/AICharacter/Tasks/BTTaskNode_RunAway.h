// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_RunAway.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERS_API UBTTaskNode_RunAway : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskNode_RunAway();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AI")
	FBlackboardKeySelector EscapeLocationKey;
	
};
