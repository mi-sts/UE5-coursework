
#include "DamageSystemUtils.h"

#include "LestaStart/Game/Common/Deadable.h"
#include "LestaStart/Game/Common/HealthComponent.h"

bool DamageSystemUtils::CheckActorDeath(AActor* Actor)
{
	if (!IsValid(Actor))
		return false;
	
	UHealthComponent* ActorHealthComponent = Actor->GetComponentByClass<UHealthComponent>();
	if (!IsValid(ActorHealthComponent))
		return false;

	if (ActorHealthComponent->GetHealth() > 0.0f)
		return false;

	IDeadable* ActorDeadable = Cast<IDeadable>(Actor);
	if (ActorDeadable == nullptr)
		return false;
	ActorDeadable->Die();

	return true;
}
