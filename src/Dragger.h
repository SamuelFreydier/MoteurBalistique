#pragma once

#include "Vector.h"

class Dragger
{
	private:
		bool isCurrentlyDragging;
		Vector startMousePosition;
		Vector startThingPosition;

	public:
		Dragger(const Vector& newStartMousePosition, const Vector& newStartThingPosition);
		void draggingIsOver() { isCurrentlyDragging = false; };

		const bool isDragging() const { return isCurrentlyDragging; };
		const Vector getStartMousePosition() const { return startMousePosition; };
		const Vector getStartThingPosition() const { return startThingPosition; };
};

