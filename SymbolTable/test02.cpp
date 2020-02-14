#include <iostream>
#include <string>
#include "symtable.h"
#include "catch.hpp"

using namespace std;

TEST_CASE("(2) exitScope()")
{
    symtable<string,string> table;
    SECTION("If there no scopes open currently throw error")
    {
        CHECK_THROWS_AS( table.exitScope(), invalid_argument);
    }
    SECTION("Get into global and X scope and exit scope X (latest)")
    {
        table.enterScope("global");
        REQUIRE(table.numscopes() == 1);

        table.enterScope("X");
        REQUIRE(table.numscopes() == 2);

        table.exitScope(); //X
        REQUIRE(table.numscopes() == 1);
    }
    SECTION("Exiting global scope (termination of the program)")
    {
        table.enterScope("global");
        REQUIRE(table.numscopes() == 1);

        table.exitScope();
        REQUIRE(table.numscopes() == 0);

        //Has to throw an error since there are no more scopes.
        CHECK_THROWS_AS( table.exitScope(), invalid_argument);
    }
}
