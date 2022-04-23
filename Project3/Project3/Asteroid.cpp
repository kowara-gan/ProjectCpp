#include "Asteroid.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "Random.h"
#include "CircleComponent.h"

Asteroid::Asteroid(Game* game)
	:Actor(game)
	, mCircle(nullptr)
{
	// �����_���Ȉʒu / �����ŏ���������
	Vector2 randPos = Random::GetVector(Vector2::Zero,
		Vector2(1024.0f, 768.0f));
	SetPosition(randPos);

	SetRotation(Random::GetFloatRange(0.0f, Math::TwoPi));

	// �X�v���C�g�R���|�[�l���g���쐬���A�e�N�X�`����ݒ肷��
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Asteroid.png"));

	// �^���R���|�[�l���g���쐬���A�O�i���x��ݒ肷��
	MoveComponent* mc = new MoveComponent(this);
	Vector2 force = GetForward() * 4000;
	mc->AddForce(force);
	//mc->SetForwardSpeed(150.0f);

	// �Փ˔���̂��߂ɃT�[�N���R���|�[�l���g���쐬
	mCircle = new CircleComponent(this);
	mCircle->SetRadius(40.0f);

	//���f�����Q�[���ɒǉ�
	game->AddAsteroid(this);
}

Asteroid::~Asteroid()
{
	GetGame()->RemoveAsteroid(this);
}
