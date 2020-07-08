#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <limits>
#include "graph.h"

using namespace std;


class priority{
public:

    typedef const pair<long long, double>& pr;
    bool operator()(pr var1, pr var2){
        if(var1.second == var2.second){
            return var1.first > var2.first;
        }
        return var1.second > var2.second;
    }

};

const double INF = numeric_limits<double>::max();

vector<long long> Dijkstra(graph<long long, double>& G,
                            long long startV,
                            map<long long, double>& distances,
                            map<long long, long long>& predecessors
                            );