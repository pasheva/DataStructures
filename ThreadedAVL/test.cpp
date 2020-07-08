/*test.cpp*/

//
// An AVL unit test based on Catch framework
//

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>


#include <iostream>
#include <vector>
#include <algorithm>
#include "avlt.h"

using namespace std;


TEST_CASE("(4.1) big tree -- 1..N, balanced?")
{
   avlt<int, int>  tree;

   for (int i = 1; i < 4096; ++i)
   {
//        cout << " After insertng " << i << " " << endl;
       tree.insert(i, -i);

   }

//    tree.dump(cout);

   REQUIRE(tree.size() == 4095);
   REQUIRE(tree.height() == 11);  // N = 4096, lgN - 1




   //
   // check the values, stored properly?
   //
   for (int i = 1; i < 4096; ++i)
   {
       int value;

       REQUIRE(tree.search(i, value));
       REQUIRE(value == -i);
       REQUIRE(tree[i] == -i);
   }

   REQUIRE((tree % 4094) == 1);
   REQUIRE(tree(4094) == 4095);

   REQUIRE((tree % 4095) == 0);
   REQUIRE(tree(4095) == 0);

   //
   // walk the tree and make sure values in order, i.e. it was
   // rotated properly:
   //
   int key;
   int i = 1;

   tree.begin();
   while (tree.next(key))
   {
       REQUIRE(key == i);
       ++i;
   }

   REQUIRE(i == (tree.size() + 1));

   REQUIRE(tree[0] == 0);
   REQUIRE(tree(0) == 0);
   REQUIRE((tree % 0) == -1);

   REQUIRE(tree[4096] == 0);
   REQUIRE(tree(4096) == 0);
   REQUIRE((tree % 4096) == -1);



}



TEST_CASE("(3) case 2 at the root")
{
   avlt<int, int>  tree;

   vector<int> keys = { 50, 60, 30, 20, 40, 35 };
   vector<int> heights = { 1, 0, 1, 0, 2, 0 };

   for (int key : keys)
   {
       tree.insert(key, -key);
   }

//    tree.dump(cout);

   REQUIRE(tree.size() == keys.size());

   auto maxH = std::max_element(heights.begin(), heights.end());
   REQUIRE(tree.height() == *maxH);

   int value;

   for (int key : keys)
   {
       REQUIRE(tree.search(key, value));
       REQUIRE(value == -key);
   }

   for (size_t i = 0; i < keys.size(); ++i)
   {
       REQUIRE((tree % keys[i]) == heights[i]);
   }

}

TEST_CASE("(4) case 3 at the root")
{
   avlt<int, int>  tree;

   vector<int> keys = { 50, 30, 80, 90, 60, 70 };
   vector<int> heights = { 1, 0, 1, 0, 2, 0 };

   for (int key : keys)
   {
       tree.insert(key, -key);
   }

   REQUIRE(tree.size() == keys.size());

   auto maxH = std::max_element(heights.begin(), heights.end());
   REQUIRE(tree.height() == *maxH);

   int value;

   for (int key : keys)
   {
       REQUIRE(tree.search(key, value));
       REQUIRE(value == -key);
   }

   for (size_t i = 0; i < keys.size(); ++i)
   {
       REQUIRE((tree % keys[i]) == heights[i]);
   }
}

TEST_CASE("(5) case 4 at the root")
{
   avlt<int, int>  tree;

   vector<int> keys = { 60, 80, 100 };
   vector<int> heights = { 0, 1, 0 };

   for (int key : keys)
   {
       tree.insert(key, -key);
   }

   REQUIRE(tree.size() == keys.size());

   auto maxH = std::max_element(heights.begin(), heights.end());
   REQUIRE(tree.height() == *maxH);

   int value;

   for (int key : keys)
   {
       REQUIRE(tree.search(key, value));
       REQUIRE(value == -key);
   }

   for (size_t i = 0; i < keys.size(); ++i)
   {
       REQUIRE((tree % keys[i]) == heights[i]);
   }
}

