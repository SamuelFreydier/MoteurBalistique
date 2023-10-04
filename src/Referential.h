#pragma once

#include "Vector.h"

class Referential
{
	public:
		Referential() = default;
		void drawReferential();
		void resizeScale(ofMouseEventArgs& mouse);
		void dragOrigin(const std::vector<float>& mouseXY, const Vector& startDragPosition, const Vector& startDragOriginPosition);

		const Vector& getPointOrigine() const { return pointOrigine; };
		float getScale() { return scale; };

		void setPointOrigine(const float& X, const float& Y) { pointOrigine = Vector({ X, Y, 0 }); }

	private:
		float scale = 0.1;
		Vector pointOrigine;

};

