#include <iostream>

#include "symtable.h"

#include "catch.hpp"

using namespace std;


TEST_CASE("(4) insert() ")
{
    symtable<string,string> table;
    symtable<int,string>tableT;

//    SECTION("Inserting with no scope available")
//    {
//        REQUIRE_THROWS(table.insert("i","int"));
//
//    }
    SECTION("Testing basic isnertion")
    {
        table.enterScope("A");
        table.insert("a","int");
        table.insert("b","bool");

        REQUIRE(table.size() == 2);

        table.enterScope("B");
        table.insert("c","int");

        REQUIRE(table.size() == 3);

        table.exitScope();
        REQUIRE(table.size() == 2);

        table.enterScope("B");
        REQUIRE(table.size()==2);
    }
    SECTION("Testing inserting the same var name with different symbol (type casting)")
    {
        table.enterScope("A");
        table.insert("i","int");
        table.insert("a","char");

        REQUIRE(table.size() == 2);

        //Exact same element
        table.insert("i","int");
        REQUIRE(table.size() == 2);
        //Only changing the type.
        table.insert("i","char");
        table.insert("a","bool");
        REQUIRE(table.size() == 2);
    }
    SECTION("Testign time complexity")
    {

        tableT.enterScope("A");
        for(int i = 0; i < 100000; ++i){
            tableT.insert(i,"time");
        }
    }

}
