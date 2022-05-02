#include "Enemy.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "NavComponent.h"
#include "Grid.h"
#include "Tile.h"
#include "CircleComponent.h"
#include "AIState.h"
#include <algorithm>

Enemy::Enemy(class Game* game)
	:Actor(game)
{
	// Add to enemy vector
	game->GetEnemies().emplace_back(this);

	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Airplane.png"));
	// スタート位置を設定する
	SetPosition(GetGame()->GetGrid()->GetStartTile()->GetPosition());
	// Setup a nav component at the start tile
	NavComponent* nc = new NavComponent(this);
	nc->SetForwardSpeed(150.0f);
	nc->StartPath(GetGame()->GetGrid()->GetStartTile());
	// 衝突判定用のコンポーネント
	mCircle = new CircleComponent(this);
	mCircle->SetRadius(25.0f);

	// AIComponentを作成
	mAIC = new EnemyComponent(this);

	// 状態をAIComponentに登録
	mAIC->RegisterState(new AIMove(mAIC));
	mAIC->RegisterState(new AIDeath(mAIC));

	// Patrolに初期化
	mAIC->ChangeState("Move");
}

Enemy::~Enemy()
{
	// 敵の配列から除く
	auto iter = std::find(GetGame()->GetEnemies().begin(),
		GetGame()->GetEnemies().end(),
		this);
	GetGame()->GetEnemies().erase(iter);
}

void Enemy::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	//if (isReach())SetState(EDead);
}
bool Enemy::IsReach()
{
	// 最終タイルに近いか返す
	Vector2 diff = GetPosition() - GetGame()->GetGrid()->GetEndTile()->GetPosition();
	if (Math::NearZero(diff.Length(), 10.0f))
	{
		return true;
	}
	return false;
}