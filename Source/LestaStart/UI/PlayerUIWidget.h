
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerUIWidget.generated.h"

UCLASS()
class LESTASTART_API UPlayerUIWidget : public UUserWidget
{
	GENERATED_BODY()

	UPlayerUIWidget(const FObjectInitializer& ObjectInitializer);

public:
	UFUNCTION(BlueprintCallable)
	void SetHealthbarPercent(float Percent);
	UFUNCTION(BlueprintCallable)
	float GetHealthbarPercent();

protected:
	float HealthbarPercent;
	
};
