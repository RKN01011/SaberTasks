// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoWidget.h"

void UAmmoWidget::UpdateAmmoCount(int32 Ammo, int32 TotalAmmo)
{
	CurrentAmmo = Ammo;
	CurrentTotalAmmo = TotalAmmo;
}
