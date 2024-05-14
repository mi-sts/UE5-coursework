
#pragma once

#include "CoreMinimal.h"
#include "WeaponProjectileFactory.h"
#include "SphereWeaponProjectileFactory.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LESTASTART_API USphereWeaponProjectileFactory : public UWeaponProjectileFactory
{
	GENERATED_BODY()

public:
	USphereWeaponProjectileFactory();

	void Initialize(float SphereDamageRadius);

protected:
	virtual void BeginPlay() override;
	virtual void CreateProjectileView(float Damage) override;
	virtual void OnServerProjectileCreation(float Damage) override;

	float DamageRadius;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