TEST_CASE("(6) case 1 for sub-tree")
{
   avlt<int, int>  tree;

   vector<int> keys = { 50, 30, 60, 70, 20, 10};
   vector<int> heights = { 2, 0, 1, 0, 1, 0 };

   for (int key : keys)
   {
       tree.insert(key, -key);
   }

   REQUIRE(tree.size() == keys.size());

   auto maxH = std::max_element(heights.begin(), heights.end());
   REQUIRE(tree.height() == *maxH);

   int value;

   for (int key : keys)
   {
       REQUIRE(tree.search(key, value));
       REQUIRE(value == -key);
   }

   for (size_t i = 0; i < keys.size(); ++i)
   {
       REQUIRE((tree % keys[i]) == heights[i]);
   }
}

TEST_CASE("(7) case 2 for sub-tree")
{
   avlt<int, int>  tree;

   vector<int> keys = { 55, 48, 64, 38, 51, 78, 60, 16, 40, 45};
   vector<int> heights = { 3, 1, 1, 1, 0, 0, 0, 0, 2, 0 };

   for (int key : keys)
   {
       tree.insert(key, -key);
   }

   REQUIRE(tree.size() == keys.size());

   auto maxH = std::max_element(heights.begin(), heights.end());
   REQUIRE(tree.height() == *maxH);

   int value;

   for (int key : keys)
   {
       REQUIRE(tree.search(key, value));
       REQUIRE(value == -key);
   }

   for (size_t i = 0; i < keys.size(); ++i)
   {
       REQUIRE((tree % keys[i]) == heights[i]);
   }
}

TEST_CASE("(8) case 3 for sub-tree")
{
   avlt<int, int>  tree;

   vector<int> keys = { 50, 30, 70, 20, 40, 60, 100, 200, 90, 80};
   vector<int> heights = { 3, 1, 1, 0, 0, 0, 1, 0, 2, 0 };

   for (int key : keys)
   {
       tree.insert(key, -key);
   }

   REQUIRE(tree.size() == keys.size());

   auto maxH = std::max_element(heights.begin(), heights.end());
   REQUIRE(tree.height() == *maxH);

   int value;

   for (int key : keys)
   {
       REQUIRE(tree.search(key, value));
       REQUIRE(value == -key);
   }

   for (size_t i = 0; i < keys.size(); ++i)
   {
       REQUIRE((tree % keys[i]) == heights[i]);
   }
}

TEST_CASE("(9) case 4 for sub-tree")
{
   avlt<int, int>  tree;

   vector<int> keys = { 50, 30, 60, 70, 20, 80};
   vector<int> heights = { 2, 1, 0, 1, 0, 0 };

   for (int key : keys)
   {
       tree.insert(key, -key);
   }

   REQUIRE(tree.size() == keys.size());

   auto maxH = std::max_element(heights.begin(), heights.end());
   REQUIRE(tree.height() == *maxH);

   int value;

   for (int key : keys)
   {
       REQUIRE(tree.search(key, value));
       REQUIRE(value == -key);
   }

   for (size_t i = 0; i < keys.size(); ++i)
   {
       REQUIRE((tree % keys[i]) == heights[i]);
   }
}

// root cases
TEST_CASE("(0) empty tree")
{
   avlt<int, int>  tree;

   REQUIRE(tree.size() == 0);
   REQUIRE(tree.height() == -1);
}

