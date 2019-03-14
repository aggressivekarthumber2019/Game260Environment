// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PawnStatMod.h"
#include "PawnStat.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class VEHICLEMOVEMENT_API UPawnStat : public UObject
{
	GENERATED_BODY()

	friend class USpeedCapMod;
	friend class UItemSpeedCapMod;
	
protected:
	UPROPERTY(BlueprintReadWrite)
		/** \brief	The health factor */
		float MHealthFactor;

	UPROPERTY(BlueprintReadWrite)
		/** \brief	The maximum speed factor */
		float MMaxSpeedFactor;

	UPROPERTY(BlueprintReadWrite)
		/** \brief	The minimum speed factor */
		float MMinSpeedFactor;

public:
	UPawnStat() = default;

	UFUNCTION(BlueprintCallable)
	/**
	 * \fn	void UPawnStat::Construct(const float HealthFactor, const float MaxSpeedFactor, const float MinSpeedFactor);
	 *
	 * \brief	Constructs
	 *
	 * \author	Jaymie
	 * \date	2/18/2019
	 *
	 * \param	HealthFactor  	The health factor.
	 * \param	MaxSpeedFactor	The maximum speed factor.
	 * \param	MinSpeedFactor	The minimum speed factor.
	 */
	void ConstructPawnStat(const float HealthFactor, const float MaxSpeedFactor, const float MinSpeedFactor);

	/**
	 * \fn	float UPawnStat::GetHealthFactor() const;
	 *
	 * \brief	Gets health factor
	 *
	 * \author	Jaymie
	 * \date	2/18/2019
	 *
	 * \returns	The health factor.
	 */
	float GetHealthFactor() const;

	/**
	 * \fn	float UPawnStat::GetMaxSpeedFactor() const;
	 *
	 * \brief	Gets maximum speed factor
	 *
	 * \author	Jaymie
	 * \date	2/18/2019
	 *
	 * \returns	The maximum speed factor.
	 */
	float GetMaxSpeedFactor() const;

	/**
	 * \fn	float UPawnStat::GetMinSpeedFactor() const;
	 *
	 * \brief	Gets minimum speed factor
	 *
	 * \author	Jaymie
	 * \date	2/18/2019
	 *
	 * \returns	The minimum speed factor.
	 */
	float GetMinSpeedFactor() const;

	/**
	 * \fn	virtual void UPawnStat::AcceptEnableMod(const UPawnStatMod& Mod) = 0;
	 *
	 * \brief	Visitor for enable modifier
	 *
	 * \author	Jaymie
	 * \date	2/18/2019
	 *
	 * \param	Mod	The modifier.
	 */
	virtual void AcceptEnableMod(const UPawnStatMod* Mod) {};

	/**
	 * \fn	virtual void UPawnStat::AcceptDisableMod(const UPawnStatMod& Mod) = 0;
	 *
	 * \brief	Visitor for disable modifier
	 *
	 * \author	Jaymie
	 * \date	2/18/2019
	 *
	 * \param	Mod	The modifier.
	 */
	virtual void AcceptDisableMod(const UPawnStatMod* Mod) {};
};
