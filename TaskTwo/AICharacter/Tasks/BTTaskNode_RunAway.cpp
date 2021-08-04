// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_RunAway.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

UBTTaskNode_RunAway::UBTTaskNode_RunAway()
{
	NodeName = "RunAway";
}

EBTNodeResult::Type UBTTaskNode_RunAway::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController	= OwnerComp.GetAIOwner();
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	
	if (!IsValid(AIController) || !IsValid(Blackboard))
	{
		return EBTNodeResult::Failed;
	}

	APawn* Pawn = AIController -> GetPawn();
	if (!IsValid(Pawn))
	{
		return EBTNodeResult::Failed;
	}

	const FVector EscapeLocation = Blackboard -> GetValueAsVector(EscapeLocationKey.SelectedKeyName);
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(AIController, EscapeLocation);
	return EBTNodeResult::InProgress;
}
