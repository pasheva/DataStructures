/*symtable.h*/

// 
// << Mariya Pasheva >>
// U. of Illinois, Chicago
// CS 251: Spring 2020
// Project #03: symtable
//
// Symbol Table: a symbol table is a stack of scopes, typically used by a
// compiler to keep track of symbols in a program (functions, variables,
// types, etc.).  In most programming languages, you "enter scope" when you 
// see {, and "exit scope" when you see the corresponding }.  Example:
//
// int main()
// {              <-- enterScope()
//    int i;      <-- enter "i" into symbol table as type "int"
//    .
//    .
//    while (true)
//    {              <-- enterScope()
//       char i;     <-- enter "i" into symbol table as type "char"
//
// Notice there are two variables named "i", which is legal because
// they are in different scopes.

/************************************************ 
 *                    NOTES
 *                    ^^^^^
 * 1. We are dealing with two tpyes of scopes
 *    local {block surounded} and global
 *    scope.
 * 2. Each new scope is pushed to a stack.
 * 3. Functions of symtable:
 *          - size();
 *          - numscopes();
 *          - enterScope(string);
 *          - exitScope();
 *          - curScope();
 *          - insert(KeyT, SymbolT)
 *          - lookup(KeyT, SymbolT&, ScopeOption option = ScopeOption::ALL)
 *          - dump(ostream& output, ScopeOption option = ScopeOption::ALL)
 *
 *
 *                VIZUALIZATION
 *                ^^^^^^^^^^^^^
 * deque:
 *  front -> [Global,Map<valName,valType]-[currentScope,Map<varName,varType>]...<- back
 *                   ^
 *                 Name,
 *                 Map<KeyT, SymbolT>
 *                 {
 *                   [i, int]
 *                   [someVal, char]
 *                   [otherVal, string]
 *                 }
 ************************************************/

#pragma once

#include <iostream>
#include <deque>
#include <map>
#include <deque>
#include <string>

using namespace std;

/*
 * Typename tells the compiler to represent the word as a type.
 * --polymorphism
 */
template<typename KeyT, typename SymbolT> //KeyType and SymbolType
class symtable
{
public:
  //
  // A symbol table is a STACK<SCOPE>.  Every scope has a name, and
  // we use a map to hold the symbols in that scope.  You can *add* to
  // this class, but you must use the Name and Symbols as given to store
  // the scope name, and scope symbols, respectively.
  //
  //--------------------------------------------------------------Scope {
  class Scope
  {
  public:
    string              Name; //Scope name
    map<KeyT, SymbolT>  Symbols; //KeyT (name of var) SymbolT (type of var)

    //constructors:
    //Default
    Scope()
    {
      // default creates empty string and an empty map:
    }

    Scope(string name)
    {
      this->Name = name;
      // empty map created by map's constructor:
    }
  };
  //-------------------------------------------------------Scope }

private:

  deque<Scope> allScopes; //When first called creates default empty map.

  /* All the symbols that have been added. Allowing for O(1) accessing. */
  int numOfSymbols;

  int numOfScopes;

public:

  //Scoped enuminator: strongly typed, strongly copied.
  //Similar to a set of values;
  //Assigment works as ScopeOption varName = ScopeOption::ALL,CURRENT,GLOBAL;
  enum class ScopeOption
  {
    ALL,  // 0
    CURRENT, // 1
    GLOBAL // 2
  };

  /**
   * default constructor:
   *
   * Creates a new, empty symbol table.  No scope is open.
   */
  symtable()
  {
      this->numOfSymbols = 0;
      this->numOfScopes = 0;
  }

  /**
   *
   * @details
   * Returns total # of symbols in the symbol table.
   * The number is being altered in the insert function
   * and the exitScope() function.
   *
   * @remarks
   * Returning it in such way prevent us from looping through each
   * Scope and getting the map size, therefore, preventing unnecessary O(n)
   *
   * Complexity: O(1)
   *
   * @return the number of all variables
   *
   */
  int size() const
  {
      return this->numOfSymbols;
  }


  /**
   * @details
   * Returning the # of scopes.
   * The number is by default 0.
   *
   * Complexity: O(1)
   * @return the number of scopes onto the stack. (NOT all elements)
   */
  int numscopes() const
  {
    return this->numOfScopes;
  }

  /**
   * @details
   * Enters a new, open scope in the symbol table, effectively "pushing" on
   * a new scope.  You must provide a name for the new scope, although
   * the name is currently used only for debugging purposes.
   *
   * @note
   * The first scope you enter is known as the GLOBAL scope, since this
   * is typically where GLOBAL symbols are stored.
   *
   * @remarks
   * Solution to the above note would be to check if the queue is empty and check
   * whether the first Scope pushed to the deque is the global one. If not,
   * then throw an exception.
   *
   * Complexity: O(1)
   */
  void enterScope(string name)
  {
      if(allScopes.empty()) //O(1)
      {
          if( name[0] == 'G' || name[0] == 'g')
          {
              allScopes.push_front(name);
              numOfScopes++;
          }
          else
          {
              throw invalid_argument("Global scope missing");
          }
      }
      else
      {
          allScopes.push_back(name);
          numOfScopes++;
      }
//      deque<Scope>::iterator it = find(this->allScopes.begin(), this->allScopes.end(), name);
//
//      if ((*it).Name == name)

  }

