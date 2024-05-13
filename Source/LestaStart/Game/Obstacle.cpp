
#include "Obstacle.h"


AObstacle::AObstacle()
{
	bReplicates = true;
	
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

void AObstacle::OnDie()
{
	Destroy(true);
}

void AObstacle::OnHealthChanged(float CurrentHealth)
{
	if (CurrentHealth <= 0.0f)
		Destroy();
}
