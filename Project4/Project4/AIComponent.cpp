#include "AIComponent.h"
#include "Actor.h"
#include "AIState.h"
#include "SDL_log.h"

AIComponent::AIComponent(class Actor* owner)
	:Component(owner)
	, mCurrentState(nullptr)
{
}

void AIComponent::Update(float deltaTime)
{
	if (mCurrentState)
	{
		mCurrentState->Update(deltaTime);
	}
}

void AIComponent::ChangeState(const std::string& name)
{
	// �ŏ��Ɍ��݂̏�Ԃ��I������
	if (mCurrentState)
	{
		mCurrentState->OnExit();
	}

	// �}�b�v����V������Ԃ�������
	auto iter = mStateMap.find(name);
	if (iter != mStateMap.end())
	{
		mCurrentState = iter->second;
		// �V������Ԃ�ݒ肷��
		mCurrentState->OnEnter();
	}
	else
	{
		SDL_Log("Could not find AIState %s in state map", name.c_str());
		mCurrentState = nullptr;
	}
}

void AIComponent::RegisterState(AIState* state)
{
	mStateMap.emplace(state->GetName(), state);
}