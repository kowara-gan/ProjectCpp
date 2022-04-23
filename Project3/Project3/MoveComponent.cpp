#include "MoveComponent.h"
#include "Actor.h"

MoveComponent::MoveComponent(class Actor* owner, int updateOrder)
	:Component(owner, updateOrder)
	, mAngularSpeed(0.0f)
	//, mForwardSpeed(0.0f)
	, mMass(1.0f)
	, mForces(Vector2::Zero)
{

}

void MoveComponent::Update(float deltaTime)
{
	// ��]�X�s�[�h��0�ɋ߂���Ίp�x���X�V���Ȃ�
	if (!Math::NearZero(mAngularSpeed))
	{
		float rot = mOwner->GetRotation();
		rot += mAngularSpeed * deltaTime;
		mOwner->SetRotation(rot);
	}
	//�����x���͂̍��v/����
	Vector2 acceleration = mForces * (1 / mMass);
	//���x���X�V
	mVelocity += acceleration * deltaTime;
	//�͂��N���A
	mForces = Vector2::Zero;

	// �O�i�X�s�[�h��0�ɋ߂���Έʒu���X�V���Ȃ�
	//if (!Math::NearZero(mForwardSpeed))
	if (!Math::NearZero(mVelocity.LengthSq()))
	{
		//�ʒu���X�V
		Vector2 pos = mOwner->GetPosition();
		pos += mVelocity * deltaTime;
		// (Screen wrapping code only for asteroids)
		if (pos.x < 0.0f) { pos.x = 1022.0f; }
		else if (pos.x > 1024.0f) { pos.x = 2.0f; }

		if (pos.y < 0.0f) { pos.y = 766.0f; }
		else if (pos.y > 768.0f) { pos.y = 2.0f; }

		mOwner->SetPosition(pos);
	}
}

void MoveComponent::AddForce(Vector2& force)
{
	mForces = force;
}