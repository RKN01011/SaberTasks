// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_SelectWeapon.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CharacterEquipmentComponent.h"

UBTTaskNode_SelectWeapon::UBTTaskNode_SelectWeapon()
{
	NodeName = "SelectWeapon";
}

EBTNodeResult::Type UBTTaskNode_SelectWeapon::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController	= OwnerComp.GetAIOwner();
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	if (!IsValid(AIController) || !IsValid(Blackboard))
	{
		return EBTNodeResult::Failed;
	}

	ABaseCharacter* BaseCharacter = StaticCast<ABaseCharacter*>(AIController -> GetPawn());
	if (!IsValid(BaseCharacter))
	{
		return EBTNodeResult::Failed;
	}

	UCharacterEquipmentComponent* EquipmentComponent = BaseCharacter -> GetEquipmentComponent();
	
	if (bRemoveWeapon)
		EquipmentComponent -> RemoveWeapon();

	if (ItemType == EAnimationItemTypes::EE_TwoHand)
		EquipmentComponent -> PickTwoHandWeapon();
	else
		EquipmentComponent -> PickOneHandWeapon();
		
	return EBTNodeResult::Succeeded;
}
