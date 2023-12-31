#pragma once
#include "Vector.h"
#include <vector>
#include "Quaternion.h"

class Matrix3x3
{
	/*
	* ordre des indices:
	* 0 1 2
	* 3 4 5 
	* 6 7 8
	*/
private:
	std::vector<float> matrix;
public:
	const float& get(int i) const { return this->matrix.at(i); }
	const float& get(char indice) const;

	/**
	* Constructeur de Matrix3x3 avec 9 float tels que:
	* Matrix =
	* | a b c |
	* | d e f |
	* | g h i |
	*/
	Matrix3x3(float a=0, float b=0, float c=0, float d=0, float e=0, float f=0, float g=0, float h=0, float i=0);

	/**
	* Constructeur de Matrix3x3 avec un vector V de float tel que:
	* Matrix =
	* | V.at(0) V.at(1) V.at(2) |
	* | V.at(3) V.at(4) V.at(5) |
	* | V.at(6) V.at(7) V.at(8) |
	* Si le vecteur est trop cours, matrix sera complet�e avec des 0
	*/
	Matrix3x3(std::vector<float> vecInit);

	/*
	* Constructeur de matrice de rotation gr�ce � un quaternion selon la formule vue en cours (seance 8)
	*/
	Matrix3x3(Quaternion quaternion_);


	/**
	* | a b c |
	* | d e f |
	* | g h i |
	* devient 
	* | a d g |
	* | b e h |
	* | c f i |
	*/
	void transpose();


	/*
	* return 
	* | a d g |
	* | b e h |
	* | c f i |
	*/
	Matrix3x3 transposed() const;

	float determinant() const;
	bool isInvertible() const { return determinant() != 0.0f; }
	bool isOrthogonal() const;
	bool isIdentity() const { return (get(0) == 1 && get(1) == 0 && get(2) == 0 && get(3) == 0 && get(4) == 1 && get(5) == 0 && get(6) == 0 && get(7) == 0 && get(8) == 1); }
	/*
	* renvoie la matrice inverse. Si elle n'est pas inversible, renvoie la matrice nulle
	*/
	Matrix3x3 inverted() const;


	// Multiplication par un scalaire
	Matrix3x3 operator*(const float& value) const;
	Matrix3x3& operator*=(const float& value);
	//multiplication par une matrice 3x3
	Matrix3x3 operator*(const Matrix3x3& value) const;
	Matrix3x3& operator*=(const Matrix3x3& value);
	// Multiplication par un vecteur
	Vector3 operator*( const Vector3& vector ) const;
	Vector3 transform( const Vector3& vector ) const;
	Vector3 transformTranspose( const Vector3& vector ) const;
	Vector3 leftTransform(const Vector3& vector) const;

	// Modifie la matrice pour qu'elle soit �gale � l'inverse de m
	void setInverse( const Matrix3x3& m );

	// Calcule la matrice antisym�trique d'un vecteur
	void setSkewSymmetric( const Vector3& vector );

	// D�finis la matrice selon les vecteurs colonnes pass�s en param�tre
	void setComponents( const Vector3& compOne, const Vector3& compTwo,
						const Vector3& compThree );

	// division
	Matrix3x3 operator/(const float& value) const;
	Matrix3x3& operator/=(const float& value);

	// operateurs []
	float& operator[](const int& vectorIndex) { return matrix.at(vectorIndex); }
	const float& operator[]( const int& vectorIndex ) const { return matrix.at( vectorIndex ); }

	void operator+=( const Matrix3x3& o );

	/*
	* Calcul de le d�terminant de la sous matrice
	* | a b |
	* | c d |
	* pour le calcul du cofacteur utilis� dans la d�termination d'une matrice inverse
	*/
	float determinantSousMatrice(const float& a_,const float& b_,const float& c_,const float& d_);
};

