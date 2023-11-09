#include "pch.h"
#include "Quaternion.h"

namespace QuaternionTest
{
    /* CONSTRUCTEUR */
    TEST( Constructor, DefaultInitialization )
    {
        Quaternion q1;

        EXPECT_EQ( q1.r, 1 );
        EXPECT_EQ( q1.i, 0 );
        EXPECT_EQ( q1.j, 0 );
        EXPECT_EQ( q1.k, 0 );
    }

    TEST( Constructor, ValueInitialization )
    {
        Quaternion q1( 5.2f, 6.3f, 7.8f, 9.6f );

        EXPECT_EQ( q1.r, 5.2f );
        EXPECT_EQ( q1.i, 6.3f );
        EXPECT_EQ( q1.j, 7.8f );
        EXPECT_EQ( q1.k, 9.6f );

        Quaternion q2( 6.5f, 8.3f );

        EXPECT_EQ( q2.r, 6.5f );
        EXPECT_EQ( q2.i, 8.3f );
        EXPECT_EQ( q2.j, 0.f );
        EXPECT_EQ( q2.k, 0.f );
    }

    TEST( Constructor, CopyInitialization )
    {
        Quaternion q1( 5.2f, 6.3f, 7.8f, 9.6f );
        Quaternion q2( q1 );

        EXPECT_EQ( q2.r, 5.2f );
        EXPECT_EQ( q2.i, 6.3f );
        EXPECT_EQ( q2.j, 7.8f );
        EXPECT_EQ( q2.k, 9.6f );
    }

    /* NORME */
    TEST( Norm, NormCalculation )
    {
        Quaternion q1;
        EXPECT_EQ( q1.getNorm(), 1.f );

        Quaternion q2( 0.f, 0.f, 0.f, 0.f );
        EXPECT_EQ( q2.getNorm(), 0.f );

        Quaternion q3( 5.2f, 6.3f, 7.8f, 9.6f );
        EXPECT_EQ( q3.getNorm(), sqrtf( 219.73f ) );
    }

    TEST( Norm, Normalization )
    {
        Quaternion q1;
        q1.normalize(); // Ne dois rien changer
        EXPECT_EQ( q1.r, 1 );
        EXPECT_EQ( q1.i, 0 );
        EXPECT_EQ( q1.j, 0 );
        EXPECT_EQ( q1.k, 0 );

        Quaternion q2( 0.f, 0.f, 0.f, 0.f );
        q2.normalize(); // Passe la partie réelle à 1
        EXPECT_EQ( q2.r, 1 );
        EXPECT_EQ( q2.i, 0 );
        EXPECT_EQ( q2.j, 0 );
        EXPECT_EQ( q2.k, 0 );

        Quaternion q3( 5.2f, 6.3f, 7.8f, 9.6f );
        float normBefore = q3.getNorm();
        q3.normalize();
        EXPECT_EQ( q3.getNorm(), 1.f );
        EXPECT_EQ( q3.r, 5.2f / normBefore );
        EXPECT_EQ( q3.i, 6.3f / normBefore );
        EXPECT_EQ( q3.j, 7.8f / normBefore );
        EXPECT_EQ( q3.k, 9.6f / normBefore );
    }

    /* OPERATIONS */
    TEST( Operations, ScalarOperations )
    {
        Quaternion q1( 5.2f, 6.3f, 7.8f, 9.6f );
        float divider = 2.3f;
        q1 /= divider;
        EXPECT_EQ( q1.r, 5.2f / divider );
        EXPECT_EQ( q1.i, 6.3f / divider );
        EXPECT_EQ( q1.j, 7.8f / divider );
        EXPECT_EQ( q1.k, 9.6f / divider );

        q1 *= divider;
        EXPECT_EQ( q1.r, 5.2f );
        EXPECT_EQ( q1.i, 6.3f );
        EXPECT_EQ( q1.j, 7.8f );
        EXPECT_EQ( q1.k, 9.6f );

        Quaternion q2( q1 );
        q2 *= 1.f;
        EXPECT_EQ( q1.r, q2.r );
        EXPECT_EQ( q1.i, q2.i );
        EXPECT_EQ( q1.j, q2.j );
        EXPECT_EQ( q1.k, q2.k );
    }

