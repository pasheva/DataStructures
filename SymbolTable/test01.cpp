#include <iostream>
#include <string>
#include "symtable.h"
#include "catch.hpp"

using namespace std;

TEST_CASE("(1) enterScope(string)")
{
    symtable<string,string> table;

    SECTION("(1.1) Checking for exception when not entering global scope first")
    {
        //The first scope to enter has to be the global, otherwise an exception is thrown.
        REQUIRE_THROWS_AS(table.enterScope("x"), invalid_argument);
    }
    SECTION("(1.2) Checking for the first scope on the front of the queue to be global")
    {
        table.enterScope("Global");
        REQUIRE(table.numscopes() == 1);
        REQUIRE(table.curScope().Name == "Global");
    }
    SECTION("(1.3) Same as section 1.2 but with smaller case g")
    {
        table.enterScope("global");
        REQUIRE(table.numscopes() == 1);
        REQUIRE(table.curScope().Name == "global");
    }


}