#include "Tower.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "Enemy.h"
#include "Bullet.h"
#include "AIState.h"

Tower::Tower(class Game* game)
	:Actor(game)
{
	SpriteComponent* sc = new SpriteComponent(this, 200);
	sc->SetTexture(game->GetTexture("Assets/Tower.png"));

	mMove = new MoveComponent(this);
	//mMove->SetAngularSpeed(Math::Pi);

	mNextAttack = AttackTime;

	// AIComponent‚ðì¬
	mAIC = new TowerComponent(this);

	// ó‘Ô‚ðAIComponent‚É“o˜^
	mAIC->RegisterState(new AIPatrol(mAIC));
	mAIC->RegisterState(new AIAttack(mAIC));

	// Patrol‚É‰Šú‰»
	mAIC->ChangeState("Patrol");
}

void Tower::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	mNextAttack -= deltaTime;
	//if (canAttack())
	//{
	//	if(isAttack())mNextAttack += AttackTime;
	//}
}
void Tower::ResetMNextAttack()
{
	mNextAttack = AttackTime;
}
bool Tower::CanAttack()
{
	return mNextAttack <= 0.0f;
}
bool Tower::IsAttack()
{
	Enemy* e = GetGame()->GetNearestEnemy(GetPosition());
	if (e != nullptr)
	{
		// Vector from me to enemy
		Vector2 dir = e->GetPosition() - GetPosition();
		float dist = dir.Length();
		if (dist < AttackRange)
		{
			// Rotate to face enemy
			SetRotation(Math::Atan2(-dir.y, dir.x));
			// Spawn bullet at tower position facing enemy
			Bullet* b = new Bullet(GetGame());
			b->SetPosition(GetPosition());
			b->SetRotation(GetRotation());
		}
		return true;
	}
	return false;
}