    TEST( Operations, QuaternionOperations )
    {
        Quaternion q1( 5.2f, 6.3f, 7.8f, 9.6f );
        Quaternion q2 = -q1;

        EXPECT_EQ( q1.r, 5.2f );
        EXPECT_EQ( q1.i, 6.3f );
        EXPECT_EQ( q1.j, 7.8f );
        EXPECT_EQ( q1.k, 9.6f );

        EXPECT_EQ( q1.r, -q2.r );
        EXPECT_EQ( q1.i, -q2.i );
        EXPECT_EQ( q1.j, -q2.j );
        EXPECT_EQ( q1.k, -q2.k );

        Quaternion q3( 8.2f, 6.4f, 7.6f, 6.8f );

        float dotProduct = q1.dotProduct( q3 );
        EXPECT_FLOAT_EQ( dotProduct, 207.52f );

        Vector3 imaginaryPartQ1( q1.i, q1.j, q1.k );
        Vector3 imaginaryPartQ3( q3.i, q3.j, q3.k );
        Vector3 imaginaryPartResult( imaginaryPartQ3 * q1.r + imaginaryPartQ1 * q3.r + imaginaryPartQ1 * imaginaryPartQ3 );
        Quaternion expectedResult( q1.r * q3.r - imaginaryPartQ1.dotProduct( imaginaryPartQ3 ),
                                   imaginaryPartResult.x,
                                   imaginaryPartResult.y,
                                   imaginaryPartResult.z );

        q1 *= q3;
        
        EXPECT_FLOAT_EQ( q1.r, expectedResult.r );
        EXPECT_FLOAT_EQ( q1.i, expectedResult.i );
        EXPECT_FLOAT_EQ( q1.j, expectedResult.j );
        EXPECT_FLOAT_EQ( q1.k, expectedResult.k );
    }

    TEST( Operations, ConjugateAndInverse )
    {
        Quaternion q1( 5.2f, 6.3f, 7.8f, 9.6f );
        Quaternion q2 = q1.getConjugate();

        EXPECT_EQ( q1.r, q2.r );
        EXPECT_EQ( q1.i, -q2.i );
        EXPECT_EQ( q1.j, -q2.j );
        EXPECT_EQ( q1.k, -q2.k );

        Quaternion q3 = q1.getInverse();
        float q1Norm = q1.getNorm();
        EXPECT_EQ( q3.r, q2.r / q1Norm );
        EXPECT_EQ( q3.i, q2.i / q1Norm );
        EXPECT_EQ( q3.j, q2.j / q1Norm );
        EXPECT_EQ( q3.k, q2.k / q1Norm );

        q1.normalize();
        Quaternion q4 = q1.getConjugate();
        Quaternion q5 = q1.getInverse();
        EXPECT_EQ( q4.r, q5.r );
        EXPECT_EQ( q4.i, q5.i );
        EXPECT_EQ( q4.j, q5.j );
        EXPECT_EQ( q4.k, q5.k );
    }

    TEST( Operations, ProductProperties )
    {
        // (q1 * q2)^-1 == (q2)^-1 * (q1)^-1
        Quaternion q1( 5.2f, 6.3f, 7.8f, 9.6f );
        Quaternion q2( 8.2f, 6.4f, 7.6f, 6.8f );

        Quaternion inverseProduct = ( q1 * q2 ).getInverse();
        Quaternion productInverses = q2.getInverse() * q1.getInverse();

        EXPECT_FLOAT_EQ( inverseProduct.r, productInverses.r );
        EXPECT_FLOAT_EQ( inverseProduct.i, productInverses.i );
        EXPECT_FLOAT_EQ( inverseProduct.j, productInverses.j );
        EXPECT_FLOAT_EQ( inverseProduct.k, productInverses.k );

        // ||q1 * q2|| = ||q1|| * ||q2||
        float productNorm = ( q1 * q2 ).getNorm();
        float normsProduct = q1.getNorm() * q2.getNorm();

        EXPECT_FLOAT_EQ( productNorm, normsProduct );

        // Pas commutatif
        Quaternion product1 = q1 * q2;
        Quaternion product2 = q2 * q1;

        bool isEqual = product1.r == product2.r &&
            product1.i == product2.i &&
            product1.j == product2.j &&
            product1.k == product2.k;

        EXPECT_FALSE( isEqual );

        // Associatif : (q1 q2) q3 == q1 (q2 q3)
        Quaternion q3( 2.6f, 6.4f, 4.f, 2.4f );

        Quaternion associativeResult1 = ( q1 * q2 ) * q3;
        Quaternion associativeResult2 = q1 * ( q2 * q3 );

        EXPECT_FLOAT_EQ( associativeResult1.r, associativeResult2.r );
        EXPECT_FLOAT_EQ( associativeResult1.i, associativeResult2.i );
        EXPECT_FLOAT_EQ( associativeResult1.j, associativeResult2.j );
        EXPECT_FLOAT_EQ( associativeResult1.k, associativeResult2.k );
    }

