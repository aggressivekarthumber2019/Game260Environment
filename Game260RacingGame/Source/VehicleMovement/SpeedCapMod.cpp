// Fill out your copyright notice in the Description page of Project Settings.

#include "SpeedCapMod.h"
#include "CarStat.h"




void USpeedCapMod::EnableMod(UCarStat* StatType) const
{
	StatType->MMaxSpeedFactor += MBoostedSpeed;
}

void USpeedCapMod::DisableMod(UCarStat* StatType) const
{
	StatType->MMaxSpeedFactor -= MBoostedSpeed;
}
