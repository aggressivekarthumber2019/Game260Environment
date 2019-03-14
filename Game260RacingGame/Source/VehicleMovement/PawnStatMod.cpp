// Fill out your copyright notice in the Description page of Project Settings.

#include "PawnStatMod.h"

void UPawnStatMod::Construct(int32 GUID, int32 MaxStack, float MaxTimeMS)
{
	MGUID = GUID;
	MMaxStackCount = MaxStack;
	MMaxTimeMS = MaxTimeMS;
}

int32 UPawnStatMod::GetGUID() const
{
	return MGUID;
}
