#pragma once
#include"pch.h"
#include "Matrix4x4.h"


namespace Matrix4x4Tests {
	Matrix4x4 matrice4Nulle(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	Matrix4x4 matrice4Ordonnee(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
	Matrix4x4 matrice4Identity(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	Matrix4x4 matrice4Arbitraire(5, 2.4f, 0, 10, 4, 7.2f, 0, 0, -2.1f, 4, 2, 4, 3, 42, 7.1f, 0.29f);
	/*
	* | 5   2.4 0   10   |
	* | 4   7.2 0   0    |
	* |-2.1 4   2   4    |
	* | 3   42  7.1 0.29 |
	*/
	TEST(Matrix4x4Tests, determinants4x4) {
		EXPECT_EQ(matrice4Identity.determinant(), 1);
		EXPECT_EQ(matrice4Nulle.determinant(), 0);
		EXPECT_FLOAT_EQ(matrice4Identity.determinantSousMatrice(Matrix3x3(5, 2, 0, 3.3, 4, 0, 0, 2, 1)), 13.4f);
	}

	TEST(Matrix4x4Tests, inversibility4x4) {
		EXPECT_TRUE(matrice4Identity.isInvertible());
		EXPECT_TRUE(matrice4Arbitraire.isInvertible());
		EXPECT_FALSE(matrice4Ordonnee.isInvertible());
		for (int i = 0; i < 16; i++)
			EXPECT_EQ(matrice4Ordonnee.inverted().get(i), 0.0f);

		EXPECT_EQ(matrice4Identity.inverted().get(0), 1.0f);
		EXPECT_EQ(matrice4Identity.inverted().get(7), 0.0f);
		Matrix4x4 matriceArbitrairetoIdentity = matrice4Arbitraire.inverted() * matrice4Arbitraire;
		float epsilon = 0.00001f;
		bool isEqual = (matriceArbitrairetoIdentity.get(0) < 1.0f + epsilon && matriceArbitrairetoIdentity.get(0) > 1.0f - epsilon);
		//EXPECT_FLOAT_EQ(matriceArbitrairetoIdentity.get(0), 1.0f);
		EXPECT_TRUE(isEqual);
		isEqual = (matriceArbitrairetoIdentity.get(15) < 1.0f + epsilon && matriceArbitrairetoIdentity.get(15) > 1.0f - epsilon);
		//EXPECT_FLOAT_EQ(matriceArbitrairetoIdentity.get(15), 1.0f);
		EXPECT_TRUE(isEqual);
		isEqual = (matriceArbitrairetoIdentity.get(9) < epsilon && matriceArbitrairetoIdentity.get(9) > -epsilon);
		//EXPECT_FLOAT_EQ(matriceArbitrairetoIdentity.get(9), 0.0f);
		EXPECT_TRUE(isEqual);
		EXPECT_FLOAT_EQ((matrice4Identity.inverted() * matrice4Identity).get(0), 1.0f);
	}

	TEST(Matrix4x4Tests, multiplications)
	{
		EXPECT_EQ((matrice4Arbitraire * matrice4Identity).get('a'), matrice4Arbitraire.get('a'));
		EXPECT_TRUE(matrice4Identity.isIdentity());
		Matrix4x4 matrice4quadArbitraire(matrice4Arbitraire * 4);
		EXPECT_EQ(matrice4quadArbitraire.get(7), matrice4Arbitraire.get(7) * 4);
		EXPECT_EQ(matrice4quadArbitraire.get(15), matrice4Arbitraire.get(15) * 4);
		matrice4quadArbitraire /= 2;
		EXPECT_EQ(matrice4quadArbitraire.get(0), matrice4Arbitraire.get(0) * 2);
	}


	TEST(Matrix4x4Tests, init4x4)
	{
		std::vector<float> vectest{ 0.2f, 1.3f, 0.0f, 5.0f, 7.2f, -8.5f };
		Matrix4x4 matriceVec(vectest);
		EXPECT_EQ(matriceVec.get(0), 0.2f);
		EXPECT_EQ(matriceVec.get(5), -8.5f);
		EXPECT_EQ(matriceVec.get(8), 0);
		EXPECT_EQ(matriceVec.get('p'), 0);
	}

	TEST(Matrix4x4Tests, transpose4x4)
	{
		Matrix4x4 transposedMat(matrice4Arbitraire.transposed());
		EXPECT_EQ(transposedMat.get(0), 5);
		EXPECT_EQ(transposedMat.get(6), matrice4Arbitraire.get(9));
		Matrix4x4 trmat(matrice4Arbitraire);
		trmat.transpose();
		for (int i = 0; i < 16;i++)
			EXPECT_EQ(transposedMat.get(i), trmat.get(i));
		EXPECT_TRUE(matrice4Identity.isOrthogonal());
		EXPECT_FALSE(Matrix4x4(1.5, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1).isOrthogonal());
	}
}
