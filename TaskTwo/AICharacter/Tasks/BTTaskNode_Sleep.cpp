// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_Sleep.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "AICharacter/AICharacter.h"
#include "AICharacter/AIPatrollingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTaskNode_Sleep::UBTTaskNode_Sleep()
{
	NodeName = "SleepIfComeBackStartPoint";
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTaskNode_Sleep::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	
	AAICharacter* Character = StaticCast<AAICharacter*>(Pawn);
	const FWayPointInfo WayPointInfo = Character -> GetPatrollingComponent() -> GetCurrentWayPointInfo();

	if (WayPointInfo.ItIsSleepPoint)
	{
		if (CurrentPatrolsCount == PatrolsBeforeRest)
		{
			GetWorld() -> GetTimerManager().SetTimer(SleepTimer, this, &UBTTaskNode_Sleep::WakeUpPawn,
			SleepDuration, false);
				return EBTNodeResult::InProgress;
		}

		CurrentPatrolsCount++;
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Succeeded;
}

void UBTTaskNode_Sleep::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (bNeedWakeUp || IsValid(Blackboard) && IsValid(Blackboard -> GetValueAsObject(Enemy.SelectedKeyName)))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		bNeedWakeUp = false;
		CurrentPatrolsCount = 0;
	}
}

void UBTTaskNode_Sleep::WakeUpPawn()
{
	bNeedWakeUp = true;
}
