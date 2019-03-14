// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnStatMod.h"
#include "SpeedCapMod.generated.h"

/**
 * 
 */
UCLASS(editinlinenew)
class VEHICLEMOVEMENT_API USpeedCapMod : public UPawnStatMod
{
	GENERATED_BODY()
	
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		/** \brief	The amount of boosted speed */
		float MBoostedSpeed;

	virtual void EnableMod(UCarStat* StatType) const override;


	virtual void DisableMod(UCarStat* StatType) const override;

};
