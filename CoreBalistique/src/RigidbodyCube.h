#ifndef RIGIDBODYCUBE_H
#define RIGIDBODYCUBE_H

#include "Rigidbody.h"

class RigidbodyCube : public Rigidbody
{
private:
	float m_size;

public:
	RigidbodyCube(const float size, const float& mass = 1.0, const Vector3& velocity = Vector3({ 0.0, 0.0, 0.0 }), const Vector3& position = Vector3({ 0.0, 0.0, 0.0 }), const Vector3& angularVelocity = Vector3({ 0.0, 0.0, 0.0 }), const Vector3& color = Vector3({ 255, 0, 0 }));

	const float& getSize() const { return m_size; }
	void setSize(const float& size) { m_size = size; }

	void draw() const override;
};

#endif