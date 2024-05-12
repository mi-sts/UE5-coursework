
#include "HealthComponent.h"

#include "LestaStart/Core/LestaCharacter.h"
#include "Net/UnrealNetwork.h"

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

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UHealthComponent, Health);
	DOREPLIFETIME(UHealthComponent, MaxHealth);
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
	if (DamageCauser == DamagedActor || DamageCauser == GetOwner())
		return;
	
	float DamageTaken = FGenericPlatformMath::Min(Health, Damage);
	SetHealth(Health - DamageTaken);
}

void UHealthComponent::OnRep_HealthIsReplicated()
{
	HealthChangedEvent.Broadcast(Health);
}


