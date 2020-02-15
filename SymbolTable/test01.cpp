#include <iostream>
#include <string>
#include "symtable.h"
#include "catch.hpp"

using namespace std;

TEST_CASE("(1) enterScope(string)")
{
    symtable<string,string> table;

    SECTION("(1.1) Checking for the first scope on the front of the queue to be global")
    {
        table.enterScope("Global");
        REQUIRE(table.numscopes() == 1);
        REQUIRE(table.curScope().Name == "Global");
    }
    SECTION("(1.2) Same as section 1.2 but with smaller case g")
    {
        table.enterScope("global");
        REQUIRE(table.numscopes() == 1);
        REQUIRE(table.curScope().Name == "global");
    }
}