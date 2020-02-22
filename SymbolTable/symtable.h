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
 * 3. We can have the same scope name multiple times,
 *    such as recursive calls and
 *    oveloaded functions.
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
class symtable{

public:
  //
  // A symbol table is a STACK<SCOPE>.  Every scope has a name, and
  // we use a map to hold the symbols in that scope.  You can *add* to
  // this class, but you must use the Name and Symbols as given to store
  // the scope name, and scope symbols, respectively.
  //
  //--------------------------------------------------------------Scope {
  class Scope{

  public:
    string              Name; //Scope name
    map<KeyT, SymbolT>  Symbols; //KeyT (name of var) SymbolT (type of var)

    //constructors:
    //Default
    Scope(){
      // default creates empty string and an empty map:
    }

    Scope(string name){
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
  enum class ScopeOption{
    ALL,  // 0
    CURRENT, // 1
    GLOBAL // 2
  };

  /**
   * default constructor:
   *
   * Creates a new, empty symbol table.  No scope is open.
   */
  symtable(){
      this->numOfSymbols = 0;
      this->numOfScopes = 0;
  }



  /**
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
   */
  int size() const{

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
  int numscopes() const{

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
  void enterScope(string name){


      allScopes.push_back(name);
      this->numOfScopes++;
  }


  /**
   * @details
   * Exits the current open scope, discarding all symbols in this scope.
   * This effectively "pops" the symbol table so that it returns to the
   * previously open scope.
   *
   * @remarks
   * A runtime_error is thrown if no scope is currently open.
   *
   * Complexity: O(1)
   */
  void exitScope(){

      if(allScopes.empty()){

          throw invalid_argument("runtime_error");
      }

      //Erasin the scope name since we are exiting it.
      allScopes.back().Name.erase();
      //Updating by removing all the variables initilized in the local current scope.
      numOfSymbols -= allScopes.back().Symbols.size();
      allScopes.back().Symbols.clear();

      allScopes.pop_back();
      this->numOfScopes--;
  }


  /**
   * @details
   * Returns a copy of the CURRENT scope.  A runtime_error is thrown if
   * no scope is currently open.
   *
   * @remark
   *  The current scope in the one on the back of the deque
   *  Returning a deep copy.
   *
   * Complexity: O(N) where N is the # of symbols in the current scope
   *
   * @return a copy of the current scope [Name, Symbols<KeyT,SymbolT>
   */
  Scope curScope() const{

      if(allScopes.empty()){

          throw invalid_argument("runtime_error");
      }

      string currScopeName = allScopes.back().Name;
      //Copy over scope name;
      Scope copy(currScopeName);
      //Copy over map; deep copy
      copy.Symbols = allScopes.back().Symbols;

      return copy;
  }

  /**
   * @details
   * Inserts the (key, symbol) pair in the CURRENT scope.  If the key
   * already exists in the current scope, the associated symbol is replaced
   * by this new symbol.
   *
   * @remarks
   * Return an exception in case we are trying to insert when there is no scope.
   * Also, first it is checked whether they key exists since we update the number of
   * symbols when we are assigning a new key.
   *
   * Complexity: O(lgN) where N is the # of symbols in current scope
   * deque.back() O(1)
   * map.count() O(log n)
   *
   *
   */
  void insert(KeyT key, SymbolT symbol){
      /* The emplace method returns a pair
       * first: a map iterator to an equivalent element
       * second: True if the values has been inserted sucesfully
       *         False if the values has not been inserted since the
       *               key already exists.
       */
      pair<typename map<KeyT, SymbolT>::iterator, bool> mapCheck;

      //In the case of no scope at all.
      if(allScopes.empty()){
         throw invalid_argument("runtime_error");
      }

      //Current scope is the last one pushed to the back.
      mapCheck = allScopes.back().Symbols.emplace(key, symbol); //O(log n)

      if(mapCheck.second){
          //If True the key and value pair has been inserted.
          numOfSymbols++;
      }
      else{
          //Changing the symbol of an existent key.
          //Likely useful when explicit type casting.
          allScopes.back().Symbols.at(key) = symbol; //O(log n)
      }

  }

  /**
   *
   * Searches the symbol table for the first (key, symbol) pair that
   * matches the given key.  The search starts in the current scope, and
   * proceeds "outward" to the GLOBAL scope.  If a matching (key, symbol)
   * pair is found, true is returned along with a copy of the symbol (via
   * "symbol" reference parameter).  If not found, false is returned and
   * the "symbol" parameter is left unchanged.
   *
   * NOTE: the search can be controlled by the "option" parameter.  By
   * default, the entire symbol table is searched as described above.
   * However, the search can also be limited to just the current scope,
   * or just the GLOBAL scope, via the "option" parameter.
   *
   * Example:
   *   symtable<string,string>  table;
   *   string                   symbol;
   *   bool                     found;
   *   ...
   *   found = table.lookup("i",
   *                        symbol,
   *                        symtable<string,string>::ScopeOption::CURRENT);
   *
   * Complexity: O(SlgN) where S is the # of scopes and N is the largest #
   * of symbols in any one scope
   */
  bool lookup(KeyT key, SymbolT& symbol, ScopeOption option = ScopeOption::ALL) const
  {

      //If wrong scope has been passed.
      if((option != ScopeOption::ALL) && (option != ScopeOption::CURRENT) && (option !=ScopeOption::GLOBAL)){
          throw invalid_argument("Option scope has to be either ALL, CURRENT or GLOBAL");
      }

      if(option == ScopeOption::ALL){
            for(int i = ((this->numscopes()) - 1); i >= 0; --i){ //N
                auto ptr = this->allScopes.at(i).Symbols.find(key); //logn
                if (ptr != this->allScopes.at(i).Symbols.end()){
                    symbol = ptr->second;
                    return true;
                }
            }
      }
      else if(option == ScopeOption::CURRENT){
          auto ptr = this->allScopes.back().Symbols.find(key); //O(1) && O(log n)
          if (ptr != this->allScopes.back().Symbols.end()){
              symbol = ptr->second;
              return true;
          }
      }
      else{ //GLOBAL
          auto ptr = this->allScopes.front().Symbols.find(key); //O(1) && O(log n)
          if (ptr != this->allScopes.front().Symbols.end()){
              symbol = ptr->second;
              return true;
          }
      }

      return false;
  }

  /**
   * Dumps the contents of the symbol table to the output stream,
   * starting with the current scope and working "outward" to the GLOBAL
   * scope.  You can dump the entire symbol table (the default), or dump
   * just the current scope or global scope; this is controlled by the
   * "option" parameter.
   *
   * Example:
   *   symtable<string,string>  table;
   *   ...
   *   table.dump(std::cout, symtable<string,string>::ScopeOption::GLOBAL);
   *
   * Complexity: O(S*N) where S is the # of scopes and N is the largest #
   * of symbols in any one scope
   */
  void dump(ostream& output, ScopeOption option = ScopeOption::ALL) const
  {
        output << "**************************************************" << endl;

      if (option == ScopeOption::ALL) {
          output << "*************** SYMBOL TABLE (ALL) ***************" << endl;
          output << "** # of scopes: " << this->numscopes() << endl;
          output << "** # of symbols: " << this->size() << endl;
          for (int i = ((this->numscopes()) - 1); i >= 0; --i) { //N
                output << "** " << allScopes.at(i).Name << " **" << endl;
                for(const auto& pair : allScopes.at(i).Symbols){ //N
                    output << pair.first << ": " << pair.second << endl;
                }
        }
        }else if (option == ScopeOption::CURRENT) {
            output << "*************** SYMBOL TABLE (CUR) ***************" << endl;
            output << "** # of scopes: " << this->numscopes() << endl;
            output << "** # of symbols: " << this->size() << endl;
            output << "** " << allScopes.back().Name << " **" << endl;
            for(const auto& pair : allScopes.back().Symbols){ //N
                output << pair.first << ": " << pair.second << endl;
            }
        }else { // global:
            output << "*************** SYMBOL TABLE (GBL) ***************" << endl;
            output << "** # of scopes: " << this->numscopes() << endl;
            output << "** # of symbols: " << this->size() << endl;
            output << "** " << allScopes.front().Name << " **" << endl;
            for(const auto& pair : allScopes.front().Symbols){ //N
                output << pair.first << ": " << pair.second << endl;
            }
        }
        output << "**************************************************" << endl;
  }

};
