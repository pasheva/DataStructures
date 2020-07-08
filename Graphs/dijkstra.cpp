#include <vector>
#include <map>
#include <queue>
#include "graph.h"
#include "dijkstra.h"

using namespace std;
/**
 * Performs Dijkstra's shortest weighted path algorithm from
 * the given start vertex.  Returns a vector of vertices in
 * the order they were visited, along with a map of (long long , double)
 * pairs where the long long is a vertex V and the double is the
 * distance from the start vertex to V; if no such path exists,
 * the distance is INF (defined in dijkstra.h).
 */
vector<long long> Dijkstra(graph<long long, double>& G,
                            long long startV,
                            map<long long, double>& distances,
                            map<long long, long long>& predecessors)
{
    typedef pair<long long, double> pr;
    priority_queue<pr, vector<pr>, priority> unvisitedVerticies;
    vector<long long>  visited;
    vector<long long> verticiesInGraph = G.getVertices();
    set<long long> setOfVisited;
    set<long long> neighbours;

    for(long long currV : verticiesInGraph){
        distances[currV] = INF;
        unvisitedVerticies.push(make_pair(currV,INF));
        predecessors[currV] = -1;
    }

    // Start V has a distance of 0 from itself.
    // Also, it prevents overwritng it.
    distances[startV] = 0;
    unvisitedVerticies.push(make_pair(startV,0));

    while(!unvisitedVerticies.empty()){
        //Grabbing the front of the queue and removing it
        pair<long long, double> curr = unvisitedVerticies.top();
        auto currV = curr.first;
        auto currDist = curr.second;
        unvisitedVerticies.pop();

        //Making sure we are taking a valid vertex
        if(distances[currV] == INF){
            break;
        }else if(setOfVisited.count(currV) != 0){
            continue;
        }else{
            setOfVisited.insert(currV);
            visited.push_back(currV);
        }
        //Grabbing the neighbours of the curr vertex.
        neighbours = G.neighbors(currV);
        for(auto adjV : neighbours){
            double edgeW = 0;
            G.getWeight(currV, adjV, edgeW);
            double alterDist =  currDist + edgeW;
            double adjVerDist = distances.find(adjV)->second;
            if(alterDist < adjVerDist){
                distances[adjV] = alterDist;
                unvisitedVerticies.push(make_pair(adjV,alterDist));
                predecessors[adjV] = currV;
            }
        }
    }
    return visited;
}