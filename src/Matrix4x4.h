#pragma once
#include "Vector.h"
#include<vector>
#include "Matrix3x3.h"
class Matrix4x4
{


	
		/*
		* ordre des indices:
		* 0 1 2 3
		* 4 5 6 7
		* 8 9 10 11
		* 12 13 14 15
		*/
	private:
		std::vector<float> matrix;
	public:
		const float& get(int i) const { return this->matrix.at(i); }
		const float& get(char indice) const;

		/**
		* Constructeur de Matrix4x4 avec 16 float tels que:
		* Matrix =
		* | a b c d |
		* | e f g h |
		* | i j k l |
		* | m n o p |
		*/
		Matrix4x4(float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l, float m, float n, float o, float p);

		/**
		* Constructeur de Matrix4x4 avec un vector V de float tel que:
		* Matrix =
		* | V.at(0) V.at(1) V.at(2) V.at(3) |
		* | V.at(4) V.at(5) V.at(6) V.at(7) |
		* | V.at(8) V.at(9) V.at(10) V.at(11) |
		* | V.at(12) V.at(13) V.at(14) V.at(15) [
		* Si le vecteur est trop cours, matrix sera completée avec des 0
		*/
		Matrix4x4(std::vector<float> vecInit);


		/**
		* | a b c d |
		* | e f g h |
		* | i j k l |
		* | m n o p |
		* devient
		* | a e i m |
		* | b f j n |
		* | c g k o |
		* | d h l p |
		*/
		void transpose();


		/*
		* return
		* | a e i m |
		* | b f j n |
		* | c g k o |
		* | d h l p |
		*/
		Matrix4x4 transposed() const;

		Matrix4x4 operator*(const vector<float>& value) const;

		float determinant() const;
		bool isInvertible() const { return determinant() == 0.0f; }
		bool isOrthogonal() const;
		bool isIdentity() const { return (get(0) == 1 && get(1) == 0 && get(2) == 0 && get(3) == 0 && get(4) == 0 && get(5) == 1 && get(6) == 0 && get(7) == 0 && get(8) == 0 && get(9) == 0 && get(10) == 1 && get(11) == 0 && get(12) == 0 && get(13) == 0 &&get(14) == 0 && get(15)==1); }
		/*
		* renvoie la matrice inverse. Si elle n'est pas inversible, renvoie la matrice nulle
		*/
		Matrix4x4 inverted() const;


		// Multiplication par un scalaire
		Matrix4x4 operator*(const float& value) const;
		Matrix4x4& operator*=(const float& value);
		//multiplication par une matrice 4x4
		Matrix4x4 operator*(const Matrix4x4& value) const;
		Matrix4x4& operator*=(const Matrix4x4& value);
		// division
		Matrix4x4 operator/(const float& value) const;
		Matrix4x4& operator/=(const float& value);



		/*
		* Calcul de le déterminant de la sous matrice Matrix3x3
		* pour le calcul du cofacteur utilisé dans la détermination d'une matrice inverse
		*/
		float determinantSousMatrice(const Matrix3x3 value);
};




