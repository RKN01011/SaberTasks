// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AmmoWidget.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "BaseCharacter.h"

#include "PlayerHUDWidget.generated.h"

class UReticleWidget;
class UCharacterAttributesComponent;
class APlayerCharacter;
/**
 * 
 */
UCLASS()
class CHARACTERS_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init(APlayerCharacter* Character);
	
	void SetOwnerCharacter(ABaseCharacter* Character)
	{ CurrentCharacter = Character;  };

protected:
	TWeakObjectPtr<ABaseCharacter> CurrentCharacter = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Widget names")
	FName AmmoWidgetName;
	
	UFUNCTION(BlueprintCallable)
	ESlateVisibility HasPickUpWeapon() const
	{ return bIsEquipWeapon ? ESlateVisibility::Visible : ESlateVisibility::Hidden; };
	
	UFUNCTION(BlueprintCallable)
	float GetHealthPercent() const;
	
	UFUNCTION(BlueprintCallable)
	float GetStaminaPercent() const;

private:
	TWeakObjectPtr<UAmmoWidget> AmmoWidget = nullptr;
	
	bool bIsEquipWeapon = false;
	
	void InitChildrenWidget(APlayerCharacter* Character);
};