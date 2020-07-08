#include <iostream>
#include <string>
#include <regex>   /*regular expressions*/
#include "hash.h"

using namespace std;

int HashByStationId(int id, int N){
    return id % N;
}

int HashByStationAbbrv(string abbrev, int N){
    //Removing spaces
    abbrev.erase(remove(abbrev.begin(), abbrev.end(), ' '), abbrev.end());
    int index = 0;
    for(char i : abbrev){
        index += (i - 0); //Get the ascii value for each char.
    }
    return index % N;
}

int HashByTripId(const string& id, int N){
    int index = 0;
    int len = id.length();
    index = stoi(id.substr(2,(len-2)));
    return index % N;
}

int HashByBikeId(const string& id, int N){
    int index = 0;
    int len = id.length();
    index = stoi(id.substr(1,(len-1)));
    return index % N;
}