#pragma once
#include <vector>
#include"pch.h"
#include"Matrix3x3.h"

namespace Matrix3x3Tests {
	Matrix3x3 matriceNulle(0, 0, 0, 0, 0, 0, 0, 0, 0);
	Matrix3x3 matriceIdentity(1, 0, 0, 0, 1, 0, 0, 0, 1);
	Matrix3x3 matriceArbitraire(5, 2, 0, 3.3, 4, 0, 0, 2, 1);


	TEST(TestMatrix3x3, determinants)
	{
		EXPECT_EQ(matriceArbitraire.determinantSousMatrice(0, 0, 0, 0), 0);
		EXPECT_EQ(matriceArbitraire.determinantSousMatrice(0, 1, 1, 0), -1);
		EXPECT_EQ(matriceArbitraire.determinantSousMatrice(1, 0, 0, 1), 1);
		EXPECT_EQ(matriceArbitraire.determinantSousMatrice(1, 1, 1, 1), 0);
		EXPECT_EQ(matriceArbitraire.determinantSousMatrice(4, 7, 2, 8), 18);
		EXPECT_EQ(matriceIdentity.determinant(), 1);
		EXPECT_EQ(matriceArbitraire.determinant(), 13.4f);
	}
	TEST(TestMatrix3x3, inversibilityTests)
	{
		EXPECT_TRUE(matriceArbitraire.isInvertible());
		EXPECT_TRUE(matriceIdentity.isInvertible());
		Matrix3x3 matricenoninversible(1, 2, 3, 2, 4, 6, 3, 5, 7);
		EXPECT_FALSE(matricenoninversible.isInvertible());
		for (int i = 0; i < 9; i++)
			EXPECT_EQ(matricenoninversible.inverted().get(i), 0.0f);
		EXPECT_EQ(matriceIdentity.inverted().get(0), 1.0f);
		EXPECT_EQ(matriceIdentity.inverted().get(7), 0.0f);
		EXPECT_EQ(matriceArbitraire.inverted().get(2), 0);
		EXPECT_EQ((matriceArbitraire.inverted() * matriceArbitraire).get(0), 1.0f);
		EXPECT_EQ((matriceArbitraire * matriceArbitraire.inverted()).get(7), 0.0f);
	}
	TEST(TestMatrix3x3, multiplications)
	{
		
		EXPECT_EQ((matriceArbitraire * matriceIdentity).get(3), matriceArbitraire.get(3));
		EXPECT_TRUE(matriceIdentity.isIdentity());
		EXPECT_FALSE(matriceArbitraire.isIdentity());
		EXPECT_FALSE(matriceNulle.isIdentity());
		Matrix3x3 tripleIdentity(matriceIdentity * 3.0f);
		EXPECT_EQ(tripleIdentity.get('a'), 3);
		EXPECT_EQ(tripleIdentity.get(2), 0);
		tripleIdentity *= 2;
		EXPECT_EQ(tripleIdentity.get(4), 6);
		Matrix3x3 quadArbitraire(matriceArbitraire * 4);
		EXPECT_EQ(quadArbitraire.get(0), 20);
		EXPECT_EQ(quadArbitraire.get(3), 13.2f);
		quadArbitraire /= 2;
		EXPECT_EQ(quadArbitraire.get(1), 4);
		EXPECT_EQ(quadArbitraire.get(2), 0);
		EXPECT_EQ(quadArbitraire.get(8), matriceArbitraire.get(8) * 2);
	}

	TEST(TestMatrix3x3, initialise)
	{
		std::vector<float> vectest{0.2f,1.3f,0.0f,5.0f};
		Matrix3x3 matriceVec(vectest);
		EXPECT_EQ(matriceVec.get(0), 0.2f);
		EXPECT_EQ(matriceVec.get(5), 0);
		EXPECT_EQ(matriceVec.get(8), 0);
		EXPECT_EQ(matriceArbitraire.get('a'), 5);
		EXPECT_EQ(matriceArbitraire.get(2), 0);
	}

	TEST(TestMatrix3x3, transpose)
	{
		Matrix3x3 matriceTR(1, 2, 3, 0, 0, 0, 0, 0, 0);
		Matrix3x3 transposedMatrix(matriceTR.transposed());
		EXPECT_EQ(transposedMatrix.get(0), matriceTR.get(0));
		EXPECT_EQ(transposedMatrix.get(1), 0);
		EXPECT_EQ(transposedMatrix.get(3), 2);
		matriceTR.transpose();
		for (int i = 0; i < 9;i++)
			EXPECT_EQ(transposedMatrix.get(i), matriceTR.get(i));
		EXPECT_TRUE(matriceIdentity.isOrthogonal());
		EXPECT_TRUE(Matrix3x3(1, 0, 0, 0, 0, -1, 0, 1, 0).isOrthogonal());
		EXPECT_FALSE(Matrix3x3(1.5, 0, 0, 0, 0, -1, 0, 1, 0).isOrthogonal());
	}



}