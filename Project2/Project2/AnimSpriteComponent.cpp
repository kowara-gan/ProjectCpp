#include "AnimSpriteComponent.h"
#include "Math.h"
#include <iostream>

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder)
	, mCurrFrame(0.0f)
	, mAnimFPS(12.0f)
	, mAnimNumBeg(0)
	, mAnimNumLast(0)
{
}

void AnimSpriteComponent::Update(float deltaTime)
{
	SpriteComponent::Update(deltaTime);

	if (mAnimTextures.size() > 0)
	{
		// �t���[�����[�g�ƃf���^�^�C���Ɋ�Â���
		// �J�����g�t���[�����X�V����
		mCurrFrame += mAnimFPS * deltaTime;

		// ���[�v�����Ȃ��A�j���[�V�����͎~�߂�
		if (mLoopFlag == false) {
			if(mCurrFrame >= mAnimNumLast - (mAnimNumBeg - 1))mCurrFrame = mAnimNumLast - (mAnimNumBeg);
			if (mLoopFlag == false)std::cout << static_cast<int>(mCurrFrame) << "\n";
		}
		else {
			// �K�v�ɉ����ăJ�����g�t���[���������߂�
			while (mCurrFrame >= mAnimNumLast - (mAnimNumBeg - 1))
			{
				mCurrFrame -= (mAnimNumLast - (mAnimNumBeg - 1));
			}
		}
		
		// �����_�ł̃e�N�X�`����ݒ肷��
		SetTexture(mAnimTextures[static_cast<int>(mCurrFrame) + (mAnimNumBeg - 1)]);
	}
}

void AnimSpriteComponent::SetAnimTextures(const std::vector<SDL_Texture*>& textures,int beg, int last, bool loop_flag)
{
	//�f�t�H���g�����̏ꍇ���ׂĂ͈̔͂��A�j���[�V�����Ƃ���
	if (last == -1)last = textures.size();
	SetAnimNum(beg, last, loop_flag);
	mAnimTextures = textures;
	// �A�N�e�B�u�ȃe�N�X�`�����ŏ��̃t���[���ɐݒ肷��
	if (mAnimTextures.size() > 0)
	{
		SetTexture(mAnimTextures[0]);
	}
}
void AnimSpriteComponent::SetAnimNum( int beg, int last, bool loop_flag)
{
	mCurrFrame = 0.0f;
	mAnimNumBeg = beg;
	mAnimNumLast = last;
	// ���[�v���邩�ۂ��̔����ǉ�
	mLoopFlag = loop_flag;
}