#include "Ship.h"
#include "SpriteComponent.h"
#include "InputComponent.h"
#include "Game.h"
#include "Laser.h"
#include "CircleComponent.h"
#include "Asteroid.h"

Ship::Ship(Game* game)
	:Actor(game)
	, mLaserCooldown(0.0f)
	, mShipCooldown(0.0f)
	, mCircle(nullptr)
{
	// スプライトコンポーネントを作成
	SpriteComponent* sc = new SpriteComponent(this, 150);
	sc->SetTexture(game->GetTexture("Assets/Ship.png"));

	// インプットコンポーネントとキーとスピードを作成
	mInput = new InputComponent(this);
	mInput->SetForwardKey(SDL_SCANCODE_W);
	mInput->SetBackKey(SDL_SCANCODE_S);
	mInput->SetClockwiseKey(SDL_SCANCODE_A);
	mInput->SetCounterClockwiseKey(SDL_SCANCODE_D);
	mInput->SetMaxForwardSpeed(300.0f);
	mInput->SetMaxAngularSpeed(Math::TwoPi);

	// サークルコンポーネントを作成
	mCircle = new CircleComponent(this);
	mCircle->SetRadius(40.0f);
}

void Ship::UpdateActor(float deltaTime)
{
	mLaserCooldown -= deltaTime;
	// 状態がEActiveなら衝突判定を計算
	if (GetState() == EActive)
	{
		// アステロイドと衝突したかを判定
		for (auto ast : GetGame()->GetAsteroids())
		{
			if (Intersect(*mCircle, *(ast->GetCircle())))
			{
				// 衝突したら状態をEPausedに設定
				SetState(EPaused);
				mShipCooldown = 2.0f;
				return;
			}
		}
	}

	// 状態がEPausedならリスポーンするかを計算
	if (GetState() == EPaused)
	{
		// リスポーンのクールダウン時間を計算
		mShipCooldown -= deltaTime;

		// クールダウン時間になったらshipを初期位置・角度にリスポーン 
		if (mShipCooldown <= 0.0f)
		{
			SetPosition(Vector2(512.0f, 384.0f));
			SetRotation(Math::PiOver2);
			SetState(EActive);
			mShipCooldown = 0.0f;
			mInput->ResetVelocity();
		}
	}
}

void Ship::ActorInput(const uint8_t* keyState)
{
	//レーザーのクールタイム処理
	if (keyState[SDL_SCANCODE_SPACE] && mLaserCooldown <= 0.0f)
	{
		//　レーザーを生成
		Laser* laser = new Laser(GetGame());
		laser->SetPosition(GetPosition());
		laser->SetRotation(GetRotation());
		laser->Shot();
		// レーザーのクールタイムを設定
		mLaserCooldown = 0.5f;
	}
}