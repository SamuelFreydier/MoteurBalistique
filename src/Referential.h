#pragma once

#include "Vector.h"

class Referential
{
	public:
		Referential() = default;
		void drawReferential() const;
		void resizeScale(const ofMouseEventArgs& mouse);
		void dragOrigin(const std::vector<float>& mouseXY, const Vector& startDragPosition, const Vector& startDragOriginPosition);

		const Vector& getPointOrigine() const { return pointOrigine; };
		const float getScale() const { return scale; };

		void setPointOrigine(const float& X, const float& Y) { pointOrigine = Vector({ X, Y, 0 }); }

		const Vector conversionPositionMecaniqueGraphique(const Vector& monVecteur, const bool& trueMecanique_falseGraphique) const;
		const Vector conversionVelocityMecaniqueGraphique(const Vector& monVecteur, const bool& trueMecanique_falseGraphique) const;

	private:
		float scale = 0.1;
		Vector pointOrigine;

};