  //
  // exitScope
  //
  // Exits the current open scope, discarding all symbols in this scope.
  // This effectively "pops" the symbol table so that it returns to the 
  // previously open scope.  A runtime_error is thrown if no scope is 
  // currently open.  
  //
  // Complexity: O(1)
  //
  void exitScope()
  {
      if(allScopes.empty())
      {
          throw invalid_argument("runtime_error");
      }
      allScopes.back().Name.erase();
      //Updating by removing all the variables initilized in the local current scope.
      numOfSymbols -= allScopes.back().Symbols.size();
      allScopes.back().Symbols.clear();

      allScopes.pop_back();
      numOfScopes--;
  }

  /**
   * @details
   * Returns a copy of the CURRENT scope.  A runtime_error is thrown if
   * no scope is currently open.
   *
   * @remark
   *  The current scope in the one on the back of the deque
   *
   * Complexity: O(N) where N is the # of symbols in the current scope
   *
   * @return a copy of the current scope [Name, Symbols<KeyT,SymbolT>
   */
  Scope curScope() const
  {
      if(allScopes.empty())
      {
          throw invalid_argument("runtime_error");
      }
      string currScopeName = allScopes.back().Name;
      //Copy over scope name;
      Scope copy(currScopeName);
      //Copy over map; deep copy
      copy.Symbols = allScopes.back().Symbols;

      return copy;
  }

  //
  // insert
  //
  // Inserts the (key, symbol) pair in the *current* scope.  If the key
  // already exists in the current scope, the associated symbol is replaced
  // by this new symbol.
  //
  // Complexity: O(lgN) where N is the # of symbols in current scope
  //
  void insert(KeyT key, SymbolT symbol)
  {
    //
    // TODO:
    //


  }

  //
  // lookup
  //
  // Searches the symbol table for the first (key, symbol) pair that 
  // matches the given key.  The search starts in the current scope, and 
  // proceeds "outward" to the GLOBAL scope.  If a matching (key, symbol)
  // pair is found, true is returned along with a copy of the symbol (via 
  // "symbol" reference parameter).  If not found, false is returned and
  // the "symbol" parameter is left unchanged.
  //
  // NOTE: the search can be controlled by the "option" parameter.  By 
  // default, the entire symbol table is searched as described above.
  // However, the search can also be limited to just the current scope, 
  // or just the GLOBAL scope, via the "option" parameter.
  //
  // Example:
  //   symtable<string,string>  table;
  //   string                   symbol;
  //   bool                     found;
  //   ...
  //   found = table.lookup("i", 
  //                        symbol, 
  //                        symtable<string,string>::ScopeOption::CURRENT);
  //
  // Complexity: O(SlgN) where S is the # of scopes and N is the largest #
  // of symbols in any one scope
  //
  bool lookup(KeyT key, 
              SymbolT& symbol, 
              ScopeOption option = ScopeOption::ALL) const
  {
    //
    // TODO:
    //
    
    return false;
  }

  //
  // dump
  // 
  // Dumps the contents of the symbol table to the output stream, 
  // starting with the current scope and working "outward" to the GLOBAL
  // scope.  You can dump the entire symbol table (the default), or dump
  // just the current scope or global scope; this is controlled by the 
  // "option" parameter.
  //
  // Example:
  //   symtable<string,string>  table;
  //   ...
  //   table.dump(std::cout, symtable<string,string>::ScopeOption::GLOBAL);
  //
  // Complexity: O(S*N) where S is the # of scopes and N is the largest #
  // of symbols in any one scope
  //
  void dump(ostream& output, ScopeOption option = ScopeOption::ALL) const
  {
    output << "**************************************************" << endl;

    if (option == ScopeOption::ALL)
      output << "*************** SYMBOL TABLE (ALL) ***************" << endl;
    else if (option == ScopeOption::CURRENT)
      output << "*************** SYMBOL TABLE (CUR) ***************" << endl;
    else // global:
      output << "*************** SYMBOL TABLE (GBL) ***************" << endl;

    output << "** # of scopes: " << this->numscopes() << endl;
    output << "** # of symbols: " << this->size() << endl;

    //
    // output format per scope:
    //
    // ** scopename **
    // key: symbol
    // key: symbol
    // ...
    //

    output << "**************************************************" << endl;
  }

};
