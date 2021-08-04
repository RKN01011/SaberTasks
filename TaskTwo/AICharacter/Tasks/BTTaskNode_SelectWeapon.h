// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Types.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Characters/Components/CharacterEquipmentComponent.h"

#include "BTTaskNode_SelectWeapon.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERS_API UBTTaskNode_SelectWeapon : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskNode_SelectWeapon();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bRemoveWeapon = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EAnimationItemTypes ItemType = EAnimationItemTypes::EE_None;

	
	
};
