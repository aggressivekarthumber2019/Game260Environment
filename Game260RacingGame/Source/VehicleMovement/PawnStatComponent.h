// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PawnStat.h"
#include "PawnStatMod.h"
#include "PawnStatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VEHICLEMOVEMENT_API UPawnStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPawnStatComponent();

protected:
	UPROPERTY(BlueprintReadWrite)
		UPawnStat* MBasePawnStat;

	// Called when the game starts
	virtual void BeginPlay() override;

private:
	struct UStatModTracker
	{
		/** \brief	Current number of stacks */
		int32 MStackCount;

		/** \brief	The modifier time remain in milliseconds */
		float MModTimeRemainMS;

		/** \brief	The modifier */
		const UPawnStatMod* MMod;
	};

	TMap<int32, UStatModTracker> MStatModifiers;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPawnStat* GetCurrentStat() const;

	void EnableMod(const UPawnStatMod* Mod);

	void DisableMod(const UPawnStatMod* Mod);
};
