// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "BaseCharacter.h"
#include "BehaviorTree/BehaviorTree.h"

#include "AICharacter.generated.h"

/**
 * 
 */
class UAIPatrollingComponent;

UCLASS(Blueprintable)
class CHARACTERS_API AAICharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AAICharacter(const FObjectInitializer& ObjectInitializer);

	FVector GetEscapeLocation() const;

	virtual void Death() override;

	UAIPatrollingComponent* GetPatrollingComponent() const;

	UBehaviorTree* GetBehaviorTree() const;

	UPROPERTY(EditAnywhere, meta=(MakeEditWidget))
	FVector EscapeLocation;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UAIPatrollingComponent* PatrollingComponent = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
	UBehaviorTree* BehaviorTree = nullptr;
	
};
