
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthbarWidget.generated.h"

UCLASS()
class LESTASTART_API UHealthbarWidget : public UUserWidget
{
	GENERATED_BODY()

	UHealthbarWidget(const FObjectInitializer& ObjectInitializer);

public:
	UFUNCTION(BlueprintCallable)
	void SetHealthbarPercent(float Percent);
	UFUNCTION(BlueprintCallable)
	float GetHealthbarPercent();

protected:
	float HealthbarPercent;
};
