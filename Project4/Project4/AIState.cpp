#include "AIState.h"
#include "AIComponent.h"
#include "TowerComponent.h"
#include "SDL_log.h"
#include "Enemy.h"
#include "EnemyComponent.h"
void AIPatrol::Update(float deltaTime)
{
	//SDL_Log("Updating %s state", GetName());
	if(((TowerComponent*)mOwner)->CanAttack())mOwner->ChangeState("Attack");

}

void AIPatrol::OnEnter()
{
	//SDL_Log("Entering %s state", GetName());
}

void AIPatrol::OnExit()
{
	//SDL_Log("Exiting %s state", GetName());
}

void AIAttack::Update(float deltaTime)
{
	//SDL_Log("Updating %s state", GetName());
	if (((TowerComponent*)mOwner)->IsAttack())mOwner->ChangeState("Patrol");
}

void AIAttack::OnEnter()
{
	//SDL_Log("Entering %s state", GetName());
}

void AIAttack::OnExit()
{
	//SDL_Log("Exiting %s state", GetName());
	((TowerComponent*)mOwner)->Reload();
}

void AIMove::Update(float deltaTime)
{
	//SDL_Log("Updating %s state", GetName());
	if (((EnemyComponent*)mOwner)->IsReach())mOwner->ChangeState("Death");
}

void AIMove::OnEnter()
{
	//SDL_Log("Entering %s state", GetName());
}

void AIMove::OnExit()
{
	//SDL_Log("Exiting %s state", GetName());
}

void AIDeath::Update(float deltaTime)
{
	//SDL_Log("Updating %s state", GetName());
}

void AIDeath::OnEnter()
{
	//SDL_Log("Entering %s state", GetName());
	((EnemyComponent*)mOwner)->Dead();
}

void AIDeath::OnExit()
{
	//SDL_Log("Exiting %s state", GetName());
}

