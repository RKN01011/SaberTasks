// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Fire.h"

#include "AIController.h"
#include "BaseCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CharacterEquipmentComponent.h"

UBTService_Fire::UBTService_Fire()
{
	NodeName = "Fire";
}

void UBTService_Fire::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	if (!IsValid(AIController) || !IsValid(Blackboard))
	{
		return;
	}

	APawn* Pawn = AIController -> GetPawn();
	checkf(Pawn -> IsA<ABaseCharacter>(), TEXT("Only ABaseCharacter"));
	ABaseCharacter* BaseCharacter = StaticCast<ABaseCharacter*>(std::move(Pawn));

	const UCharacterEquipmentComponent* EquipmentComponent = BaseCharacter -> GetEquipmentComponent();
	ARangeWeapon* RangeWeapon = StaticCast<ARangeWeapon*>(EquipmentComponent -> GetCurrentEquipWeapon());

	if (!IsValid(RangeWeapon))
	{
		return;
	}

	AActor* CurrentTarget = Cast<AActor>(Blackboard -> GetValueAsObject(TargetKey.SelectedKeyName));
	if (!IsValid(CurrentTarget))
	{
		if (RangeWeapon -> IsFiring())
		{
			RangeWeapon -> EndFire();
		}
		
		return;
	}

	float DistSq = FVector::DistSquared(CurrentTarget -> GetActorLocation(), BaseCharacter -> GetActorLocation());
	if (DistSq > FMath::Square(MaxFireDistance))
	{
		if (RangeWeapon -> IsFiring())
		{
			RangeWeapon -> EndFire();
		}
		
		return;
	}

	if (RangeWeapon -> IsReloading()) return;

	EEquippedItemFireMode FireMode = RangeWeapon -> GetFireMode();
	if (FireMode == EEquippedItemFireMode::EE_Single)
	{
		RangeWeapon -> BeginFire();
		return;
	}
	
	RangeWeapon -> BeginFire();
}
