
#include "Deadable.h"

IDeadable::IDeadable() : Dead(false)
{
}

void IDeadable::Die()
{
	if (Dead)
		return;

	OnDie();
	Dead = true;
}

bool IDeadable::IsDead()
{
	return Dead;
}
