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
	// 回転スピードが0に近ければ角度を更新しない
	if (!Math::NearZero(mAngularSpeed))
	{
		float rot = mOwner->GetRotation();
		rot += mAngularSpeed * deltaTime;
		mOwner->SetRotation(rot);
	}
	//加速度＝力の合計/質量
	Vector2 acceleration = mForces * (1 / mMass);
	//速度を更新
	mVelocity += acceleration * deltaTime;
	//力をクリア
	mForces = Vector2::Zero;

	// 前進スピードが0に近ければ位置を更新しない
	//if (!Math::NearZero(mForwardSpeed))
	if (!Math::NearZero(mVelocity.LengthSq()))
	{
		//位置を更新
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