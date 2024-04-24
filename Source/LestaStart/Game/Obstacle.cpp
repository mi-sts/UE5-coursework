
#include "Obstacle.h"


AObstacle::AObstacle()
{
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(FName("HealthComponent"));
}

void AObstacle::BeginPlay()
{
	Super::BeginPlay();
	HealthComponent->OnHealthChanged().AddUObject(this, &AObstacle::OnHealthChanged);
}

void AObstacle::Destroyed()
{
	Super::Destroyed();
	HealthComponent->OnHealthChanged().RemoveAll(this);
}

void AObstacle::OnHealthChanged(float CurrentHealth)
{
	UE_LOG(LogInput, Log, TEXT("Health %f"), CurrentHealth);
	if (CurrentHealth <= 0.0f)
		Destroy();
}
