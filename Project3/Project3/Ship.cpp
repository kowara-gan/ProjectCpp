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
	// �X�v���C�g�R���|�[�l���g���쐬
	SpriteComponent* sc = new SpriteComponent(this, 150);
	sc->SetTexture(game->GetTexture("Assets/Ship.png"));

	// �C���v�b�g�R���|�[�l���g�ƃL�[�ƃX�s�[�h���쐬
	mInput = new InputComponent(this);
	mInput->SetForwardKey(SDL_SCANCODE_W);
	mInput->SetBackKey(SDL_SCANCODE_S);
	mInput->SetClockwiseKey(SDL_SCANCODE_A);
	mInput->SetCounterClockwiseKey(SDL_SCANCODE_D);
	mInput->SetMaxForwardSpeed(300.0f);
	mInput->SetMaxAngularSpeed(Math::TwoPi);

	// �T�[�N���R���|�[�l���g���쐬
	mCircle = new CircleComponent(this);
	mCircle->SetRadius(40.0f);
}

void Ship::UpdateActor(float deltaTime)
{
	mLaserCooldown -= deltaTime;
	// ��Ԃ�EActive�Ȃ�Փ˔�����v�Z
	if (GetState() == EActive)
	{
		// �A�X�e���C�h�ƏՓ˂������𔻒�
		for (auto ast : GetGame()->GetAsteroids())
		{
			if (Intersect(*mCircle, *(ast->GetCircle())))
			{
				// �Փ˂������Ԃ�EPaused�ɐݒ�
				SetState(EPaused);
				mShipCooldown = 2.0f;
				return;
			}
		}
	}

	// ��Ԃ�EPaused�Ȃ烊�X�|�[�����邩���v�Z
	if (GetState() == EPaused)
	{
		// ���X�|�[���̃N�[���_�E�����Ԃ��v�Z
		mShipCooldown -= deltaTime;

		// �N�[���_�E�����ԂɂȂ�����ship�������ʒu�E�p�x�Ƀ��X�|�[�� 
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
	//���[�U�[�̃N�[���^�C������
	if (keyState[SDL_SCANCODE_SPACE] && mLaserCooldown <= 0.0f)
	{
		//�@���[�U�[�𐶐�
		Laser* laser = new Laser(GetGame());
		laser->SetPosition(GetPosition());
		laser->SetRotation(GetRotation());
		laser->Shot();
		// ���[�U�[�̃N�[���^�C����ݒ�
		mLaserCooldown = 0.5f;
	}
}