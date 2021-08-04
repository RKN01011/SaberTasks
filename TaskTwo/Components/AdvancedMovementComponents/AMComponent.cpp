// Fill out your copyright notice in the Description page of Project Settings.


#include "AMComponent.h"
#include "BaseCharacter.h"

void UAMComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = StaticCast<ABaseCharacter*>(GetOwner());
	MovementComponent = StaticCast<UBaseMovementComponent*>(Character -> GetMovementComponent());
}

