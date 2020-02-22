#include <iostream>
#include <string>
#include "symtable.h"
#include "catch.hpp"

using namespace std;

TEST_CASE("(3) curScope()")
{
    symtable<string, string> table;
    symtable<string,string>::Scope scopeCopy;
    symtable<string,string>::Scope scopeCopy2;

//    SECTION("Throwing an erro in case the scope is empty.")
//    {
//         REQUIRE_THROWS(table.curScope());
//    }
    SECTION("Making sure the scope is copied over properly (Deep Copy)")
    {
        table.enterScope("A");
        table.insert("a", "int");
        table.insert("b","char");
        table.insert("c","bool");

        table.enterScope("B");
        table.insert("a", "int");
        table.insert("b","char");
        table.insert("c","bool");

        scopeCopy = table.curScope();
        REQUIRE(scopeCopy.Symbols.size() == 3);
        REQUIRE(scopeCopy.Name == table.curScope().Name);

        scopeCopy.Symbols["d"] = "int";
        scopeCopy.Name = "C";
        REQUIRE(scopeCopy.Symbols.size() == 4);
        REQUIRE(table.curScope().Symbols.size() == 3);
        REQUIRE(scopeCopy.Name != table.curScope().Name);

        scopeCopy.Symbols.clear();
        REQUIRE(scopeCopy.Symbols.empty() == true);
        REQUIRE(table.curScope().Symbols.size() == 3);
    }
}