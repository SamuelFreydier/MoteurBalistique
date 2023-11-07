#ifndef QUATERNION_H
#define QUATERNION_H

#include "Vector.h"

class Quaternion
{
public:
    // Partie réelle du quaternion
    float r;

    // Premier nombre imaginaire du quaternion
    float i;

    // Deuxième nombre imaginaire du quaternion
    float j;

    // Troisième nombre imaginaire du quaternion
    float k;

public:
    // Constructeur créant un quaternion de rotation 0 par défaut
    Quaternion( const float& newR = 1, const float& newI = 0, const float& newJ = 0, const float& newK = 0 );

    // Normalise le quaternion pour que sa norme soit de 1 et qu'il devienne un quaternion de rotation valide
    void normalize();

    // Multiplications entre quaternions
    Quaternion operator*( const Quaternion& other );
    Quaternion& operator*=( const Quaternion& other );

    // Tourne le quaternion d'un certain montant, indiqué par un vecteur
    void rotateByVector( const Vector3& vector );

    // Met à jour la rotation avec un vecteur et une échelle. Permet de mettre à jour le quaternion avec une nouvelle rotation et le temps
    void addScaledVector( const Vector3& vector, float scale );

    // Getters (pour s'assurer de rester en lecture seule dans certains cas)
    const float& getR() const { return r; }
    const float& getI() const { return i; }
    const float& getJ() const { return j; }
    const float& getK() const { return k; }
};

#endif