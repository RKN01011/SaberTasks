// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacterController.h"

#include "Types.h"
#include "BaseCharacter.h"
#include "AICharacter/AICharacter.h"
#include "AICharacter/AIPatrollingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CharacterEquipmentComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"

AAICharacterController::AAICharacterController()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
}

void AAICharacterController::BeginPlay()
{
	Super::BeginPlay();
	UAIPatrollingComponent* PatrollingComponent = BaseCharacter -> GetPatrollingComponent();

	if (IsValid(Blackboard))
	{
		AAICharacter* AICharacter = StaticCast<AAICharacter*>(GetPawn());
		Blackboard -> SetValueAsVector(BB_EscapeLocation, AICharacter -> GetEscapeLocation());

		if (PatrollingComponent -> CanPatrol())
		{
			const FVector ClosestWayPoint = PatrollingComponent -> SelectClosestWaypoint();
		
			Blackboard -> SetValueAsObject(BB_CurrentTarget, nullptr);
			Blackboard -> SetValueAsVector(BB_NextLocation, ClosestWayPoint);
			bIsPatrolling = true;
		}
	}

	UCharacterEquipmentComponent* EquipmentComponent = BaseCharacter -> GetEquipmentComponent();
	EquipmentComponent -> OnWeaponAmmoAmountEnded.AddUFunction(this, FName{"WeaponAmmoAmountEnded"});
}

AActor* AAICharacterController::GetClosestSensedActor(TSubclassOf<UAISense> SenseClass) const
{
	if (!IsValid(GetPawn())) return nullptr;
	
	TArray<AActor*> SensedActors;
	PerceptionComponent -> GetCurrentlyPerceivedActors(SenseClass, SensedActors);

	AActor* ClosestActor = nullptr;
	float MinSquaredDistance = FLT_MAX;
	const FVector TurretLocation = GetPawn() -> GetActorLocation();

	for (auto SeenActor : SensedActors)
	{
		const float CurrentSquaredDistance = (TurretLocation - SeenActor -> GetActorLocation()).SizeSquared();
		if (CurrentSquaredDistance < MinSquaredDistance)
		{
			MinSquaredDistance = CurrentSquaredDistance;
			ClosestActor = SeenActor;
		}
	}

	return ClosestActor;
}

void AAICharacterController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	if (IsValid(InPawn))
	{
		checkf(InPawn -> IsA<ABaseCharacter>(), TEXT("Only ABaseCharacter"));
		BaseCharacter = StaticCast<AAICharacter*>(InPawn);
		RunBehaviorTree(BaseCharacter -> GetBehaviorTree());
		return;
	}

	BaseCharacter = nullptr;
}

void AAICharacterController::ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	Super::ActorsPerceptionUpdated(UpdatedActors);

	if (!BaseCharacter.IsValid()) return;

	TryMoveToNextTarget();
}

void AAICharacterController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	if (!Result.IsSuccess()) return;
	TryMoveToNextTarget();
}

void AAICharacterController::TryMoveToNextTarget()
{
	UAIPatrollingComponent* PatrollingComponent = BaseCharacter -> GetPatrollingComponent();
	AActor* ClosestActor = GetClosestSensedActor(UAISense_Sight::StaticClass());
	
	if (IsValid(ClosestActor))
	{
		if (IsValid(Blackboard))
		{
			Blackboard -> SetValueAsObject(BB_CurrentTarget, ClosestActor);
			SetFocus(ClosestActor, EAIFocusPriority::Gameplay);
		}

		bIsPatrolling = false;
		return;
		
	}
	
	if (PatrollingComponent -> CanPatrol())
	{
		FVector NextWayPoint = bIsPatrolling ? PatrollingComponent -> SelectNextWaypoint() :
			PatrollingComponent -> SelectClosestWaypoint();
		
		if (IsValid(Blackboard))
		{
			ClearFocus(EAIFocusPriority::Gameplay);
			Blackboard -> SetValueAsVector(BB_NextLocation, NextWayPoint);
			Blackboard -> SetValueAsObject(BB_CurrentTarget, nullptr);
		}

		bIsPatrolling = true;
	}
}

bool AAICharacterController::IsTargetReached(FVector TargetLocation) const
{
	return (TargetLocation - BaseCharacter -> GetActorLocation()).SizeSquared() <= FMath::Square(TargetReachRadius);
}

void AAICharacterController::WeaponAmmoAmountEnded()
{
	if (IsValid(Blackboard))
	{
		Blackboard -> SetValueAsBool(BB_IsAmmoEnded, true);
	}
}