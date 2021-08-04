// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUDWidget.h"

#include "Components/CharacterEquipmentComponent.h"
#include "PlayerCharacter/PlayerCharacter.h"

#define DEFAULT_PERCENT 1.f

void UPlayerHUDWidget::Init(APlayerCharacter* Character)
{
	SetOwnerCharacter(Character);
	InitChildrenWidget(Character);
	AddToViewport();
	
	UCharacterEquipmentComponent* CharacterEquipmentComponent = Character -> GetEquipmentComponent();
	CharacterEquipmentComponent -> OnEquipWeapon.AddLambda([this](bool Value) {bIsEquipWeapon = Value;});
}

float UPlayerHUDWidget::GetHealthPercent() const
{
	return CurrentCharacter.IsValid() ? CurrentCharacter -> GetAttributesComponent() -> GetHealthPercent()
		: DEFAULT_PERCENT;
}

float UPlayerHUDWidget::GetStaminaPercent() const
{
	return CurrentCharacter.IsValid() ? CurrentCharacter -> GetAttributesComponent() -> GetStaminaPercent()
		: DEFAULT_PERCENT;
}

void UPlayerHUDWidget::InitChildrenWidget(APlayerCharacter* Character)
{
	UCharacterEquipmentComponent* CharacterEquipmentComponent = Character -> GetEquipmentComponent();
	
	AmmoWidget = WidgetTree -> FindWidget<UAmmoWidget>(AmmoWidgetName);
	if (AmmoWidget.IsValid())
	{
		CharacterEquipmentComponent -> OnCurrentWeaponAmmoChangeEvent.AddUFunction(
			AmmoWidget.Get(), FName{"UpdateAmmoCount"});
	}
}