TEST_CASE("(1) case1 at the root: left")
{
   avlt<int, int>  tree;

   vector<int> keys = { 1, 2, 3 };
   vector<int> heights = { 0, 1, 0 };

   for (int key : keys)  tree.insert(key, -key);

   // size and height?  after rebalance should be 1:
   REQUIRE(tree.size() == keys.size());

   auto maxH = std::max_element(heights.begin(), heights.end());
   REQUIRE(tree.height() == *maxH);

   // values inserted?
   int value;

   for (int key : keys)
   {
       REQUIRE(tree.search(key, value));
       REQUIRE(value == -key);
   }
   // heights correct after rebalance?
   for (size_t i = 0; i < keys.size(); ++i)
       REQUIRE((tree % keys[i]) == heights[i]);
//    tree.dump(std::cout);
}

TEST_CASE("(2) case2 at the root: Left Right")
{
   avlt<int, int>  tree;

   vector<int> keys = { 10, 8, 5, 3, 1, 6};
   vector<int> heights = { 0, 1, 2, 1 , 0, 0};

   for (int key : keys)  tree.insert(key, -key);
   // size and height?  after rebalance should be 1:
   REQUIRE(tree.size() == keys.size());

   auto maxH = std::max_element(heights.begin(), heights.end());
   REQUIRE(tree.height() == *maxH);

   // values inserted?
   int value;

   for (int key : keys)
   {
       REQUIRE(tree.search(key, value));
       REQUIRE(value == -key);
   }
//   // heights correct after rebalance?
   for (size_t i = 0; i < keys.size(); ++i)
       REQUIRE((tree % keys[i]) == heights[i]);
//    tree.dump(std::cout);
}

TEST_CASE("(3) case 3 at the root: Right Left")
{
   avlt<int, int>  tree;

   vector<int> keys =    { 1, 2, 3,20,30, 4};
   vector<int> heights = { 0, 1, 2, 1, 0, 0};

   for (int key : keys)  tree.insert(key, -key);
   // size and height?  after rebalance should be 1:
   REQUIRE(tree.size() == keys.size());

   auto maxH = std::max_element(heights.begin(), heights.end());
   REQUIRE(tree.height() == *maxH);

   // values inserted?
   int value;

   for (int key : keys)
   {
       REQUIRE(tree.search(key, value));
       REQUIRE(value == -key);
   }
   // heights correct after rebalance?
   for (size_t i = 0; i < keys.size(); ++i)
       REQUIRE((tree % keys[i]) == heights[i]);
//    tree.dump(std::cout);
}

TEST_CASE("(4) case 4 at the root: right")
{
   avlt<int, int>  tree;

   vector<int> keys = { 100, 80, 60 };
   vector<int> heights = { 0, 1, 0 };

   for (int key : keys)
   {
       tree.insert(key, -key);
   }

   // size and height?  after rebalance should be 1:
   //
   REQUIRE(tree.size() == keys.size());

   auto maxH = std::max_element(heights.begin(), heights.end());
   REQUIRE(tree.height() == *maxH);
   //
   // values inserted?
   //
   int value;

   for (int key : keys)
   {
       REQUIRE(tree.search(key, value));
       REQUIRE(value == -key);
   }

   //
   // heights correct after rebalance?
   //
   for (size_t i = 0; i < keys.size(); ++i)
       REQUIRE((tree % keys[i]) == heights[i]);
//    tree.dump(std::cout);
}

// Left node cases.
TEST_CASE("(5) case 1 on the Left: Left")
{
   avlt<int, int>  tree;

   vector<int> keys = {   50,60,40,70,55,45,30,25,10 };
   vector<int> heights = { 3, 1, 2, 0, 0, 0, 0, 1, 0 };

   for (int key : keys)  tree.insert(key, -key);

   // size and height?  after rebalance should be 1:
   REQUIRE(tree.size() == keys.size());

   auto maxH = std::max_element(heights.begin(), heights.end());
   REQUIRE(tree.height() == *maxH);

   // values inserted?
   int value;
   for (int key : keys)
   {
       REQUIRE(tree.search(key, value));
       REQUIRE(value == -key);
   }

   // heights correct after rebalance?
   for (size_t i = 0; i < keys.size(); ++i)
       REQUIRE((tree % keys[i]) == heights[i]);
//    tree.dump(std::cout);
}

