#include <iostream>
#include <string>
#include "symtable.h"
#include "catch.hpp"

using namespace std;

TEST_CASE("(0) size() and numscopes()")
{
    symtable<string, string> table;
    REQUIRE(table.size() == 0);
    REQUIRE(table.numscopes() == 0);
}