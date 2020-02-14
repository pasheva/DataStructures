//
// A symtable unit test based on Catch framework
//
// << YOUR NAME >>
// U. of Illinois, Chicago
// CS 251, Spring 2020
// Project #03: symtable
//

#include <iostream>

#include "symtable.h"

#include "catch.hpp"

using namespace std;


TEST_CASE("(1) basic symtable test") 
{
  symtable<string, string>  table;
  
  REQUIRE(table.size() == 0);
  REQUIRE(table.numscopes() == 0);

  table.enterScope("global");  //GLOBAL ENTER
  
  table.insert("i", "int");
  table.insert("j", "double");
  
  REQUIRE(table.size() == 2);
  REQUIRE(table.numscopes() == 1);
  
  table.enterScope("x"); //X FUNCTION SCOPE ENTER
  table.insert("k", "char");
  table.insert("k", "char");

  /* Has to be three since it is updating the type of var, explicit casting?*/
  REQUIRE(table.size() == 3);
  REQUIRE(table.numscopes() == 2);

  table.insert("k", "int");


  REQUIRE(table.size() == 3);
  REQUIRE(table.numscopes() == 2);

  table.exitScope();
  /* When exiting the scope the variables from that scope have to be deleted. */
  REQUIRE(table.size() == 2);
  REQUIRE(table.numscopes() == 1);

  //
  // these lookups should both succeed:
  //
  string symbol;

  table.enterScope("x"); //X FUNCTION SCOPE ENTER
  table.insert("k", "char");

  REQUIRE(table.lookup("k", symbol));
  REQUIRE(symbol == "char");
  
  REQUIRE(table.lookup("i", symbol));
  REQUIRE(symbol == "int");
  
  REQUIRE(table.lookup("j", symbol));
  REQUIRE(symbol == "double");
  
  //
  // this lookup should return false:
  //
  REQUIRE((!table.lookup("x", symbol)));
}
