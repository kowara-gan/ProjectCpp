#pragma once
#include "Component.h"
#include "Math.h"

#pragma once
#include "Component.h"

class MoveComponent : public Component
{
public:
	// Lower update order to update first
	MoveComponent(class Actor* owner, int updateOrder = 10);

	void Update(float deltaTime) override;
	void AddForce(Vector2& force);

	float GetAngularSpeed() const { return mAngularSpeed; }
	//float GetForwardSpeed() const { return mForwardSpeed; }
	float GetMass() const { return mMass; }
	void SetAngularSpeed(float speed) { mAngularSpeed = speed; }
	//void SetForwardSpeed(float speed) { mForwardSpeed = speed; }
	void SetMass(float mass) { mMass = mass; }

	void ResetVelocity() { mVelocity = Vector2::Zero; }
private:
	// ��]�𐧌䂷�� (���W�A�� / �b)
	float mAngularSpeed;
	// �O�i�^���𐧌䂷�� �P�ʒ� / �b = ���x
	//float mForwardSpeed;

	// ����
	float mMass;
	// ��
	Vector2 mForces;
	// ���x
	Vector2 mVelocity;
};