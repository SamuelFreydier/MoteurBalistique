#include "MouseDragger.h"


MouseDragger::MouseDragger()
{

}



MouseDragger::MouseDragger(const Vector3& newStartMousePosition, const Vector3& newStartThingPosition)
	: m_isCurrentlyDragging(true), m_startMousePosition(newStartMousePosition), m_startThingPosition(newStartThingPosition)
{

}

MouseDragger::MouseDragger(const Vector3& newStartMousePosition, const float& newParticleRadius)
	: m_isCurrentlyDragging(true), m_startMousePosition(newStartMousePosition), m_particleRadius(newParticleRadius)
{

}

/**
 * @brief Affiche la flèche (ou le rond) permettant de tirer une particule
*/
void MouseDragger::drawDragger() const
{
	Vector3 currentMousePosition = Vector3({ (float)ofGetMouseX(), (float)ofGetMouseY(), 0.0 });
	float normeDeltaDragPosition = (currentMousePosition - m_startMousePosition).norm();

	float coefSize = normeDeltaDragPosition / 15;
	float arrowSize = 1.5 * coefSize;
	float lineSize = 0.5 * coefSize;

	float adjustedValue = sqrtf(sqrtf(1 / m_particleMass));
	if (adjustedValue > 1)
	{
		adjustedValue = 1;
	}

	ofSetColor(255, adjustedValue * 255, (1 - adjustedValue) * 255);
	ofSetLineWidth(lineSize);

	float referentialScale = Engine::getInstance()->getReferential().getScale();

	if (normeDeltaDragPosition < (m_particleRadius / referentialScale)) // si le launcher trop petit, alors on affiche un rond
	{
		ofDrawCircle(m_startMousePosition.v3(), m_particleRadius / referentialScale);
	}
	else // sinon on affiche une flèche
	{
		ofDrawArrow(currentMousePosition.v3(), m_startMousePosition.v3(), arrowSize);
	}

	ofSetColor(255, (1 - adjustedValue) * 255, adjustedValue * 255);
	ofDrawBitmapString(ofToString(normeDeltaDragPosition * referentialScale) + " m/s\n" + ofToString(m_particleMass) + " kg", (currentMousePosition + Vector3({ 20.0, 20.0, 0.0 })).v3());
}


/**
 * @brief Affiche le rectangle de sélection de particules
*/
void MouseDragger::drawSelectionDragger() const
{
	const Vector3 coinHautGauche = Vector3(std::min(m_startMousePosition.x, (float)ofGetMouseX()), std::min(m_startMousePosition.y, (float)ofGetMouseY()));
	const float w = std::abs(ofGetMouseX() - m_startMousePosition.x);
	const float h = std::abs(ofGetMouseY() - m_startMousePosition.y);


	ofSetColor(100, 100, 100);
	ofDrawRectangle(coinHautGauche.v2(), w, h);
}


/**
 * @brief Met le statut "sélectionné" de toutes les particules sélectionnées dans le rectangle à jour
 * @param newSelectedParticles 
*/
void MouseDragger::setSelectedParticles(const Engine::Particles& newSelectedParticles)
{ 
	m_selectedParticles = newSelectedParticles; 

	for (std::shared_ptr<Particle> selectedParticle : m_selectedParticles)
	{
		selectedParticle->isSelected = true;
	}
};

/**
 * @brief Met à jour la masse de la particule avec la molette
 * @param mouse 
*/
void MouseDragger::changeParticleMass(const ofMouseEventArgs& mouse)
{
	float puissanceScroll = 10;
	if (abs(mouse.scrollY) < puissanceScroll)
	{
		float coef = 1 + mouse.scrollY / puissanceScroll;
		m_particleMass *= coef;
	}
}


/**
 * @brief Permet de savoir si l'étirement de la flèche a été assez grand pour appliquer une impulsion
 * @return 
*/
const bool MouseDragger::isDraggingBig() const
{
	Vector3 currentMousePosition = Vector3({ (float)ofGetMouseX(), (float)ofGetMouseY(), 0.0 });
	float normeDeltaDragPosition = (currentMousePosition - m_startMousePosition).norm();
	float referentialScale = Engine::getInstance()->getReferential().getScale();

	return normeDeltaDragPosition >= (m_particleRadius / referentialScale);
}