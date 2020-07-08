#pragma once

#include <iostream>
#include <stdexcept>
#include <vector>
#include <set>
#include <map>
#include <utility>

using namespace std;


template<typename VertexT, typename WeightT>
class graph
{
private:
    typedef map<VertexT,WeightT> adjData;
    /*  Implementation:
     *      Nested Map
     *  Representation:
     *   [A] => [B|115]
     *   [B] => [D|1]
     *   [D] => [H|37] [F|27]
     */
    map<VertexT, adjData> adjList;



public:
  /**
  * constructor:
  *
  *Constructor has nothing to do.
  */
  graph()
  {
      ;
  }

  /**
  * NumVertices
  *
  * Returns the # of vertices currently in the graph.
  */
  int NumVertices() const
  {
    return static_cast<int>(adjList.size());
  }

  /**
  * NumEdges
  *
  * Returns the # of edges currently in the graph.
  */
  int NumEdges() const
  {
    int count = 0;

    for(auto itr  = adjList.begin(); itr!=adjList.end(); ++itr){
        count+= itr->second.size();
    }

    return count;
  }

  /**
   * addVertex
   * Adding a vertex to the map if it does not exist already.
   *
   * @param v the vertex to be added
   */
  bool addVertex(VertexT v)
  {

    //Check whether it has been inserted already.
    if(adjList.count(v) > 0){
        return false;
    }

    // Insert with a new vertex with default map value;
   adjList[v] = adjData ();
    return true;
  }

  /**
   * addEdge
   *
   * Addint a new edge as the value of the vertex which is a map structure.
   * If the edge already exist , we are just updating the height.
   *
   * @param from Start vertex
   * @param to Destination vertex
   * @param the weight to be added for that edge
   */
  bool addEdge(VertexT from, VertexT to, WeightT weight)
  {

    if(adjList.count(from)>0 && adjList.count(to)>0){
        auto itr = adjList.find(from);
        if(itr!=adjList.end()){
            auto itr2 = itr->second.find(to);
            if(itr2!=itr->second.end()){
                //Update the weight
                itr2->second = weight;
            }else{
                //Insert a new pair.
                itr->second.insert(make_pair(to, weight));
            }
            return true;
        }
    }
    return false;
  }

  /**
   * getWeight
   *
   * Searching both map for the from veretx and to vertex.
   *
   * @param from Start vertex
   * @param to Destination vertex
   * @param weight passed be reference would return the weight it exists.
   *
   * @return True if the weight has been found and false if not.
   *
   */
  bool getWeight(VertexT from, VertexT to, WeightT& weight) const
  {
      auto itr = adjList.find(from);
      if(itr!=adjList.end()){
          auto itr2 = itr->second.find(to);
          if(itr2!=itr->second.end()){
              //The edge exists and has been found.
              weight = itr2->second;
              return true;
          }
      }
    return false;
  }

  /**
   * neighbors
   *
   *  Initially we a looking for the vertex and after that
   *  linearlly going through each value of the map (adjList < vertex, map(value))
   *  and inserting all the vetexes to a set.
   *
   *  @param the vertex for which we trying to find the neighbours
   *
   *  @return a set of all the neighbours
   */
  set<VertexT> neighbors(VertexT v) const
  {
    set<VertexT>  S;

    auto itr = adjList.find(v);
    if(itr!=adjList.end()) {
        for(auto itr2 = itr->second.begin(); itr2!=itr->second.end(); ++itr2){
                S.insert(itr2->first);
        }
    }
    return S;
  }

  /**
   * getVertices
   *
   * Iterattign over the amp to get all the vertecies.
   *
   * @return vector with all the vertecies in the graph
   */
  vector<VertexT> getVertices() const
  {
    vector<VertexT> allV;  // returns a copy:
    for(auto v : adjList){
        allV.push_back(v.first);
    }
    return allV;
  }

  /**
   * dump
   *
   * Dumps the internal state of the graph for debugging purposes.
   *
   * Example:
   *     graph<string,int>  G(26);
   *    ...
   *    G.dump(cout);  // dump to console
   */
  void dump(ostream& output) const
  {
    output << "***************************************************" << endl;
    output << "********************* GRAPH ***********************" << endl;

    output << "**Num vertices: " << this->NumVertices() << endl;
    output << "**Num edges: " << this->NumEdges() << endl;

    output << endl;
    output << "**Vertices:" << endl;
    int i = 0;
      for(auto v : adjList){
      output << " " << i << ". " <<  v.first << endl;
      i++;
    }

    output << endl;
    output << "**Edges:" << endl;

        for(auto itr  = adjList.begin(); itr!=adjList.end(); ++itr){
            output << " row " << itr->first << ": ";
            for(auto itr2 = itr->second.begin(); itr2!=itr->second.end(); ++itr2){
                output << "(" << itr->first << "," << itr2->first << "," << itr2->second << ") ";
            }
            output << endl;
        }

    output << "**************************************************" << endl;
  }

};