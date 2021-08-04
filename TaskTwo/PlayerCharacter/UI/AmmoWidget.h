// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AmmoWidget.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERS_API UAmmoWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ammo")
	int32 CurrentAmmo;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ammo")
	int32 CurrentTotalAmmo;

private:
	UFUNCTION()
	void UpdateAmmoCount(int32 Ammo, int32 TotalAmmo);
};
