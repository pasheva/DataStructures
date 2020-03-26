#include<iostream>
#include"avlt.h"

using namespace std;

int main(){

    avlt<int,int> tree;

    vector<int> keys = {12};

    for (int key : keys)
    {
        tree.insert(key, -key);
    }

    tree.removeLeaves();

   return 0;
}