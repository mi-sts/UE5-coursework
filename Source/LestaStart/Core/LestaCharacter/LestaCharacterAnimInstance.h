
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "LestaCharacterAnimInstance.generated.h"

UCLASS()
class LESTASTART_API ULestaCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	float GetPitch();
	UFUNCTION(BlueprintCallable)
	void SetPitch(float Value);
	
private:
	float Pitch = 0.0f;
};