TEST_CASE("(6) case 2 on the Left: Left Right")
{
   avlt<int, int>  tree;

   vector<int> keys = {   50,60,40,70,55,45,30,25,35,36 };
   vector<int> heights = { 3, 1, 1, 0, 0, 0, 1, 0, 2, 0 };

   for (int key : keys)  tree.insert(key, -key);

   // size and height?  after rebalance should be 1:
   REQUIRE(tree.size() == keys.size());

   auto maxH = std::max_element(heights.begin(), heights.end());
   REQUIRE(tree.height() == *maxH);

   // values inserted?
   int value;
   for (int key : keys)
   {
       REQUIRE(tree.search(key, value));
       REQUIRE(value == -key);
   }

   // heights correct after rebalance?
   for (size_t i = 0; i < keys.size(); ++i)
       REQUIRE((tree % keys[i]) == heights[i]);
//    tree.dump(std::cout);
}

TEST_CASE("(7) case 3 on the Left: Right Left")
{
   avlt<int, int>  tree;

   vector<int> keys = {   50,60,40,70,55,45,30,47,41,42};
   vector<int> heights = { 3, 1, 1, 0, 0, 1, 0, 0, 2, 0};

   for (int key : keys)  tree.insert(key, -key);

   // size and height?  after rebalance should be 1:
   REQUIRE(tree.size() == keys.size());

   auto maxH = std::max_element(heights.begin(), heights.end());
   REQUIRE(tree.height() == *maxH);

   // values inserted?
   int value;
   for (int key : keys)
   {
       REQUIRE(tree.search(key, value));
       REQUIRE(value == -key);
   }

   // heights correct after rebalance?
   for (size_t i = 0; i < keys.size(); ++i)
       REQUIRE((tree % keys[i]) == heights[i]);
//    tree.dump(std::cout);
}

TEST_CASE("(8) case 4 on the Left: Right")
{
   avlt<int, int>  tree;

   vector<int> keys = {   50,60,40,70,55,45,30,46,47};
   vector<int> heights = { 3, 1, 2, 0, 0, 0, 0, 1, 0 };

   for (int key : keys)  tree.insert(key, -key);

   // size and height?  after rebalance should be 1:
   REQUIRE(tree.size() == keys.size());

   auto maxH = std::max_element(heights.begin(), heights.end());
   REQUIRE(tree.height() == *maxH);

   // values inserted?
   int value;
   for (int key : keys)
   {
       REQUIRE(tree.search(key, value));
       REQUIRE(value == -key);
   }

   // heights correct after rebalance?
   for (size_t i = 0; i < keys.size(); ++i)
       REQUIRE((tree % keys[i]) == heights[i]);
//    tree.dump(std::cout);
}

// right node case.
TEST_CASE("(9) case 1 on the right: Left")
{
   avlt<int, int>  tree;

   vector<int> keys = {   50,60,40,70,55,45,30,54,53 };
   vector<int> heights = { 3, 2, 1, 0, 0, 0, 0, 1, 0};

   for (int key : keys)  tree.insert(key, -key);

   // size and height?  after rebalance should be 1:
   REQUIRE(tree.size() == keys.size());

   auto maxH = std::max_element(heights.begin(), heights.end());
   REQUIRE(tree.height() == *maxH);

   // values inserted?
   int value;
   for (int key : keys)
   {
       REQUIRE(tree.search(key, value));
       REQUIRE(value == -key);
   }

   // heights correct after rebalance?
   for (size_t i = 0; i < keys.size(); ++i)
       REQUIRE((tree % keys[i]) == heights[i]);
//    tree.dump(std::cout);
}

