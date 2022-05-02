#pragma once
#include "Actor.h"
#include "TowerComponent.h"

class Tower : public Actor
{
public:
	Tower(class Game* game);
	void UpdateActor(float deltaTime) override;
	void ResetMNextAttack();
	bool CanAttack();
	bool IsAttack();
private:
	class MoveComponent* mMove;
	class TowerComponent* mAIC;
	float mNextAttack;
	const float AttackTime = 2.5f;
	const float AttackRange = 100.0f;
};
