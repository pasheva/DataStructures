#include<iostream>
#include<typeinfo>
#include<string>

using namespace std;

enum class Test
{
    ALL,
    INNER,
    OUTER
};

int main()
{
    Test zeroPos = Test::ALL;
    Test firstPos = Test::INNER;

    firstPos = Test::ALL;
    if( Test::INNER == firstPos)
    {
        cout << "1";
    }

    return 0;
}