    TEST( Operations, Difference )
    {
        // d * q1 == q2 <=> q2 * conjugQ1 == d
        Quaternion q1( 5.2f, 6.3f, 7.8f, 9.6f );
        Quaternion d( 6.4f, 8.6f, 7.6f, 2.2f );
        q1.normalize();
        d.normalize();
        Quaternion q2( d * q1 );
        Quaternion product( q2 * q1.getConjugate() );

        EXPECT_FLOAT_EQ( product.r, d.r );
        EXPECT_FLOAT_EQ( product.i, d.i );
        EXPECT_FLOAT_EQ( product.j, d.j );
        EXPECT_FLOAT_EQ( product.k, d.k );
    }

    TEST( Operations, Exponentiation )
    {
        // Rotation de 30° autour de x
        float initialAngleRad = 30 * PI / 180.f;
        Quaternion q1( cos( initialAngleRad / 2 ), sin( initialAngleRad / 2 ), 0, 0 );
        q1.normalize();

        // Rotation de -120° de x
        Quaternion q2 = q1.getExponentiation( 8.f );

        float expectedAngleRad = 120 * PI / 180.f;
        float epsilon = 0.00001f;

        // Ici -cos et pas cos car la rotation est censée aller dans l'autre sens
        bool isEqual = fabs( q2.r - ( -cos( expectedAngleRad / 2.f ) ) ) < epsilon &&
                             fabs( q2.i - sin( expectedAngleRad / 2.f ) ) < epsilon &&
                             q2.j == 0.f && q2.k == 0.f;
        
        EXPECT_TRUE( isEqual );

        // Rotation de 10° (fraction de 1/3)
        Quaternion q3 = q1.getExponentiation( 1 / 3.f );

        expectedAngleRad = 10 * PI / 180.f;

        isEqual = fabs( q3.r - cos( expectedAngleRad / 2.f ) ) < epsilon &&
            fabs( q3.i - sin( expectedAngleRad / 2.f ) ) < epsilon &&
            q3.j == 0.f && q3.k == 0.f;

        EXPECT_TRUE( isEqual );

        // Rotation de 60° autour de x
        initialAngleRad = 60 * PI / 180.f;
        Quaternion q4( cos( initialAngleRad / 2 ), sin( initialAngleRad / 2 ), 0, 0 );
        q4.normalize();

        // Slerp entre q1 et q4 de 1/2 => On doit tomber sur 45° autour de x
        Quaternion slerpResult = q1.slerp( q4, 1 / 2.f );

        expectedAngleRad = 45 * PI / 180.f;
        isEqual = fabs( slerpResult.r - cos( expectedAngleRad / 2.f ) ) < epsilon &&
            fabs( slerpResult.i - sin( expectedAngleRad / 2.f ) ) < epsilon &&
            slerpResult.j == 0.f && slerpResult.k == 0.f;

        EXPECT_TRUE( isEqual );
    }

    TEST( Rotations, ApplyAngularVelocity )
    {
        // Exemple du cours 8 page 6
        Quaternion q1( 1 / 2.f, 1 / 2.f, 1 / 2.f, 1 / 2.f );
        Vector3 angularVelocity( 0, PI, 0 );
        float deltaTime = 2.f;

        q1.applyAngularVelocity( angularVelocity, deltaTime );

        EXPECT_FLOAT_EQ( q1.r, 1 / 2.f * ( 1 - PI ) );
        EXPECT_FLOAT_EQ( q1.i, 1 / 2.f * ( 1 + PI ) );
        EXPECT_FLOAT_EQ( q1.j, 1 / 2.f * ( 1 + PI ) );
        EXPECT_FLOAT_EQ( q1.k, 1 / 2.f * ( 1 - PI ) );
    }
}