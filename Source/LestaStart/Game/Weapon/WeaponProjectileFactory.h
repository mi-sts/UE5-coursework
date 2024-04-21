
#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/ActorComponent.h"
#include "WeaponProjectileFactory.generated.h"


UCLASS(Abstract)
class LESTASTART_API UWeaponProjectileFactory : public UActorComponent
{
	GENERATED_BODY()

public:
	UWeaponProjectileFactory();
	
	virtual void CreateProjectile()
	PURE_VIRTUAL(UWeaponProjectileFactory::CreateProjectile);

	void Initialize(TFunction<FTransform()> MuzzleTransformGetter,TFunction<FTransform()> CameraTransformGetter);
	
protected:
	virtual void BeginPlay() override;

	TFunction<FTransform()> WeaponMuzzleTransformGetter;
	TFunction<FTransform()> PlayerCameraTransformGetter;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
