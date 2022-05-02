#pragma once
#include "Actor.h"
#include "EnemyComponent.h"

class Enemy : public Actor
{
public:
	Enemy(class Game* game);
	~Enemy();
	void UpdateActor(float deltaTime) override;
	bool IsReach();
	class CircleComponent* GetCircle() { return mCircle; }
private:
	class CircleComponent* mCircle;
	class EnemyComponent* mAIC;
};