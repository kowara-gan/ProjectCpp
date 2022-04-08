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
		// フレームレートとデルタタイムに基づいて
		// カレントフレームを更新する
		mCurrFrame += mAnimFPS * deltaTime;

		// ループさせないアニメーションは止める
		if (mLoopFlag == false) {
			if(mCurrFrame >= mAnimNumLast - (mAnimNumBeg - 1))mCurrFrame = mAnimNumLast - (mAnimNumBeg);
			if (mLoopFlag == false)std::cout << static_cast<int>(mCurrFrame) << "\n";
		}
		else {
			// 必要に応じてカレントフレームを巻き戻す
			while (mCurrFrame >= mAnimNumLast - (mAnimNumBeg - 1))
			{
				mCurrFrame -= (mAnimNumLast - (mAnimNumBeg - 1));
			}
		}
		
		// 現時点でのテクスチャを設定する
		SetTexture(mAnimTextures[static_cast<int>(mCurrFrame) + (mAnimNumBeg - 1)]);
	}
}

void AnimSpriteComponent::SetAnimTextures(const std::vector<SDL_Texture*>& textures,int beg, int last, bool loop_flag)
{
	//デフォルト引数の場合すべての範囲をアニメーションとする
	if (last == -1)last = textures.size();
	SetAnimNum(beg, last, loop_flag);
	mAnimTextures = textures;
	// アクティブなテクスチャを最初のフレームに設定する
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
	// ループするか否かの判定を追加
	mLoopFlag = loop_flag;
}