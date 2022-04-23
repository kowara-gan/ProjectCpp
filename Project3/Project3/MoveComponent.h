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
	// 回転を制御する (ラジアン / 秒)
	float mAngularSpeed;
	// 前進運動を制御する 単位長 / 秒 = 速度
	//float mForwardSpeed;

	// 質量
	float mMass;
	// 力
	Vector2 mForces;
	// 速度
	Vector2 mVelocity;
};