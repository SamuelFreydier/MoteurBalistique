#pragma once
#include"Rigidbody.h"
class RigidbodyCylinder : public Rigidbody
{
private:
	float m_radius;
	float m_height;

public:
	RigidbodyCylinder(const float radius, const float height, const float& mass = 1.0, const Vector3& velocity = Vector3({ 0.0, 0.0, 0.0 }), const Vector3& position = Vector3({ 0.0, 0.0, 0.0 }), const Vector3& angularVelocity = Vector3({ 0.0, 0.0, 0.0 }), const Vector3& color = Vector3({ 255, 0, 0 }));
	void draw() const override;
};

