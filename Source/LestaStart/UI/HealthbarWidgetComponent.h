
#pragma once

#include "CoreMinimal.h"
#include "HealthbarWidget.h"
#include "Components/WidgetComponent.h"
#include "HealthbarWidgetComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LESTASTART_API UHealthbarWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UHealthbarWidgetComponent();
	void SetHealthPercent(float Percent);

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UHealthbarWidget> HealthbarWidgetClass;
	UPROPERTY()
	UHealthbarWidget* HealthbarWidget;

	bool VisibleToPlayer;
	
	virtual void BeginPlay() override;
	
private:
	void SetupVisibility();
	void InitializeWidget();
	void UpdateLookAtPlayerRotation();

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
