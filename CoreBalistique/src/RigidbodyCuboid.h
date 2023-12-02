#pragma once

#include "Rigidbody.h"

class RigidbodyCuboid : public Rigidbody
{
private:
	float m_width;
	float m_depth;
	float m_height;
	std::vector<Vector3> m_localVertices;

public:
	RigidbodyCuboid(const float width, const float depth, const float height, const float& mass = 1.0, const Vector3& velocity = Vector3({ 0.0, 0.0, 0.0 }), const Vector3& position = Vector3({ 0.0, 0.0, 0.0 }), const Vector3& angularVelocity = Vector3({ 0.0, 0.0, 0.0 }), const Vector3& color = Vector3({ 255, 0, 0 }));
	
	std::vector<Vector3> getVerticesGlobalPosition() const override;
	
	void draw() const override;
};

