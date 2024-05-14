
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

	UFUNCTION()
	void CreateProjectile(float Damage);
	UFUNCTION()
	virtual void CreateProjectileView(float Damage);
	
	virtual void EnableCreation();
	virtual void DisableCreation();

	void Initialize(TFunction<FTransform()> MuzzleTransformGetter,TFunction<FTransform()> CameraTransformGetter);
	
protected:
	virtual void BeginPlay() override;

	virtual void OnServerProjectileCreation(float Damage);

	UFUNCTION(Server, Reliable)
	void ServerCreateProjectile(float Damage);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastCreateProjectileView(float Damage);
	
	AActor* GetWeaponOwner();

	TFunction<FTransform()> WeaponMuzzleTransformGetter;
	TFunction<FTransform()> PlayerCameraTransformGetter;

	UPROPERTY(Replicated)
	bool IsCreationEnabled;
	
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
