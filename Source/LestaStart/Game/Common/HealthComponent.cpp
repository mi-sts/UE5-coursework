
#include "HealthComponent.h"


UHealthComponent::UHealthComponent(): MaxHealth(100.0f), Health(MaxHealth)
{
	SetHealth(MaxHealth);
}

float UHealthComponent::GetMaxHealth()
{
	return MaxHealth;
}

float UHealthComponent::GetHealth()
{
	return Health;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnTakeDamage);
}

bool UHealthComponent::SetHealth(float Value)
{
	if (Value < 0 || Value > MaxHealth)
		return false;
	
	Health = Value;
	HealthChangedEvent.Broadcast(Health);
	return true;
}

void UHealthComponent::OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                    AController* InstigatedBy, AActor* DamageCauser)
{
	if (DamageCauser == DamagedActor)
		return;

	float DamageTaken = FGenericPlatformMath::Min(Health, Damage);
	SetHealth(Health - DamageTaken);
}


