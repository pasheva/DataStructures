#pragma once

#include <iostream>
#include <functional>

using namespace std;

template<typename KeyT, typename ValueT>
class hashmap
{
private:
    struct HashEntry
  {
    bool   ContainsData;  // false => empty, true => contains data
    KeyT   Key;           // (key, value)
    ValueT Value;
    //Default value for ContainsData upon struct instantiation .
    HashEntry()
    {
      ContainsData = false;
    }
  };

  HashEntry* HT;  // hash table array of structs
  int        N{};   // capacity of hash table (# of locations or array size)

  //
  //copy consturctor
  //deep copy of the object
  //
  void _fullCopy(const hashmap& m){
       this->N = m.N;
       this->HT = new HashEntry[N];
       for(int i = 0; i < m.N; i++){
           this->HT[i] = m.HT[i];
       }
  }

public:


  ValueT getEntryValue(int i){
      return HT[i].Value;
  }

  KeyT getEntryKey(int i){
       return HT[i].Key;
  }

  bool hasData(int i){
        return HT[i].ContainsData;
  }

  //
  // constructor: N is the size of the underlying hash table
  //
  hashmap(int N)
  {
    this->N = N;
    this->HT = new HashEntry[N];
  }


  //
  // destructor
  //
  virtual ~hashmap()
  {
    delete[] this->HT;
  }
  

  //
  // copy constructor: dep copy
  //
  hashmap(const hashmap& m){
    _fullCopy(m);
  }

  // copy construcotr
  // making a deep copy of object.
  hashmap& operator=(const hashmap& m){
      delete[] this->HT;
      this->N = 0;
      _fullCopy(m);
      return *this;
  }
  //
  // getN
  //
  // Returns the capacity of the hash table, i.e. the value N that
  // was passed to the constructor.
  //
  int getN() const
  {
    return this->N;
  }

  //
  // insert
  //
  //Inserts the given (key, value) pair into the hash table, overwriting
  // the previous value if already inserted.  If the insert was successful
  // then true is returned, otherwise false is returned (meaning the key
  // was not hashed successfully, e.g. due to improper formatting).
  //
  // NOTE: the caller must provide a function Hash(key, N) that returns
  // an integer in the the range 0..N-1, inclusive, if the key is valid.
  // If the key is invalid, the hash function must return a negative
  // integer. The hash function is provided by simply calling insert
  // with the name of your hash function.  Example:
  //
  //   int myHash(string key, int N)
  //   { ... }
  //
  //   int N = 10000;
  //   hashmap<string, double>  hmap(N);  
  //
  //   if (hmap.insert("deep-dish pizza", 19.99, myHash))
  //     cout << "inserted" << endl;
  //   else
  //     cout << "not inserted?!" << endl;
  // is open (ContainsData is false).  If so, store the data and
  // change ContainsData to true.  If not, "probe" the next
  // location --- i.e. advance to the next location and
  // try again.  Be careful to wrap around to the top of the array
  // if you reach the bottom!
  //
  bool insert(KeyT key, ValueT value, function<int(KeyT,int)> Hash)
  {
    //
    // Call the provided hash function with the key, and N, the size
    // of our hash table:
    //
    int index = Hash(key, this->N);
    int binsProbed = 0;

      if (index < 0 || index >= N)  // invalid key or hash function:
      {
          return false;
      }
      else{
          while (binsProbed < this->N) {
              if (!HT[index].ContainsData) {
                  HT[index].Key = key;
                  HT[index].Value = value;
                  HT[index].ContainsData = true;
                  break;
              }
              else if(HT[index].Key == key) { //If it exists update the value.
                  HT[index].Value = value;
                  break;
              }

              index = (index + 1) % this->N;
              binsProbed++;
          }
      return true;
      }
  }

  //
  // search
  //
  // Searches the hash table for the matching key, and if found then
  // (1) the value is returned via the reference parameter and (2) true
  // is returned.  If the key could not be found, or if the hash 
  // function returned an invalid value (e.g. due to an improperly 
  // formatted key), then false is returned.
  //
  // NOTE: the caller must provide a function Hash(key, N) that returns
  // an integer in the the range 0..N-1, inclusive, if the key is valid.
  // If the key is invalid, the hash function must return a negative
  // integer. The hash function is provided by simply calling search
  // with the name of your hash function.  Example:
  //
  //   int myHash(string key, int N)
  //   { ... }
  //
  //   int N = 10000;
  //   hashmap<string, double>  hmap(N);
  //   ...
  //   double price;
  //   if (hmap.search("deep-dish pizza", price, myHash))
  //     cout << "deep-dish piazza costs " << price << endl;
  //   else
  //     cout << "not found?!" << endl;
  //
  bool search(KeyT key, ValueT& value, function<int(KeyT,int)> Hash) const {
      //
      // Call the provided hash function with the key, and N, the size
      // of our hash table:
      //
      int index = Hash(key, this->N);
      int binsProbed = 0;

      if (index < 0 || index >= N){ // invalid key or hash function:
            return false;
      }
      else if (HT[index].ContainsData)  // hash table contains data:
      {
          while (HT[index].ContainsData && binsProbed < this->N) { //stops once an empty bucket is found.
              if (HT[index].Key == key) {
                  value = HT[index].Value;
                  return true;
              }
              index = (index + 1) % this->N;
              binsProbed++;
          }

          return false;
      }
      return false; // default
  }
};



