#include <iostream>
#include <string>
#include "symtable.h"
#include "catch.hpp"

using namespace std;

TEST_CASE("(1) enterScope(string)") {
    symtable<string, string> table;
    symtable<int, int> tableInt;

    SECTION("(1.1) Checking for the first scope on the front of the queue to be global") {
        table.enterScope("Global");
        REQUIRE(table.size() == 0);
        REQUIRE(table.numscopes() == 1);
        REQUIRE(table.curScope().Name == "Global");
    }
    SECTION("(1.2) Same as section 1.2 but with smaller case g") {
        table.enterScope("global");
        REQUIRE(table.size() == 0);
        REQUIRE(table.numscopes() == 1);
        REQUIRE(table.curScope().Name == "global");
    }
    SECTION("1.3 Enter multiple scopes") {
        table.enterScope("A");
        REQUIRE(table.numscopes() == 1);
        table.enterScope("A");
        REQUIRE(table.numscopes() == 2);
        table.enterScope("B");
        REQUIRE(table.numscopes() == 3);
    }
    SECTION("1.4 Insertion time complexity") {
        for (int i = 0; i < 100000; ++i) {
            table.enterScope("Time");
        }
        REQUIRE(table.numscopes() == 100000);
    }
}