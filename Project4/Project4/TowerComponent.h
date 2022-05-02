#pragma once
#include "AIComponent.h"
#include <cstdint>

class TowerComponent : public AIComponent
{
public:
	// Lower update order to update first
	TowerComponent(class Actor* owner);
	bool CanAttack();

	bool IsAttack();

	void Reload();

};