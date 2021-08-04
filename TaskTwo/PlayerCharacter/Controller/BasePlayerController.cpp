// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerController.h"

#include "Blueprint/UserWidget.h"

void ABasePlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	Character = StaticCast<ABaseCharacter*>(InPawn);

	if (IsValid(InPawn) && IsLocalController())
		InitWidget();
}

void ABasePlayerController::InitWidget()
{
	UPlayerHUDWidget* PlayerHUDWidget = CreateWidget<UPlayerHUDWidget>(GetWorld(), PlayerHUDWidgetClass);
	if (IsValid(PlayerHUDWidget))
		PlayerHUDWidget -> Init(Character.Get());
}
