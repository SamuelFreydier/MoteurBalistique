#pragma once

#include "Vector.h"

class Referential
{
	public:
		Referential() = default;
		void drawReferential() const;
		void resizeScale(const ofMouseEventArgs& mouse);
		void dragOrigin(const std::vector<float>& mouseXY, const Vector3& startDragPosition, const Vector3& startDragOriginPosition);

		const Vector3& getPointOrigine() const { return pointOrigine; };
		const float getScale() const { return scale; };

		void setPointOrigine(const float& X, const float& Y) { pointOrigine = Vector3({ X, Y, 0 }); }

		const Vector3 conversionPositionMecaniqueGraphique(const Vector3& monVecteur, const bool& trueMecanique_falseGraphique) const;
		const Vector3 conversionVelocityMecaniqueGraphique(const Vector3& monVecteur, const bool& trueMecanique_falseGraphique) const;

	private:
		float scale = 0.1;
		Vector3 pointOrigine;

};

