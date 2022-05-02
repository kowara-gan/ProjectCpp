#pragma once
#include "Component.h"
#include <unordered_map>
#include <string>

class AIComponent : public Component
{
public:
	AIComponent(class Actor* owner);

	void Update(float deltaTime) override;
	void ChangeState(const std::string& name);

	// �V������Ԃ�o�^
	void RegisterState(class AIState* state);
private:
	// ��Ԃ̖��O�ƃC���X�^���X��Ή��Â���}�b�v
	std::unordered_map<std::string, class AIState*> mStateMap;
	// ���݂̏��
	class AIState* mCurrentState;
};