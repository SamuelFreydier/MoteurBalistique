#include "Referential.h"


void Referential::drawReferential() const
{
	// On dessine le rérérentiel en blanc et une épaisseur de ligne donnée
	ofSetColor(255, 255, 255);
	ofSetLineWidth(1);

	// dessiner axe des abscisses et des ordonnées (fleches)
	const Vector3 pointDebutAbscisse = Vector3({ 0, m_originPoint.getY(), 0 });
	const Vector3 pointFinAbscisse = Vector3({ (float)ofGetScreenWidth() - 10, m_originPoint.getY(), 0 });
	const Vector3 pointDebutOrdonnees = Vector3({ m_originPoint.getX(), (float)ofGetScreenHeight(), 0 });
	const Vector3 pointFinOrdonnees = Vector3({ m_originPoint.getX(), 10, 0 });


	int taillePointeFleche = 5;
	ofDrawArrow(pointDebutAbscisse.v3(), pointFinAbscisse.v3(), taillePointeFleche);
	ofDrawArrow(pointDebutOrdonnees.v3(), pointFinOrdonnees.v3(), taillePointeFleche);


	// Trouver le bon intervalle de graduation
	const float listMaxLength[4] = { m_originPoint.getX(), ofGetScreenWidth() - m_originPoint.getX(), m_originPoint.getY(), ofGetScreenHeight() - m_originPoint.getY() };
	const float maxLength = *std::max_element(listMaxLength, listMaxLength + 4);
	const int nbrGraduations = maxLength / 50;
	const int intervallesDispos[12] = { 1, 2, 5, 10, 25, 50, 100, 250, 500, 1000, 2500, 5000 };
	int intervalle = 10000;
	for (int i = 0; i < 12; i++)
	{
		if (intervalle == 10000)
		{
			if ((maxLength * m_scale) / intervallesDispos[i] < nbrGraduations)
			{
				intervalle = intervallesDispos[i];
			}
		}
	}


	for (int i = 1; i <= nbrGraduations; i ++)
	{
		float temp;
		const float iLocation = i * intervalle / m_scale;
		const float yAbscisse = m_originPoint.getY();
		const float xOrdonnes = m_originPoint.getX();


		// dessiner graduations sur l'axe des abscisses positives
		temp = m_originPoint.getX() + iLocation;
		ofDrawLine(temp, yAbscisse - 5, temp, yAbscisse + 5);
		ofDrawBitmapString(ofToString(i * intervalle), temp - 10, yAbscisse + 20);


		// dessiner graduations sur l'axe des abscisses négatives
		temp = m_originPoint.getX() - iLocation;
		ofDrawLine(temp, yAbscisse - 5, temp, yAbscisse + 5);
		ofDrawBitmapString(ofToString(- i * intervalle), temp - 10, yAbscisse + 20);
		

		// dessiner graduations sur l'axe des ordonnées positives
		temp = m_originPoint.getY() - iLocation;
		ofDrawLine(xOrdonnes - 5, temp, xOrdonnes + 5, temp);
		ofDrawBitmapString(ofToString(i * intervalle), xOrdonnes - 40, temp + 5);


		// dessiner graduations sur l'axe des ordonnées négatives
		temp = m_originPoint.getY() + iLocation;
		ofDrawLine(xOrdonnes - 5, temp, xOrdonnes + 5, temp);
		ofDrawBitmapString(ofToString(- i * intervalle), xOrdonnes - 40, temp + 5);
	}
	

	// dessiner légendes à l'origine, fleche des abscisses et fleche des ordonnées
	//ofDrawBitmapString(ofToString(0), pointOrigine.v3().x - 20, pointOrigine.v3().y + 20);
	ofDrawBitmapString("  X (en m)", pointFinAbscisse.v3().x - 70, pointFinAbscisse.v3().y + 40);
	ofDrawBitmapString("  Y\n(en m)", pointFinOrdonnees.v3().x - 50, pointFinOrdonnees.v3().y + 10);
}


void Referential::resizeScale(const ofMouseEventArgs& mouse)
{
	const Vector3 mouseXY = Vector3({ mouse.x, mouse.y, 0.0 });
	const Vector3 deltaMouseOrigine = mouseXY - m_originPoint;

	float puissanceScroll = 20;
	if (abs(mouse.scrollY) < puissanceScroll)
	{
		float coef = 1 - mouse.scrollY / puissanceScroll;

		m_scale *= coef;
		m_originPoint += deltaMouseOrigine * (1 - 1 / coef);
	}
}


void Referential::dragOrigin(const std::vector<float>& mouseXY, const Vector3& startDragPosition, const Vector3& startDragOriginPosition)
{
	const Vector3 mouse_2D_Position = Vector3({ mouseXY[0], mouseXY[1], 0.0 });
	const Vector3 deltaDragPosition = mouse_2D_Position - startDragPosition;
		
	m_originPoint = startDragOriginPosition + deltaDragPosition;
}



const Vector3 Referential::conversionPositionMecaniqueGraphique(const Vector3& monVecteur, const bool& trueMecanique_falseGraphique) const
{
	Vector3 vecteurEntree = monVecteur;
	Vector3 vecteurSortie;

	if (trueMecanique_falseGraphique) // transformations successives du référentiel mécanique pour le placer dans le référentiel graphique
	{		
		Vector3 sortieGraphique = Vector3({ monVecteur.getX(), -monVecteur.getY(), monVecteur.getZ()}); // on inverse axe Y
		sortieGraphique *= 1 / m_scale; // on met à l'échelle du référentiel graphique
		sortieGraphique += m_originPoint; // on translate de sorte à se superposer à l'origine graphique du référentiel
		
		vecteurSortie = sortieGraphique;
	}
	else // transformations successives du référentiel graphique pour le placer dans le référentiel mécanique
	{
		Vector3 sortieMecanique = monVecteur - m_originPoint; // on translate de sorte à se superposer à l'origine de l'écran
		sortieMecanique *= m_scale; // on met à l'échelle du référentiel mécanique
		sortieMecanique = Vector3({ sortieMecanique.getX(), -sortieMecanique.getY(), sortieMecanique.getZ()}); // on inverse axe Y

		vecteurSortie = sortieMecanique;
	}

	return vecteurSortie;
}


const Vector3 Referential::conversionVelocityMecaniqueGraphique(const Vector3& monVecteur, const bool& trueMecanique_falseGraphique) const
{
	Vector3 vecteurEntree = monVecteur;
	Vector3 vecteurSortie;

	if (trueMecanique_falseGraphique) // transformations successives du référentiel mécanique pour le placer dans le référentiel graphique
	{
		Vector3 sortieGraphique = Vector3({ monVecteur.getX(), -monVecteur.getY(), monVecteur.getZ()}); // on inverse axe Y
		sortieGraphique *= 1 / m_scale; // on met à l'échelle du référentiel graphique

		vecteurSortie = sortieGraphique;
	}
	else // transformations successives du référentiel graphique pour le placer dans le référentiel mécanique
	{
		Vector3 sortieMecanique = monVecteur * m_scale; // on met à l'échelle du référentiel mécanique
		sortieMecanique = Vector3({ sortieMecanique.getX(), -sortieMecanique.getY(), sortieMecanique.getZ()}); // on inverse axe Y

		vecteurSortie = sortieMecanique;
	}

	return vecteurSortie;
}
