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


TEST_CASE("(1) empty tree")
{
  avlt<int, int>  tree;

  REQUIRE(tree.size() == 0);
  REQUIRE(tree.height() == -1);
}


TEST_CASE("(2) case 1 at the root")
{
  avlt<int, int>  tree;

  vector<int> keys = { 100, 80, 60 };
  vector<int> heights = { 0, 1, 0 };

  for (int key : keys)
  {
    tree.insert(key, -key);
  }

  //
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
  {
    REQUIRE((tree % keys[i]) == heights[i]);
  }
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
























