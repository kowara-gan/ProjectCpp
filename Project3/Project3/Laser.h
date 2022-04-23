#pragma once
#include "Actor.h"
class Laser : public Actor
{
public:
	Laser(class Game* game);

	void Shot();

	void UpdateActor(float deltaTime) override;
private:
	class CircleComponent* mCircle;
	float mDeathTimer;
};
