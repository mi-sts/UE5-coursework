
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponProjectileFactory.generated.h"


UCLASS(Abstract)
class LESTASTART_API UWeaponProjectileFactory : public UActorComponent
{
	GENERATED_BODY()

public:
	UWeaponProjectileFactory();
	virtual void StartCreation()
	PURE_VIRTUAL(UWeaponProjectileFactory::StartCreation);
	virtual void StopCreation()
	PURE_VIRTUAL(UWeaponProjectileFactory::StopCreation);

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
