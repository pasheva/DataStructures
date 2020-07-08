#include<iostream>
#include"bstt.h"
#include <vector>

using namespace std;

int main(){

    bstt<int,int> tree;

    vector<int> keys = {10,15,13};

    for (int key : keys)
    {
        tree.insert(key, -key);
    }

    tree.removeLeaves();

    return 0;
}