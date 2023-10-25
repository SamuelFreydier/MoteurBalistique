#pragma once

#include "Vector.h"
#include "Engine.h"

class MouseDragger
{
	private:
		bool m_isCurrentlyDragging;
		Vector3 m_startMousePosition;
		Vector3 m_startThingPosition;

		Engine::Particles m_selectedParticles;

		float m_particleMass = 5;
		float m_particleRadius = 5;

	public:
		MouseDragger();
		MouseDragger(const Vector3& newStartMousePosition, const Vector3& newStartThingPosition = Vector3());
		MouseDragger(const Vector3& newStartMousePosition, const float& newParticleRadius);
		void draggingIsOver() { m_isCurrentlyDragging = false; };
		void drawDragger() const;
		void drawSelectionDragger() const;

		const bool isDragging() const { return m_isCurrentlyDragging; };
		const Vector3 getStartMousePosition() const { return m_startMousePosition; };
		const Vector3 getStartThingPosition() const { return m_startThingPosition; };
		const float& getParticleMass() const { return m_particleMass; };
		const float& getParticleRadius() const { return m_particleRadius; };

		const Engine::Particles& getSelectedParticles() { return m_selectedParticles; };
		void setSelectedParticles(const Engine::Particles& newSelectedParticles);

		void changeParticleMass(const ofMouseEventArgs& mouse);
		const bool isDraggingBig() const;
};

