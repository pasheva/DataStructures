
#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

#include "mymatrix.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <random>
#include <chrono>

#include <cmath>

#include "mymatrix.h"

using namespace std;


TEST_CASE("Test 26: matrix multiply 4x4", "[Project02]")
{
    //
    // Web site for MM calculator:
    //
    // https://matrix.reshish.com/multCalculation.php
    //
    mymatrix<int> M1;
    mymatrix<int> M2;

    for (int r = 0; r < 4; ++r)
        for (int c = 0; c < 4; ++c)
            M1(r, c) = (r * 40) + (10 * (c + 1));

    //M1._output();

    for (int r = 0; r < 4; ++r)
        for (int c = 0; c < 4; ++c)
            M2(r, c) = (80 - (r*20)) - (5 * c);

    //M2._output();

    mymatrix<int> M3;

    M3 = M1 * M2;

    //M3._output();

    REQUIRE(M3.numrows() == 4);
    REQUIRE(M3.numcols(0) == 4);
    REQUIRE(M3.numcols(1) == 4);
    REQUIRE(M3.numcols(2) == 4);
    REQUIRE(M3.numcols(3) == 4);
    REQUIRE(M3.size() == 16);

    REQUIRE(M3(0, 0) == 4000);
    REQUIRE(M3(0, 1) == 3500);
    REQUIRE(M3(0, 2) == 3000);
    REQUIRE(M3(0, 3) == 2500);

    REQUIRE(M3(1, 0) == 12000);
    REQUIRE(M3(1, 1) == 10700);
    REQUIRE(M3(1, 2) == 9400);
    REQUIRE(M3(1, 3) == 8100);

    REQUIRE(M3(2, 0) == 20000);
    REQUIRE(M3(2, 1) == 17900);
    REQUIRE(M3(2, 2) == 15800);
    REQUIRE(M3(2, 3) == 13700);

    REQUIRE(M3(3, 0) == 28000);
    REQUIRE(M3(3, 1) == 25100);
    REQUIRE(M3(3, 2) == 22200);
    REQUIRE(M3(3, 3) == 19300);
}