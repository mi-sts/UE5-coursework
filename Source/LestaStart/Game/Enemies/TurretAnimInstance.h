
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TurretAnimInstance.generated.h"

UCLASS(Transient, Blueprintable, HideCategories=AnimInstance, BlueprintType)
class LESTASTART_API UTurretAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FRotator TurretRotation = FRotator(0.0, 0.0, 0.0);
};
