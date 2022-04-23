#include "Laser.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "CircleComponent.h"
#include "Asteroid.h"


Laser::Laser(Game* game)
	:Actor(game)
	, mDeathTimer(1.0f)
{
	// Create a sprite component
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Laser.png"));

	// Create a move component, and set a forward speed
	//MoveComponent* mc = new MoveComponent(this);
	//mc->SetForwardSpeed(800.0f);

	// Create a circle component (for collision)
	mCircle = new CircleComponent(this);
	mCircle->SetRadius(11.0f);
}
void Laser::Shot()
{
	// Create a move component, and set a forward speed
	MoveComponent* mc = new MoveComponent(this);
	Vector2 force = GetForward() * 20000;
	mc->AddForce(force);
}

void Laser::UpdateActor(float deltaTime)
{
	// 1秒後に消える
	mDeathTimer -= deltaTime;
	if (mDeathTimer <= 0.0f)
	{
		SetState(EDead);
	}
	else
	{
		// アステロイドと衝突したかを判定
		for (auto ast : GetGame()->GetAsteroids())
		{
			if (Intersect(*mCircle, *(ast->GetCircle())))
			{
				// 衝突したら状態をEDeadに設定
				SetState(EDead);
				ast->SetState(EDead);
				break;
			}
		}
	}
}