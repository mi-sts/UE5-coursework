

#include "PlayerUIWidget.h"


UPlayerUIWidget::UPlayerUIWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	HealthbarPercent = 1.0f;
}

void UPlayerUIWidget::SetHealthbarPercent(float Percent)
{
	if (Percent < 0.0f || Percent > 1.0f)
	{
		UE_LOG(LogInput, Error, TEXT("The installing healthbar percent value is not correct!"));
		return;
	}

	HealthbarPercent = Percent;
}

float UPlayerUIWidget::GetHealthbarPercent()
{
	return HealthbarPercent;
}
