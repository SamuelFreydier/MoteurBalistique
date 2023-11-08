#include "pch.h"
#include "Vector.h"

// Define a tolerance for floating-point comparisons
const float EPSILON = 1e-6;

// Test fixture class for Vector3
class Vector3Test : public testing::Test {
protected:
    Vector3 v1;
    Vector3 v2;

    void SetUp() override {
        v1 = Vector3(1.0f, 2.0f, 3.0f);
        v2 = Vector3(4.0f, 5.0f, 6.0f);
    }
};

// Test cases
TEST_F(Vector3Test, ConstructorAndEquality) {
    Vector3 v3;
    ASSERT_EQ(v1, v1);
    ASSERT_NE(v1, v2);
    ASSERT_EQ(v3, Vector3(0.0f, 0.0f, 0.0f));
}

TEST_F(Vector3Test, ScalarMultiplication) {
    Vector3 result = v1 * 2.0f;
    ASSERT_NEAR(result.x, 2.0f, EPSILON);
    ASSERT_NEAR(result.y, 4.0f, EPSILON);
    ASSERT_NEAR(result.z, 6.0f, EPSILON);
}

TEST_F(Vector3Test, ScalarDivision) {
    Vector3 result = v2 / 2.0f;
    ASSERT_NEAR(result.x, 2.0f, EPSILON);
    ASSERT_NEAR(result.y, 2.5f, EPSILON);
    ASSERT_NEAR(result.z, 3.0f, EPSILON);
}

TEST_F(Vector3Test, CrossProduct) {
    Vector3 result = v1 * v2;
    Vector3 expected(-3.0f, 6.0f, -3.0f);
    ASSERT_EQ(result, expected);
}

TEST_F(Vector3Test, DotProduct) {
    float result = v1.dotProduct(v2);
    ASSERT_NEAR(result, 32.0f, EPSILON);
}

TEST_F(Vector3Test, VectorAddition) {
    Vector3 result = v1 + v2;
    Vector3 expected(5.0f, 7.0f, 9.0f);
    ASSERT_EQ(result, expected);
}

TEST_F(Vector3Test, VectorSubtraction) {
    Vector3 result = v2 - v1;
    Vector3 expected(3.0f, 3.0f, 3.0f);
    ASSERT_EQ(result, expected);
}

TEST_F(Vector3Test, VectorNormalization) {
    Vector3 v3(1.0f, 1.0f, 1.0f);
    Vector3 normalizedV3 = v3.normalized();
    float norm = normalizedV3.norm();
    ASSERT_NEAR(norm, 1.0f, EPSILON);
}

TEST_F(Vector3Test, Distance) {
    float dist = v1.distance(v2);
    ASSERT_NEAR(dist, 5.196152, EPSILON);
}

TEST_F(Vector3Test, Clear) {
    v1.clear();
    ASSERT_EQ(v1, Vector3(0.0f, 0.0f, 0.0f));
}