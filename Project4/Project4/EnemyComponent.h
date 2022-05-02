#pragma once
#include "AIComponent.h"
#include <cstdint>

class EnemyComponent : public AIComponent
{
public:
	// Lower update order to update first
	EnemyComponent(class Actor* owner);
	bool IsReach();
	void Dead();
};