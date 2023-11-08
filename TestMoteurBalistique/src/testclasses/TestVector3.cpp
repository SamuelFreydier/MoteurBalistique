#include "../catch.hpp"
#include "../../../src/Particle.h"

TEST_CASE("Vector3 operations", "[Vector3]") {
    //Vector3 v1(1.0f, 2.0f, 3.0f);
    //Vector3 v2(4.0f, 5.0f, 6.0f);
    //
    //SECTION("Vector3 multiplication by scalar") {
    //    Vector3 result = v1 * 2.0f;
    //    REQUIRE(result == Vector3(2.0f, 4.0f, 6.0f));
    //
    //    Vector3 res2 = v1 * -1.0f;
    //    REQUIRE(res2 == Vector3(-1.0f, -2.0f, -3.0f));
    //
    //    Vector3 res3 = v1 * 0.0f;
    //    REQUIRE(res3 == Vector3(0.0f, 0.0f, 0.0f));
    //}
    //
    //SECTION("Vector3 division by scalar") {
    //    Vector3 result = v2 / 2.0f;
    //    REQUIRE(result == Vector3(2.0f, 2.5f, 3.0f));
    //}
    //
    //SECTION("Vector3 dot product") {
    //    float dot = v1.dotProduct(v2);
    //    REQUIRE(dot == 32.0f);
    //}
    //
    //SECTION("Vector3 addition") {
    //    Vector3 result = v1 + v2;
    //    REQUIRE(result == Vector3(5.0f, 7.0f, 9.0f));
    //}
    //
    //SECTION("Vector3 subtraction") {
    //    Vector3 result = v1 - v2;
    //    REQUIRE(result == Vector3(-3.0f, -3.0f, -3.0f));
    //}
    //
    //SECTION("Vector3 negation") {
    //    Vector3 result = -v1;
    //    REQUIRE(result == Vector3(-1.0f, -2.0f, -3.0f));
    //}
    //
    //SECTION("Vector3 equality") {
    //    Vector3 v3(1.0f, 2.0f, 3.0f);
    //    REQUIRE(v1 == v3);
    //}
    //
    //SECTION("Vector3 inequality") {
    //    REQUIRE(v1 != v2);
    //}
    //
    //SECTION("Vector3 normalization") {
    //    Vector3 v4(3.0f, 0.0f, 0.0f);
    //    v4.normalize();
    //    REQUIRE(v4 == Vector3(1.0f, 0.0f, 0.0f));
    //}
}
