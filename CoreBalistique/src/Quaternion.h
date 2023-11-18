#ifndef QUATERNION_H
#define QUATERNION_H

#include "Vector.h"

class Quaternion
{
public:
    // Partie r�elle du quaternion
    float r;

    // Premier nombre imaginaire du quaternion
    float i;

    // Deuxi�me nombre imaginaire du quaternion
    float j;

    // Troisi�me nombre imaginaire du quaternion
    float k;

public:
    // Constructeur cr�ant un quaternion de rotation 0 par d�faut
    Quaternion( const float& newR = 1, const float& newI = 0, const float& newJ = 0, const float& newK = 0 );

    // Normalise le quaternion pour que sa norme soit de 1 et qu'il devienne un quaternion de rotation valide
    void normalize();
    
    // Norme du quaternion
    float getNorm() const;

    // Conjugu� du quaternion
    Quaternion getConjugate() const;

    // Inverse du quaternion
    Quaternion getInverse() const;

    // Exponentation : fraction de la rotation du quaternion
    Quaternion getExponentiation( const float& fraction );

    // Interpolation lin�aire sph�rique (Spherical linear interpolation) entre le quaternion courant et un quaternion de destination en utilisant l'exponentiation
    Quaternion slerp( const Quaternion& destination, const float& fraction );

    // Op�rations entre quaternions
    Quaternion operator*( const Quaternion& other ) const;
    Quaternion& operator*=( const Quaternion& other );
    Quaternion operator-() const;
    float dotProduct( const Quaternion& other ) const;

    // Op�rations avec des scalaires
    Quaternion operator*( const float& scalar ) const;
    Quaternion& operator*=( const float& scalar );
    Quaternion operator/( const float& scalar ) const;
    Quaternion& operator/=( const float& scalar );

    // Met � jour la rotation en fonction d'une v�locit� angulaire et du temps. g'(t) = g0 + 1/2 * w * g(t) * deltaTime
    void applyAngularVelocity( const Vector3& angularVelocity, float deltaTime );

    // Getters (pour s'assurer de rester en lecture seule dans certains cas)
    const float& getR() const { return r; }
    const float& getI() const { return i; }
    const float& getJ() const { return j; }
    const float& getK() const { return k; }

    // Quaternion graphique
    glm::quat quat() const { return glm::quat(r, i, j, k); }
};

#endif