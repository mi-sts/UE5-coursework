// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class LESTASTART_API TransformUtils
{
public:
	static FRotator CalculateLookAtRotator(FVector RotatedLocation, FVector LookAtLocation)
	{
		FVector RotatingToLookAtActorDirection = LookAtLocation - RotatedLocation;
		RotatingToLookAtActorDirection.Z = 0.0f;
		return FRotationMatrix::MakeFromX(RotatingToLookAtActorDirection).Rotator();
	}
};
