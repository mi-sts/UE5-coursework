// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "LestaStart/Game/Common/HealthComponent.h"
#include "LestaStart/Game/Weapon/Weapon.h"
#include "LestaCharacter.generated.h"

class UCameraComponent;

/** Base Character class for the Lesta Start project. */
UCLASS()
class LESTASTART_API ALestaCharacter : public ACharacter
{
	GENERATED_BODY()

	// You can extend this class if required
	// Blueprint Character class is derived from the ALestaCharacter

public:
	ALestaCharacter();
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	float GetHealth();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCameraComponent> CameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UHealthComponent> HealthComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AWeapon> AttachedWeapon;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AWeapon> FirstWeapon; 
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AWeapon> SecondWeapon; 
	
	/** Input action assigned to movement. */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveInputAction;

	/** Input action assigned to camera movement. */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> LookInputAction;

	/** Input action assigned to player shooting. */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> ShootInputAction;

	UPROPERTY(VisibleAnywhere)
	FName WeaponSocketName = FName("weapon_socket");

	FTransform WeaponSocketTransform;

	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	
	virtual void OnMoveInput(const FInputActionInstance& InputActionInstance);
	virtual void OnLookInput(const FInputActionInstance& InputActionInstance);
	virtual void OnShootInput(const FInputActionInstance& InputActionInstance);

	virtual void OnHealthChanged(float CurrentHealth);
	virtual void OnDead();
	
	void AttachWeapon(AWeapon* AttachingWeapon);
	
private:
	void AddBindings();
	void RemoveBindings();
	
};
