
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
	
	void CreateProjectile(float Damage);
	
	virtual void EnableCreation();
	virtual void DisableCreation();

	void Initialize(TFunction<FTransform()> MuzzleTransformGetter,TFunction<FTransform()> CameraTransformGetter);
	
protected:
	virtual void BeginPlay() override;

	virtual void OnProjectileCreation(float Damage)
	PURE_VIRTUAL(UWeaponProjectileFactory::CreateProjectile);

	AActor* GetWeaponOwner();

	TFunction<FTransform()> WeaponMuzzleTransformGetter;
	TFunction<FTransform()> PlayerCameraTransformGetter;

	bool IsCreationEnabled;
	
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
