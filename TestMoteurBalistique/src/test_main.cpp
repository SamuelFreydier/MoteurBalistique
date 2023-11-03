#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE( "Exemple" )
{
    int i = 0;

    REQUIRE( i < 1 );
    REQUIRE( i == 0 );
    REQUIRE( i > 1 ); // Test qui ne passe pas
}