#include "MouseDragger.h"


MouseDragger::MouseDragger()
{

}



MouseDragger::MouseDragger(const Vector3& newStartMousePosition, const Vector3& newStartThingPosition)
	: isCurrentlyDragging(true), startMousePosition(newStartMousePosition), startThingPosition(newStartThingPosition)
{

}

MouseDragger::MouseDragger(const Vector3& newStartMousePosition, const float& newParticleRadius)
	: isCurrentlyDragging(true), startMousePosition(newStartMousePosition), particleRadius(newParticleRadius)
{

}


void MouseDragger::drawDragger() const
{
	std::cout << isDraggingBig() << "\n";
	Vector3 currentMousePosition = Vector3({ (float)ofGetMouseX(), (float)ofGetMouseY(), 0.0 });
	float normeDeltaDragPosition = (currentMousePosition - startMousePosition).norm();

	float coefSize = normeDeltaDragPosition / 15;
	float arrowSize = 1.5 * coefSize;
	float lineSize = 0.5 * coefSize;

	float adjustedValue = sqrtf(sqrtf(1 / particleMass));
	if (adjustedValue > 1)
	{
		adjustedValue = 1;
	}

	ofSetColor(255, adjustedValue * 255, (1 - adjustedValue) * 255);
	ofSetLineWidth(lineSize);

	float referentialScale = Engine::getInstance()->getReferential().getScale();

	if (normeDeltaDragPosition < (particleRadius / referentialScale)) // si le launcher trop petit, alors on affiche un rond
	{
		ofDrawCircle(startMousePosition.v3(), particleRadius / referentialScale);
	}
	else // sinon on affiche une flèche
	{
		ofDrawArrow(currentMousePosition.v3(), startMousePosition.v3(), arrowSize);
	}

	ofSetColor(255, (1 - adjustedValue) * 255, adjustedValue * 255);
	ofDrawBitmapString(ofToString(normeDeltaDragPosition * referentialScale) + " m/s\n" + ofToString(particleMass) + " kg", (currentMousePosition + Vector3({ 20.0, 20.0, 0.0 })).v3());
}


void MouseDragger::changeParticleMass(const ofMouseEventArgs& mouse)
{
	float puissanceScroll = 10;
	if (abs(mouse.scrollY) < puissanceScroll)
	{
		float coef = 1 + mouse.scrollY / puissanceScroll;
		particleMass *= coef;
	}
}


const bool MouseDragger::isDraggingBig() const
{
	Vector3 currentMousePosition = Vector3({ (float)ofGetMouseX(), (float)ofGetMouseY(), 0.0 });
	float normeDeltaDragPosition = (currentMousePosition - startMousePosition).norm();
	float referentialScale = Engine::getInstance()->getReferential().getScale();

	return normeDeltaDragPosition >= (particleRadius / referentialScale);
}