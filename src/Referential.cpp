#include "Referential.h"


void Referential::drawReferential() const
{
	// On dessine le r�r�rentiel en blanc et une �paisseur de ligne donn�e
	ofSetColor(255, 255, 255);
	ofSetLineWidth(1);

	// dessiner axe des abscisses et des ordonn�es (fleches)
	const Vector pointDebutAbscisse = Vector({ 0, pointOrigine.getY(), 0 });
	const Vector pointFinAbscisse = Vector({ (float)ofGetScreenWidth() - 10, pointOrigine.getY(), 0 });
	const Vector pointDebutOrdonnees = Vector({ pointOrigine.getX(), (float)ofGetScreenHeight(), 0 });
	const Vector pointFinOrdonnees = Vector({ pointOrigine.getX(), 10, 0 });


	int taillePointeFleche = 5;
	ofDrawArrow(pointDebutAbscisse.v3(), pointFinAbscisse.v3(), taillePointeFleche);
	ofDrawArrow(pointDebutOrdonnees.v3(), pointFinOrdonnees.v3(), taillePointeFleche);


	// Trouver le bon intervalle de graduation
	const float listMaxLength[4] = { pointOrigine.getX(), ofGetScreenWidth() - pointOrigine.getX(), pointOrigine.getY(), ofGetScreenHeight() - pointOrigine.getY() };
	const float maxLength = *std::max_element(listMaxLength, listMaxLength + 4);
	const int nbrGraduations = maxLength / 50;
	const int intervallesDispos[12] = { 1, 2, 5, 10, 25, 50, 100, 250, 500, 1000, 2500, 5000 };
	int intervalle = 10000;
	for (int i = 0; i < 12; i++)
	{
		if (intervalle == 10000)
		{
			if ((maxLength * scale) / intervallesDispos[i] < nbrGraduations)
			{
				intervalle = intervallesDispos[i];
			}
		}
	}


	for (int i = 1; i <= nbrGraduations; i ++)
	{
		float temp;
		const float iLocation = i * intervalle / scale;
		const float yAbscisse = pointOrigine.getY();
		const float xOrdonnes = pointOrigine.getX();


		// dessiner graduations sur l'axe des abscisses positives
		temp = pointOrigine.getX() + iLocation;
		ofDrawLine(temp, yAbscisse - 5, temp, yAbscisse + 5);
		ofDrawBitmapString(ofToString(i * intervalle), temp - 10, yAbscisse + 20);


		// dessiner graduations sur l'axe des abscisses n�gatives
		temp = pointOrigine.getX() - iLocation;
		ofDrawLine(temp, yAbscisse - 5, temp, yAbscisse + 5);
		ofDrawBitmapString(ofToString(- i * intervalle), temp - 10, yAbscisse + 20);
		

		// dessiner graduations sur l'axe des ordonn�es positives
		temp = pointOrigine.getY() - iLocation;
		ofDrawLine(xOrdonnes - 5, temp, xOrdonnes + 5, temp);
		ofDrawBitmapString(ofToString(i * intervalle), xOrdonnes - 40, temp + 5);


		// dessiner graduations sur l'axe des ordonn�es n�gatives
		temp = pointOrigine.getY() + iLocation;
		ofDrawLine(xOrdonnes - 5, temp, xOrdonnes + 5, temp);
		ofDrawBitmapString(ofToString(- i * intervalle), xOrdonnes - 40, temp + 5);
	}
	

	// dessiner l�gendes � l'origine, fleche des abscisses et fleche des ordonn�es
	//ofDrawBitmapString(ofToString(0), pointOrigine.v3().x - 20, pointOrigine.v3().y + 20);
	ofDrawBitmapString("  X (en m)", pointFinAbscisse.v3().x - 70, pointFinAbscisse.v3().y + 40);
	ofDrawBitmapString("  Y\n(en m)", pointFinOrdonnees.v3().x - 50, pointFinOrdonnees.v3().y + 10);
}


void Referential::resizeScale(const ofMouseEventArgs& mouse)
{
	const Vector mouseXY = Vector({ mouse.x, mouse.y, 0.0 });
	const Vector deltaMouseOrigine = mouseXY - pointOrigine;

	float puissanceScroll = 20;
	if (abs(mouse.scrollY) < puissanceScroll)
	{
		float coef = 1 - mouse.scrollY / puissanceScroll;

		scale *= coef;
		pointOrigine += deltaMouseOrigine * (1 - 1 / coef);
	}
}


void Referential::dragOrigin(const std::vector<float>& mouseXY, const Vector& startDragPosition, const Vector& startDragOriginPosition)
{
	const Vector mouse_2D_Position = Vector(mouseXY);
	const Vector deltaDragPosition = mouse_2D_Position - startDragPosition;
		
	pointOrigine = startDragOriginPosition + deltaDragPosition;
}



const Vector Referential::conversionPositionMecaniqueGraphique(const Vector& monVecteur, const bool& trueMecanique_falseGraphique) const
{
	Vector vecteurEntree = monVecteur;
	Vector vecteurSortie;

	if (trueMecanique_falseGraphique) // transformations successives du r�f�rentiel m�canique pour le placer dans le r�f�rentiel graphique
	{		
		Vector sortieGraphique = Vector({ monVecteur.getX(), -monVecteur.getY(), 0.0 }); // on inverse axe Y
		sortieGraphique *= 1 / scale; // on met � l'�chelle du r�f�rentiel graphique
		sortieGraphique += pointOrigine; // on translate de sorte � se superposer � l'origine graphique du r�f�rentiel
		
		vecteurSortie = sortieGraphique;
	}
	else // transformations successives du r�f�rentiel graphique pour le placer dans le r�f�rentiel m�canique
	{
		Vector sortieMecanique = monVecteur - pointOrigine; // on translate de sorte � se superposer � l'origine de l'�cran
		sortieMecanique *= scale; // on met � l'�chelle du r�f�rentiel m�canique
		sortieMecanique = Vector({ sortieMecanique.getX(), -sortieMecanique.getY(), 0.0 }); // on inverse axe Y

		vecteurSortie = sortieMecanique;
	}

	return vecteurSortie;
}


const Vector Referential::conversionVelocityMecaniqueGraphique(const Vector& monVecteur, const bool& trueMecanique_falseGraphique) const
{
	Vector vecteurEntree = monVecteur;
	Vector vecteurSortie;

	if (trueMecanique_falseGraphique) // transformations successives du r�f�rentiel m�canique pour le placer dans le r�f�rentiel graphique
	{
		Vector sortieGraphique = Vector({ monVecteur.getX(), -monVecteur.getY(), 0.0 }); // on inverse axe Y
		sortieGraphique *= 1 / scale; // on met � l'�chelle du r�f�rentiel graphique

		vecteurSortie = sortieGraphique;
	}
	else // transformations successives du r�f�rentiel graphique pour le placer dans le r�f�rentiel m�canique
	{
		Vector sortieMecanique = monVecteur * scale; // on met � l'�chelle du r�f�rentiel m�canique
		sortieMecanique = Vector({ sortieMecanique.getX(), -sortieMecanique.getY(), 0.0 }); // on inverse axe Y

		vecteurSortie = sortieMecanique;
	}

	return vecteurSortie;
}
