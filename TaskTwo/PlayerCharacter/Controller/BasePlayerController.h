// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/PlayerController.h"
#include "PlayerCharacter/PlayerCharacter.h"
#include "PlayerCharacter/UI/PlayerHUDWidget.h"

#include "BasePlayerController.generated.h"


/**
 * 
 */
UCLASS()
class CHARACTERS_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void SetPawn(APawn* InPawn) override;
	
protected:
	void InitWidget();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="HUD")
	TSubclassOf<UPlayerHUDWidget> PlayerHUDWidgetClass = nullptr;

private:
	TSoftObjectPtr<APlayerCharacter> Character;
};
