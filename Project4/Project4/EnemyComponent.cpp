#include "EnemyComponent.h"
#include "Enemy.h"


EnemyComponent::EnemyComponent(class Actor* owner)
	:AIComponent(owner)
{

}
bool EnemyComponent::IsReach()
{
	return ((Enemy*)mOwner)->IsReach();

}
void EnemyComponent::Dead()
{
	return mOwner->SetState(Actor::State::EDead);

}