TEST_CASE("(10) case 2 on the Right: Left Right")
{
   avlt<int, int>  tree;

   vector<int> keys =    {50,60,40,70,55,45,30,53,57,58};
   vector<int> heights = { 3, 1, 1, 0, 1, 0, 0, 0, 2, 0};

   for (int key : keys)  tree.insert(key, -key);

   // size and height?  after rebalance should be 1:
   REQUIRE(tree.size() == keys.size());

   auto maxH = std::max_element(heights.begin(), heights.end());
   REQUIRE(tree.height() == *maxH);

   // values inserted?
   int value;
   for (int key : keys)
   {
       REQUIRE(tree.search(key, value));
       REQUIRE(value == -key);
   }

   // heights correct after rebalance?
   for (size_t i = 0; i < keys.size(); ++i)
       REQUIRE((tree % keys[i]) == heights[i]);
//    tree.dump(std::cout);
}

TEST_CASE("(11) case 3 on the right: right Left")
{
   avlt<int, int>  tree;
   vector<int> keys =    {50,60,40,70,55,45,30,90,65,63};
   vector<int> heights = { 3, 1, 1, 1, 0, 0, 0, 0, 2, 0};

   for (int key : keys)  tree.insert(key, -key);

   // size and height?  after rebalance should be 1:
   REQUIRE(tree.size() == keys.size());

   auto maxH = std::max_element(heights.begin(), heights.end());
   REQUIRE(tree.height() == *maxH);

   // values inserted?
   int value;
   for (int key : keys)
   {
       REQUIRE(tree.search(key, value));
       REQUIRE(value == -key);
   }

   // heights correct after rebalance?
   for (size_t i = 0; i < keys.size(); ++i)
       REQUIRE((tree % keys[i]) == heights[i]);
//    tree.dump(std::cout);
}

TEST_CASE("(12) case 4 on the right: right")
{
   avlt<int, int>  tree;

   vector<int> keys =    {50,60,40,70,55,45,30,75,80};
   vector<int> heights = { 3, 2, 1, 0, 0, 0, 0, 1, 0};

   for (int key : keys)  tree.insert(key, -key);

   // size and height?  after rebalance should be 1:
   REQUIRE(tree.size() == keys.size());

   auto maxH = std::max_element(heights.begin(), heights.end());
   REQUIRE(tree.height() == *maxH);

   // values inserted?
   int value;
   for (int key : keys)
   {
       REQUIRE(tree.search(key, value));
       REQUIRE(value == -key);
   }

   // heights correct after rebalance?
   for (size_t i = 0; i < keys.size(); ++i)
       REQUIRE((tree % keys[i]) == heights[i]);
//    tree.dump(std::cout);
}

TEST_CASE("(13) case 3 on the right: right Left")
{
   avlt<int, int>  tree;
   vector<int> keys =    {50,60,40,70,55,45,30,90,65,63};
   vector<int> heights = { 3, 1, 1, 1, 0, 0, 0, 0, 2, 0};

   for (int key : keys)  tree.insert(key, -key);

   // size and height?  after rebalance should be 1:
   REQUIRE(tree.size() == keys.size());

   auto maxH = std::max_element(heights.begin(), heights.end());
   REQUIRE(tree.height() == *maxH);

   // values inserted?
   int value;
   for (int key : keys)
   {
       REQUIRE(tree.search(key, value));
       REQUIRE(value == -key);
   }

   // heights correct after rebalance?
   for (size_t i = 0; i < keys.size(); ++i)
       REQUIRE((tree % keys[i]) == heights[i]);
//    tree.dump(std::cout);
}



