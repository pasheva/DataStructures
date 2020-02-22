#include <iostream>
#include "symtable.h"
#include "catch.hpp"
#include <string>

using namespace std;


TEST_CASE("(5) lookup()")
{
    symtable<string,string> table;
    symtable<int,int> tableT;
    string symbol;
    bool eval;

     SECTION("In case we are trying to look up in a scope which does not exist")
     {
         eval = table.lookup("i",symbol);
         REQUIRE(!eval);
     }
     SECTION("Testign the whole scope")
     {
         table.enterScope("A");
         table.insert("a","int");
         table.insert("a1","char");
         table.insert("a2","int");

         table.enterScope("B");
         table.insert("b","int");
         table.insert("b1","char");
         table.insert("b2","int");

         table.lookup("a",symbol);
         REQUIRE(symbol == "int");
         table.lookup("a1",symbol);
         REQUIRE(symbol == "char");
         table.lookup("a2",symbol);
         REQUIRE(symbol == "int");

         table.lookup("b",symbol);
         REQUIRE(symbol == "int");
         table.lookup("b1",symbol);
         REQUIRE(symbol == "char");
         table.lookup("b2",symbol);
         REQUIRE(symbol == "int");

         table.insert("b2","char");
         table.lookup("b2",symbol);
         REQUIRE(symbol == "char");

         eval = table.lookup("c",symbol);
         REQUIRE(!eval);
         REQUIRE(symbol == "char");
     }
     SECTION("Testing latest scope.")
     {
         table.enterScope("A");
         table.insert("a","int");
         table.insert("a1","char");
         table.insert("a2","int");

         table.enterScope("B");
         table.insert("b","int");
         table.insert("b1","char");
         table.insert("b2","int");


         table.lookup("b",symbol, symtable<basic_string<char>, basic_string<char> >::ScopeOption::CURRENT);
         REQUIRE(symbol == "int");
         table.lookup("b1",symbol, symtable<basic_string<char>, basic_string<char> >::ScopeOption::CURRENT);
         REQUIRE(symbol == "char");
         table.lookup("b2",symbol,symtable<basic_string<char>, basic_string<char> >::ScopeOption::CURRENT);
         REQUIRE(symbol == "int");

         eval = table.lookup("a",symbol, symtable<basic_string<char>, basic_string<char> >::ScopeOption::CURRENT);
         REQUIRE(!eval);
         REQUIRE(symbol == "int");
         eval = table.lookup("a1",symbol, symtable<basic_string<char>, basic_string<char> >::ScopeOption::CURRENT);
         REQUIRE(!eval);
         REQUIRE(symbol == "int");
         eval = table.lookup("a2",symbol, symtable<basic_string<char>, basic_string<char> >::ScopeOption::CURRENT);
         REQUIRE(!eval);
         REQUIRE(symbol == "int");

         table.insert("b2","char");
         table.lookup("b2",symbol, symtable<basic_string<char>, basic_string<char> >::ScopeOption::CURRENT);
         REQUIRE(symbol == "char");

         eval = table.lookup("c",symbol, symtable<basic_string<char>, basic_string<char> >::ScopeOption::CURRENT);
         REQUIRE(!eval);
         REQUIRE(symbol == "char");
     }
     SECTION("Testing global")
     {

         table.enterScope("A");
         table.insert("a","int");
         table.insert("a1","char");
         table.insert("a2","int");

         table.enterScope("B");
         table.insert("b","int");
         table.insert("b1","char");
         table.insert("b2","int");

         table.lookup("a",symbol, symtable<basic_string<char>, basic_string<char> >::ScopeOption::GLOBAL);
         REQUIRE(symbol == "int");
         table.lookup("a1",symbol, symtable<basic_string<char>, basic_string<char> >::ScopeOption::GLOBAL);
         REQUIRE(symbol == "char");
         table.lookup("a2",symbol, symtable<basic_string<char>, basic_string<char> >::ScopeOption::GLOBAL);
         REQUIRE(symbol == "int");

         eval = table.lookup("b",symbol,symtable<basic_string<char>, basic_string<char> >::ScopeOption::GLOBAL);
         REQUIRE(!eval);
         REQUIRE(symbol == "int");
         eval = table.lookup("b1",symbol, symtable<basic_string<char>, basic_string<char> >::ScopeOption::GLOBAL);
         REQUIRE(!eval);
         REQUIRE(symbol == "int");
         eval = table.lookup("b2",symbol, symtable<basic_string<char>, basic_string<char> >::ScopeOption::GLOBAL);
         REQUIRE(!eval);
         REQUIRE(symbol == "int");

         eval = table.lookup("c",symbol, symtable<basic_string<char>, basic_string<char> >::ScopeOption::GLOBAL);
         REQUIRE(!eval);
         REQUIRE(symbol == "int");
     }
     SECTION("Testing time complexity") {
         int symbolT;
         tableT.enterScope("Scope");
         for (int i = 0; i < 10000000; ++i) {
             tableT.insert(i, 1);
         }
         for (int i = 0; i < 100000; ++i) {
             eval = tableT.lookup(i, symbolT);
         }
         REQUIRE(tableT.numscopes() == 1);
         REQUIRE(tableT.size() == 10000000);
     }
}