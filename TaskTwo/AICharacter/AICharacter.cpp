// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"

#include "AIController.h"
#include "AIPatrollingComponent.h"

AAICharacter::AAICharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PatrollingComponent = CreateDefaultSubobject<UAIPatrollingComponent>(TEXT("PatrollingComponent"));
}

FVector AAICharacter::GetEscapeLocation() const
{
	return GetActorTransform().TransformPosition(EscapeLocation);
}

void AAICharacter::Death()
{
	Super::Death();
	AAIController* AController = StaticCast<AAIController*>(Controller);
	AController -> BrainComponent -> StopLogic("Stop");
}

UAIPatrollingComponent* AAICharacter::GetPatrollingComponent() const
{
	return PatrollingComponent;
}

UBehaviorTree* AAICharacter::GetBehaviorTree() const
{
	return BehaviorTree;
}
