/************************************************
 *  Title: CS251, Project 2 Matrix, UIC         *
 *                                              *
 * Author: Mariya Pasheva                       *
 *                                              *
 * System: Linux                                *
 *                                              *
 * Date: 02/01/2020                             *
 *                                              *
 * Purpose: Building and testing a matrix class *
 *          with Catch2 framework.              *
 *                                              *
 * Usage:                                       *
 * g++ -g  main.cpp -o program.exe              *
 *        ./program.exe                         *
************************************************/

//#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

#include "mymatrix.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <random>
#include <chrono>
#include <cmath>

using namespace std;

int main(){
        mymatrix<int> M;
}


//TEST_CASE("Default constructor")
//{
//    mymatrix<int> M;
//    REQUIRE(M.size() == 16);
//}
//
//TEST_CASE("Overloaded constructor")
//{
//    SECTION("More rows than columns")
//    {
//        mymatrix<int>M(10,3);
//        REQUIRE(M.size() == 30);
//    }
//    SECTION("More columns than rows")
//    {
//        mymatrix<int>M(2,10);
//        REQUIRE(M.size() == 20);
//    }
//}
//TEST_CASE("Teasting the deep copy constructor")
//{
//    mymatrix<int>M;
//    M.at(3,2) = 50;
//    mymatrix<int>M2 = M;
//
//    SECTION("Changing M2 should not reflect M")
//    {
//        M2.at(3,2) = 10;
//        REQUIRE(M2.at(3,2) == 10);
//        REQUIRE(M.at(3,2) == 50);
//    }
//}
//TEST_CASE("Testing the number of rows output")
//{
//    mymatrix<int>M;
//    mymatrix<int>M2(1,5);
//    mymatrix<int>M3(100,2);
//
//    REQUIRE(M.numrows() == 4);
//    REQUIRE(M2.numrows() == 1);
//    REQUIRE(M3.numrows() == 100);
//}
//TEST_CASE("Number of columns at a particular row")
//{
//    mymatrix<int>M;
//    mymatrix<int>M2(6,10);
//    mymatrix<int>M3(3,95);
//
//    SECTION("Changing pacritular row to a different number of columns")
//    {
//        M.growcols(1,15);
//        REQUIRE(M.numcols(1) == 15);
//    }
//
//    REQUIRE(M.numcols(0) == 4);
//    REQUIRE(M2.numcols(1) == 10);
//    REQUIRE(M3.numcols(2) == 95);
//}
//TEST_CASE("Growing a particular column")
//{
//    mymatrix<int> M;
//    mymatrix<int> M2(6, 9);
//
//    M.growcols(2,6);
//    M2.growcols(5,20);
//
//    REQUIRE(M.numcols(2) == 6);
//    REQUIRE(M2.numcols(5) == 20);
//}
//TEST_CASE("Testing the grow function")
//{
//    mymatrix<int>M;
//
//    SECTION("Grow both row and column")
//    {
//        M.grow(10,10);
//        REQUIRE(M.numrows() == 10);
//        REQUIRE(M.numcols(0) == 10);
//        REQUIRE(M.numcols(1) == 10);
//        REQUIRE(M.numcols(2) == 10);
//        REQUIRE(M.numcols(3) == 10);
//        REQUIRE(M.numcols(4) == 10);
//        REQUIRE(M.numcols(5) == 10);
//        REQUIRE(M.numcols(6) == 10);
//        REQUIRE(M.numcols(7) == 10);
//        REQUIRE(M.numcols(8) == 10);
//        REQUIRE(M.numcols(9) == 10);
//    }
//    SECTION("Grow only columns")
//    {
//        M.grow(4,7);
//        REQUIRE(M.numrows() == 4);
//        REQUIRE(M.numcols(0) == 7);
//        REQUIRE(M.numcols(1) == 7);
//        REQUIRE(M.numcols(2) == 7);
//        REQUIRE(M.numcols(3) == 7);
//    }
//    SECTION("Grow columns which are uneven")
//    {
//        M.growcols(1,15);
//        M.grow(4,9);
//        REQUIRE(M.numrows() == 4);
//        REQUIRE(M.numcols(0) == 9);
//        REQUIRE(M.numcols(1) == 15);
//        REQUIRE(M.numcols(2) == 9);
//        REQUIRE(M.numcols(3) == 9);
//    }
//}
//TEST_CASE("Testing whether we are returning the correct size")
//{
//    mymatrix<int>M;
//
//    REQUIRE(M.size() == 16);
//
//    SECTION("Different number of columns")
//    {
//        M.grow(10,10);
//        M.growcols(0,50);
//        M.growcols(9,15);
//        REQUIRE(M.size() == 145);
//    }
//}
//TEST_CASE("Accessing and altering element at row and column")
//{
//    mymatrix<int>M;
//
//    M.at(0,3) = 5;
//    REQUIRE(M.at(0,3) == 5);
//    M.at(0,3) *= 2;
//    REQUIRE(M.at(0,3) == 10);
//    M.growcols(3,10);
//    M.at(3,6) = 6;
//    REQUIRE(M.at(3,6) == 6);
//}
//TEST_CASE("Operator ()")
//{
//    mymatrix<int>M;
//
//    M(0,3) = 5;
//    REQUIRE(M(0,3) == 5);
//    M.at(0,3) *= 2;
//    REQUIRE(M(0,3) == 10);
//    M.growcols(3,10);
//    M(3,6) = 6;
//    REQUIRE(M(3,6) == 6);
//}
//TEST_CASE("Operator *")
//{
//    mymatrix<int>M1(2,2);
//    M1.at(0,0) = 1;
//    M1.at(0,1) = 2;
//    M1.at(1,0) = 3;
//    M1.at(1,1) = 4;
//    mymatrix<int>M2;
//    M2 = M1 * 3;
//    REQUIRE(M2.at(0,0) == 3);
//    REQUIRE(M2.at(0,1) == 6);
//    REQUIRE(M2.at(1,0) == 9);
//    REQUIRE(M2.at(1,1) == 12);
//}
//TEST_CASE("Testing matrix multiplication")
//{
//    mymatrix<int>M1(2,3);
//    mymatrix<int>M2(3,3);
//    //result is 2x3 matrix
//    mymatrix<int>M3;
//
//    //Setting M1 values;
//    M1.at(0,0) = 1;
//    M1.at(0,1) = 2;
//    M1.at(0,2) = 3;
//    M1.at(1,0) = 4;
//    M1.at(1,1) = 5;
//    M1.at(1,2) = 6;
//    //Stting M2 values;
//    M2.at(0,0) = 1;
//    M2.at(0,1) = 2;
//    M2.at(0,2) = 3;
//    M2.at(1,0) = 4;
//    M2.at(1,1) = 5;
//    M2.at(1,2) = 6;
//    M2.at(2,0) = 7;
//    M2.at(2,1) = 8;
//    M2.at(2,2) = 9;
//
//    M3 = M1 * M2;
//    //Testing size
//    REQUIRE(M3.size() == 6);
//    REQUIRE(M3.numrows() == 2);
//    REQUIRE(M3.numcols(0) == 3);
//    REQUIRE(M3.numcols(1) == 3);
//    //Testing values, multiplication.
//    REQUIRE(M3.at(0,0) == 30);
//    REQUIRE(M3.at(0,1) == 36);
//    REQUIRE(M3.at(0,2) == 42);
//    REQUIRE(M3.at(1,0) == 66);
//    REQUIRE(M3.at(1,1) == 81);
//    REQUIRE(M3.at(1,2) == 96);
//}
