// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "LestaCharacterAnimInstance.h"
#include "LestaStart/Game/Common/Deadable.h"
#include "LestaStart/Game/Common/HealthComponent.h"
#include "LestaStart/Game/Weapon/Weapon.h"
#include "LestaStart/UI/HealthbarWidgetComponent.h"
#include "LestaCharacter.generated.h"

class UCameraComponent;

/** Base Character class for the Lesta Start project. */
UCLASS()
class LESTASTART_API ALestaCharacter : public ACharacter, public IDeadable
{
	GENERATED_BODY()

	// You can extend this class if required
	// Blueprint Character class is derived from the ALestaCharacter

public:
	ALestaCharacter();
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	float GetHealth();
	UFUNCTION(BlueprintCallable)
	float GetMaxHealth();

protected:
	UPROPERTY(VisibleAnywhere, Replicated)
	TObjectPtr<UCameraComponent> CameraComponent;
	UPROPERTY(VisibleAnywhere, Replicated)
	TObjectPtr<UHealthComponent> HealthComponent;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_AttachedWeapon)
	TObjectPtr<AWeapon> AttachedWeapon;
	UPROPERTY(VisibleAnywhere, Replicated)
	TObjectPtr<AWeapon> FirstWeapon; 
	UPROPERTY(VisibleAnywhere, Replicated)
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

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> FirstWeaponInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> SecondWeaponInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> NextWeaponInputAction;
	
	UPROPERTY(VisibleAnywhere)
	FName WeaponSocketName = FName("weapon_socket");
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UHealthbarWidgetComponent> SimulatedPlayerHealthbarWidgetComponent;
	
	FTransform WeaponSocketTransform;

	UPROPERTY()
	ULestaCharacterAnimInstance* AnimInstance;
	
	virtual void BeginPlay() override;
	void SpawnInitialWeapons();
	UFUNCTION(Server, Reliable)
	void ServerRegisterPlayer();
	virtual void Destroyed() override;
	
	virtual void OnMoveInput(const FInputActionInstance& InputActionInstance);
	virtual void OnLookInput(const FInputActionInstance& InputActionInstance);
	virtual void OnShootInput(const FInputActionInstance& InputActionInstance);
	virtual void OnNextWeaponInput(const FInputActionInstance& InputActionInstance);
	UFUNCTION(Server, Reliable)
	void ServerUpdateCharacterPitch(float ControlPitch, float CameraPitch);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastUpdateCharacterPitch(float ControlPitch, float CameraPitch);
	UFUNCTION(Server, Reliable)
	void ServerPullTrigger();
	UFUNCTION(Server, Reliable)
	void ServerReleaseTrigger();
	virtual void OnFirstWeaponInput(const FInputActionInstance& InputActionInstance);
	virtual void OnSecondWeaponInput(const FInputActionInstance& InputActionInstance);

	virtual void OnHealthChanged(float CurrentHealth);
	virtual void OnDie() override;
	UFUNCTION(Server, Reliable)
	void ServerHandleDeath();
	
	UFUNCTION(Client, Reliable)
	void ClientDead();

	AWeapon* SpawnWeapon(TSubclassOf<AWeapon> WeaponClass);
	UFUNCTION(Server, Reliable)
	void ServerAttachWeapon(AWeapon* AttachingWeapon);
	
private:
	void AddBindings();
	void RemoveBindings();

	UFUNCTION()
	void OnRep_AttachedWeapon();
};
