// Fill out your copyright notice in the Description page of Project Settings.

#include "PawnStatComponent.h"


// Sets default values for this component's properties
UPawnStatComponent::UPawnStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPawnStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPawnStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

UPawnStat* UPawnStatComponent::GetCurrentStat() const
{
	return MBasePawnStat;
}

void UPawnStatComponent::EnableMod(const UPawnStatMod* Mod)
{
	if (MStatModifiers.Contains(Mod->GetGUID())) // Already has this mod
	{
		auto ModTracker = &MStatModifiers[Mod->GetGUID()];
		auto CurrentStackCount = ModTracker->MStackCount;
		auto MaxStackCount = ModTracker->MMod->MMaxStackCount;

		// Can still be stacked
		if (CurrentStackCount < MaxStackCount)
		{
			++ModTracker->MStackCount;
		}
	}
	else
	{
		UStatModTracker ModTracker;

		ModTracker.MMod = Mod;
		ModTracker.MModTimeRemainMS = ModTracker.MMod->MMaxTimeMS;
		ModTracker.MStackCount = 1;

		MStatModifiers.Emplace(Mod->GetGUID(), MoveTemp(ModTracker));
	}

	MBasePawnStat->AcceptEnableMod(Mod);
}

void UPawnStatComponent::DisableMod(const UPawnStatMod* Mod)
{
	if (MStatModifiers.Contains(Mod->GetGUID())) // Already has this mod
	{
		auto ModTracker = &MStatModifiers[Mod->GetGUID()];

		if (ModTracker->MStackCount == 1) // Last stack
		{
			MBasePawnStat->AcceptDisableMod(Mod);

			MStatModifiers.Remove(Mod->GetGUID());
		}
		else // Remove 1 stack
		{
			ModTracker->MStackCount--;
		}
	}
}

