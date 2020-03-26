//#define CATCH_CONFIG_MAIN
//#include <catch2/catch.hpp>
//
//#include <iostream>
//#include "bstt.h"
//
//using namespace std;
//
//TEST_CASE(" Test 1")
//{
//    bstt<int,int> tree;
//    SECTION("Inserting"){
//        tree.insert(30,30);
//        tree.insert(15,15);
//        tree.insert(25,25);
//        tree.insert(50,50);
//        tree.insert(70,70);
//        tree.insert(8,8);
//        tree.insert(20,20);
//        tree.insert(28,28);
//        tree.insert(60,60);
//        tree.insert(70,70);
//        tree.insert(45,45);
//        tree.insert(80,80);
//        tree.insert(70,70);
//    }
//    SECTION("Fiding value with overloaded []"){
//        tree.insert(30,30);
//        tree.insert(15,15);
//        tree.insert(25,25);
//        tree.insert(50,50);
//        tree.insert(70,70);
//        tree.insert(8,8);
//
////        int x = tree[30];
////        x = tree[15];
////        x = tree[25];
//
//        REQUIRE(tree[30] == 30);
//        REQUIRE(tree[15] == 15);
//        REQUIRE(tree[25] == 25);
//        REQUIRE(tree[50] == 50);
//        REQUIRE(tree[70] == 70);
//        REQUIRE(tree[8] == 8 );
//
//    }
//    SECTION("Testing overloading the () "){
//        tree.insert(30,30);
//        tree.insert(15,15);
//        tree.insert(25,25);
//        tree.insert(50,50);
//        tree.insert(70,70);
//        tree.insert(8,8);
//        tree.insert(20,20);
//        tree.insert(28,28);
//        tree.insert(60,60);
//        tree.insert(70,70);
//        tree.insert(45,45);
//        tree.insert(80,80);
//        tree.insert(70,70);
//
//        REQUIRE(tree(15) == 25);
//        REQUIRE(tree(25) == 28);
//        REQUIRE(tree(30) == 50);
//        REQUIRE(tree(50) == 70);
//        REQUIRE(tree(80) == int{});
//    }
//    SECTION("NEXT BEGIN"){
//        tree.insert(30,30);
//        tree.insert(15,15);
//        tree.insert(25,25);
//        tree.insert(50,50);
//        tree.insert(70,70);
//        tree.insert(8,8);
//        tree.insert(20,20);
//        tree.insert(28,28);
//        tree.insert(60,60);
//        tree.insert(70,70);
//
//        int key;
//        tree.begin();
//        while(tree.next(key)){
//            cout << key << endl;
//        }
//    }
//}
//


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