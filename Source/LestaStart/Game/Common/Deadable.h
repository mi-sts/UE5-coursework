
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Deadable.generated.h"

UINTERFACE()
class UDeadable : public UInterface
{
	GENERATED_BODY()
};

class LESTASTART_API IDeadable
{
	GENERATED_BODY()

public:
	IDeadable();
	void Die();
	bool IsDead();

protected:
	bool Dead;

	virtual void OnDie() = 0;
};
