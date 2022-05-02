#include "TowerComponent.h"
#include "Tower.h"

TowerComponent::TowerComponent(class Actor* owner)
	:AIComponent(owner)
{

}
bool TowerComponent::CanAttack()
{
	return ((Tower*)mOwner)->CanAttack();

}
bool TowerComponent::IsAttack()
{
	return ((Tower*)mOwner)->IsAttack();

}
void TowerComponent::Reload()
{
	((Tower*)mOwner)->ResetMNextAttack();

}