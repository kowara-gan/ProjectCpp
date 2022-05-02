#include "Bullet.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "CircleComponent.h"
#include "Game.h"
#include "Enemy.h"

Bullet::Bullet(class Game* game)
	:Actor(game)
{
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Projectile.png"));

	MoveComponent* mc = new MoveComponent(this);
	mc->SetForwardSpeed(400.0f);

	mCircle = new CircleComponent(this);
	mCircle->SetRadius(5.0f);

	mLiveTime = 1.0f;
}

void Bullet::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	// 敵との衝突を確認する
	for (Enemy* e : GetGame()->GetEnemies())
	{
		if (Intersect(*mCircle, *(e->GetCircle())))
		{
			// 衝突時両方とも消去
			e->SetState(EDead);
			SetState(EDead);
			break;
		}
	}

	mLiveTime -= deltaTime;
	if (mLiveTime <= 0.0f)
	{
		// 時間切れにより消去
		SetState(EDead);
	}
}