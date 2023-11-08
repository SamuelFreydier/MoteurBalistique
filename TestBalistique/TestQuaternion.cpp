#include "pch.h"
#include "Quaternion.h"

namespace QuaternionTest
{
    TEST( TestQuaternion, DefaultInitialization )
    {
        Quaternion q1;

        EXPECT_EQ( q1.r, 1 );
        EXPECT_EQ( q1.i, 0 );
        EXPECT_EQ( q1.j, 0 );
        EXPECT_EQ( q1.k, 0 );
    }
}