TEST_CASE("(14) testing range_search")
{
   avlt<int, int>  tree;

   vector<int> keys =    {50,60,40,70,55,45,30,75,80};
   vector<int> heights = { 3, 2, 1, 0, 0, 0, 0, 1, 0};

   for (int key : keys)  tree.insert(key, -key);

   // size and height?  after rebalance should be 1:
   REQUIRE(tree.size() == keys.size());

   auto maxH = std::max_element(heights.begin(), heights.end());
   REQUIRE(tree.height() == *maxH);

   // values inserted?
   int value;
   for (int key : keys)
   {
       REQUIRE(tree.search(key, value));
       REQUIRE(value == -key);
   }

   // heights correct after rebalance?
   for (size_t i = 0; i < keys.size(); ++i)
       REQUIRE((tree % keys[i]) == heights[i]);


   vector<int> ans = tree.range_search(60,60);
//    for(size_t i; i < ans.size(); i++)
//        cout << ans[i] << ",";
//    cout << endl;

}
////
TEST_CASE("(0) Acopy contructor")
{
   avlt<int, int>  tree;

   vector<int> keys = { 50, 30, 60, 70, 20, 10};
   vector<int> heights = { 2, 0, 1, 0, 1, 0 };

   for (int key : keys)
   {
       tree.insert(key, -key);
   }

   avlt<int, int> tree2(tree);

   avlt<int, int> tree3;

   tree3 = tree2;
//
//    tree.dump(std::cout);
//    tree2.dump(std::cout);
//    tree3.dump(std::cout);
//
//    tree.clear();
//
//    tree.dump(std::cout);
//    tree2.dump(std::cout);
//    tree3.dump(std::cout);
//
//    tree2.clear();
//
//    tree3.dump(std::cout);
}



TEST_CASE("(1) Arigorous copy contructor")
{
   avlt<int, int>  tree;
   avlt<int,int> tree3;



   for (int i = 1; i <= 10; ++i)
   {
//        cout << " After insertng " << i << " " << endl;
       tree.insert(i, -i);

   }

   REQUIRE(tree.size() == 10);

//
//    for (int i = 1; i < 1000000; ++i)
//    {
//        int value;
//
//        REQUIRE(tree.search(i, value));
//        REQUIRE(value == -i);
//        REQUIRE(tree[i] == -i);
//    }

   avlt<int, int> tree2(tree);
   REQUIRE(tree.size() == tree2.size());
   REQUIRE(tree.height() == tree2.height());

   tree3 = tree2;
   tree.clear();

//    tree.dump(std::cout);
//    tree2.dump(std::cout);
//    tree3.dump(std::cout);

   REQUIRE(tree.size() == 0);
   REQUIRE(tree2.size() == 10);


   REQUIRE(tree2.size() == tree3.size());
   REQUIRE(tree2.height() == tree3.height());

//    tree.dump(std::cout);
//    tree2.dump(std::cout);
//    tree3.dump(std::cout);

   tree2.clear();

//    tree3.dump(std::cout);
}


TEST_CASE("(1) rigorous range")
{
    avlt<double, int>  tree;


    for (int i = 1; i <= 1000000; ++i)
    {
//        cout << " After insertng " << i << " " << endl;
        tree.insert(i, -i);

    }
//    vector<int> keys = { 50, 30, 60, 20, 40, 35};
//
//    for (int key : keys)
//    {
//        tree.insert(key, -key);
//    }

    REQUIRE(tree.size() == 1000000);

    for (int i = 1; i <= 1000000 - 2; ++i)
    {
        vector<double> results = tree.range_search(i, i + 2);
        REQUIRE(results.size() == 3);
        REQUIRE(results[0] == i);
        REQUIRE(results[1] == (i + 1));
        REQUIRE(results[2] == (i + 2));
    }



//    vector<double> ans = tree.range_search(19,50);
//
//    REQUIRE(ans.size() == );
}


TEST_CASE("(0) copy contructor")
{
   avlt<int, int>  tree;

   vector<int> keys = { 50};
   vector<int> heights = { 0 };

   for (int key : keys)
   {
       tree.insert(key, -key);
   }

   avlt<int, int> tree2(tree);

   REQUIRE(tree.height() == 0);
   REQUIRE(tree.height() == tree2.height());

   avlt<int, int> tree3;
   tree3 = tree2;

   REQUIRE(tree3.height() == tree.height());

}
