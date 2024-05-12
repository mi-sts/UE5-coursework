// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthbarWidget.h"

UHealthbarWidget::UHealthbarWidget(
	const FObjectInitializer& ObjectInitializer
) : Super(ObjectInitializer)
{
	HealthbarPercent = 1.0f;
}

void UHealthbarWidget::SetHealthbarPercent(float Percent)
{
	if (Percent < 0.0f || Percent > 1.0f)
	{
		UE_LOG(LogInput, Error, TEXT("The installing healthbar percent value is not correct!"));
		return;
	}

	HealthbarPercent = Percent;
}

float UHealthbarWidget::GetHealthbarPercent()
{
	return HealthbarPercent;
}
