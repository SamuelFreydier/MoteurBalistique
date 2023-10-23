#pragma once

#include "Vector.h"
#include "Engine.h"

class MouseDragger
{
	private:
		bool isCurrentlyDragging;
		Vector3 startMousePosition;
		Vector3 startThingPosition;

		float particleMass = 5;
		float particleRadius = 5;

	public:
		MouseDragger();
		MouseDragger(const Vector3& newStartMousePosition, const Vector3& newStartThingPosition = Vector3());
		MouseDragger(const Vector3& newStartMousePosition, const float& newParticleRadius = 5);
		void draggingIsOver() { isCurrentlyDragging = false; };
		void drawDragger() const;

		const bool isDragging() const { return isCurrentlyDragging; };
		const Vector3 getStartMousePosition() const { return startMousePosition; };
		const Vector3 getStartThingPosition() const { return startThingPosition; };
		const float& getParticleMass() const { return particleMass; };
		const float& getParticleRadius() const { return particleRadius; };

		void changeParticleMass(const ofMouseEventArgs& mouse);
		const bool isDraggingBig() const;
};

