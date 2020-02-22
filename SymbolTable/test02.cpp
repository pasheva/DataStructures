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
        REQUIRE_THROWS( table.exitScope());
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
//        CHECK_THROWS_AS( table.exitScope(), invalid_argument);
    }
    SECTION("Making sure the symmbols are updated")
    {
        table.enterScope("A");

        table.insert("a","char");
        table.insert("b", "bool");
        table.insert("c", "int");

        REQUIRE(table.numscopes() == 1);
        REQUIRE(table.size() == 3);

        table.exitScope();

        REQUIRE(table.numscopes()==0);
        REQUIRE(table.size()==0);
    }
    SECTION("Testing the time complexity")
    {
        for(int i = 0; i < 10; ++i){
            table.enterScope("Time");
        }
        for(int i = 0; i < 10; ++i){
            table.exitScope();
        }
        REQUIRE(table.numscopes()==0);
        REQUIRE(table.size() == 0);
    }
}
