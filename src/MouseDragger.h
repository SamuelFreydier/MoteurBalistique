#pragma once

#include "Vector.h"
#include "Engine.h"

class MouseDragger
{
	private:
		bool isCurrentlyDragging;
		Vector startMousePosition;
		Vector startThingPosition;

		float particleMass = 5;
		float particleRadius = 5;

	public:
		MouseDragger();
		MouseDragger(const Vector& newStartMousePosition, const Vector& newStartThingPosition = Vector());
		MouseDragger(const Vector& newStartMousePosition, const float& newParticleRadius = 5);
		void draggingIsOver() { isCurrentlyDragging = false; };
		void drawDragger() const;

		const bool isDragging() const { return isCurrentlyDragging; };
		const Vector getStartMousePosition() const { return startMousePosition; };
		const Vector getStartThingPosition() const { return startThingPosition; };
		const float& getParticleMass() const { return particleMass; };
		const float& getParticleRadius() const { return particleRadius; };

		void changeParticleMass(const ofMouseEventArgs& mouse);
		const bool isDraggingBig() const;
};

