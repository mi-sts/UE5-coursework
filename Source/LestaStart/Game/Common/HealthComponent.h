// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LESTASTART_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealthComponent();
	
	DECLARE_EVENT_OneParam(UHealthComponent, FHealthChangedEvent, float);
	FHealthChangedEvent& OnHealthChanged() { return HealthChangedEvent; } 

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth();
	UFUNCTION(BlueprintCallable)
	float GetHealth();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
	bool SetHealth(float Value);

	FHealthChangedEvent HealthChangedEvent;
	
	UFUNCTION()
	virtual void OnTakeDamage(
		AActor* DamagedActor,
		float Damage,
		const UDamageType* DamageType,
		AController* InstigatedBy,
		AActor* DamageCauser
	);

	UPROPERTY(EditAnywhere, Replicated)
	float MaxHealth;
	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_HealthIsReplicated)
	float Health;
	UFUNCTION()
	void OnRep_HealthIsReplicated();
};
