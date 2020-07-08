/*main.cpp*/

// References:
// TinyXML: https://github.com/leethomason/tinyxml2
// OpenStreetMap: https://www.openstreetmap.org
// OpenStreetMap docs:
//   https://wiki.openstreetmap.org/wiki/Main_Page
//   https://wiki.openstreetmap.org/wiki/Map_Features
//   https://wiki.openstreetmap.org/wiki/Node
//   https://wiki.openstreetmap.org/wiki/Way
//   https://wiki.openstreetmap.org/wiki/Relation
//

#include <iostream>
#include <iomanip>  /*setprecision*/
#include <string>
#include <vector>
#include <map>
#include <cassert>
#include <cfloat>
#include "tinyxml2.h"
#include "dist.h"
#include "osm.h"
#include "graph.h"
#include "dijkstra.h"

using namespace std;
using namespace tinyxml2;


/**
 *        >>> TASKS <<<
 *   5.Add nodes as vertices.  [DONE]
 *   6.Add edges based on footways. [DONE]
 *   7.Input start and destination buildings, locate on map. [DONE]
 *   8.Search the footways and find the nearest nodes to the start and destination buildings [DONE]
 *      these become the “start” and “dest” nodes.
 *   9.Run Dijkstra’s algorithm from the start node. [DONE]
 *   10.Output the distance and path from start nodeto destination node. [DONE]
 *      If no path exists, output “Sorry, destination unreachable”. [DONE]
 *   11.Repeatwith another pair of buildings. [DONE]
 */

//Signitures in order
void dijkstraAlgo(graph<long long, double>&G, long long startNode, long long finalNode);
long long findNearestNode(BuildingInfo& building,
                          vector<FootwayInfo>& Footways,
                          map<long long, Coordinates>&  Nodes,
                          Coordinates& nearestNode);
bool findBuilding(string& buildingName, const vector<BuildingInfo>& Buildings, BuildingInfo& building);
void addNodes(graph<long long, double>&G,  map<long long, Coordinates>&  Nodes);
void addEges(graph<long long, double>&G,  map<long long, Coordinates>&  Nodes,  vector<FootwayInfo>& Footways);
int main();




/**
 *
 *   The function applied the Dijkster algorithm and after that it checks
 *   if the destination (final) Node is not INF (max possible value for a double)
 *   therefore we have a distance available and path is available.We then get the
 *   path from the predecessors, however, I am storing it in a vector since it
 *   added in a reverse order.
 *
 *   @param G The graph with nodes and distances
 *   @param starNode is the node we trace from
 *   @param destNode is the node we trace to
 *
 */
void dijkstraAlgo(graph<long long, double>&G, long long startNode, long long destNode){

    cout << "Navigating with Dijkstra..." << endl;

    map<long long, long long> predecessors;
    map<long long, double> distances;
    vector<long long> path = Dijkstra(G, startNode, distances, predecessors);

    if (distances[destNode] != INF){
        cout << "Distance to dest: " << distances[destNode] << " miles" << endl;
        vector <long long> finalPath;
        /*
         *  Output Example:
         *  464748192->5632839978->5632839988->5632839977->
         *  463814003->464345369->463814052->464748194->
         *  462010750->5632839974->5632839973
         */
        while(destNode != startNode){
            finalPath.push_back(destNode);
            destNode = predecessors[destNode];
        }
        finalPath.push_back(destNode);

        cout << "Path: " ;
        int i = finalPath.size()-1;
        cout << finalPath[i];
        i--;
        for(; i >= 0; i--){
            cout << "->";
            cout << finalPath[i];
        }
    }
    else  cout << "Sorry, destination unreachable" << endl;
}


/**
 *
 *  We are fiding the nearest node based on a building by
 *  getting the longitude and latitude, calculating the distance
 *  and using the "minimum" algorithm.
 *
 *  @param building the building struct which has the fullname,abbrv,id,lat,lon
 *  @param Footways TheFootway is ID mapped to other IDs
 *  @param Nodes The ID mapped to the Coordinates (id,lat,lon)
 *  @param nearestNode The struct with Coordinates which consists of id,lat,lon
 *
 *  @return the ID of the nearest node.
 */
long long findNearestNode(BuildingInfo& building,
                           vector<FootwayInfo>& Footways,
                           map<long long, Coordinates>&  Nodes,
                           Coordinates& nearestNode
                           ){
    double Lat1 = building.Coords.Lat;
    double Lon1 = building.Coords.Lon;
    long long nearestNodeID = 0;
    double minDist = DBL_MAX;

    for(auto & Way: Footways){
        int vecSize = Way.Nodes.size();
        for( int i = 0; i < vecSize; ++i){
            long long ID = Way.Nodes.at(i);
            auto itr = Nodes.find(ID);
            double Lat2 = itr->second.Lat;
            double Lon2 = itr->second.Lon;
            double dist = distBetween2Points(Lat1,Lon1,Lat2,Lon2);
            if (dist < minDist){
                minDist = dist;
                nearestNodeID = ID;
                nearestNode.Lon = Lon2;
                nearestNode.Lat = Lat2;
            }
        }
    }
    return nearestNodeID;
}

/**
 *
 * Finding the buildings by:
 *  1. Abbreviation
 *  2. Name (partial names are accepted)
 *
 *
 * @return True if the name or abbrev has been found, otherwise false;
 */
bool findBuilding(string& buildingName, const vector<BuildingInfo>& Buildings, BuildingInfo& building){
    for(auto & Building : Buildings){
        building = Building;
        if(buildingName == Building.Abbrev){
            return true;
        }else if(Building.Fullname.find(buildingName) != std::string::npos) {
            return true;
        }else if( buildingName == ""){ //Accounting for empty imput.
            return false;
        }
    }
    return false;
}

/**
 *
 *  Adding all the nodes available on the map to our graph.
 */
void addNodes(graph<long long, double>&G,  map<long long, Coordinates>&  Nodes){
    for(auto & Node : Nodes){
        G.addVertex(Node.first);
    }
}

/**
 *  Adding the adges which is the distance between nodes calculated based on longitude and latitude.
 *  The direction between nodes is both ways.
 *
 *  N1<-->N2 N2<--->N3 N3<--->N4  N4<--->N5 ...
 */
void addEges(graph<long long, double>&G,  map<long long, Coordinates>&  Nodes,  vector<FootwayInfo>& Footways){
    for(auto & Way: Footways){
        int vecSize = Way.Nodes.size();
        for( int i = 0; i < vecSize; ++i){
            //Check boundry for the next node.
            if( (i+1) >= vecSize){
                break;
            }

            long long fromID = Way.Nodes.at(i);
            auto itr1 = Nodes.find(fromID);
            double Lat1 = itr1->second.Lat;
            double Lon1 = itr1->second.Lon;
            long long toID = Way.Nodes.at(i+1);
            auto itr2 = Nodes.find(toID);
            double Lat2 = itr2->second.Lat;
            double Lon2 = itr2->second.Lon;
            double dist = distBetween2Points(Lat1,Lon1,Lat2,Lon2);
            //Add path one way
            G.addEdge(fromID, toID, dist);
            // Add path the other way
            G.addEdge(toID, fromID, dist);
        }
    }
}


/**
 *  Main menu programm.
 */
int main()
{
    /*
     *  Coordinates:
     *      long long ID
     *      double lat
     *      double lon
     */
    map<long long, Coordinates>  Nodes;     // maps a Node ID to it's coordinates (lat, lon)
    /*
     *  FootwayInfo:
     *      long long ID
     *      vector<long long> Nodes   // Nodes >= 2;
     */
    vector<FootwayInfo>          Footways;  // info about each footway, in no particular order
    /*
     *  BuildingInfo:
     *      string Fullname
     *      string Abbrev
     *      Coordinates Coords (id, lat, lon)
     */
    vector<BuildingInfo>         Buildings; // info about each building, in no particular order
    XMLDocument                  xmldoc;
    graph<long long, double> G; // vetercies are nodes, weights are distances

    cout << "** Navigating UIC open street map **" << endl;
    cout << endl;
    cout << std::setprecision(8);

    string def_filename = "map.osm";
    string filename;

    cout << "Enter map filename> ";
    getline(cin, filename);

    if (filename == "")
    {
        filename = def_filename;
    }

    // Load XML-based map file
    if (!LoadOpenStreetMap(filename, xmldoc))
    {
        cout << "**Error: unable to load open street map." << endl;
        cout << endl;
        return 0;
    }

    // Read the nodes, which are the various known positions on the map:
    int nodeCount = ReadMapNodes(xmldoc, Nodes);
    // Read the footways, which are the walking paths:
    int footwayCount = ReadFootways(xmldoc, Footways);
    // Read the university buildings:
    int buildingCount = ReadUniversityBuildings(xmldoc, Nodes, Buildings);
    //  Add Nodes
    addNodes(G,Nodes);
    // Add Edges
    addEges(G,Nodes,Footways);

    // Statistics
    assert(nodeCount == Nodes.size());
    assert(footwayCount == Footways.size());
    assert(buildingCount == Buildings.size());

    cout << endl;
    cout << "# of nodes: " << Nodes.size() << endl;
    cout << "# of footways: " << Footways.size() << endl;
    cout << "# of buildings: " << Buildings.size() << endl;
    cout << "# of vertices: " << G.NumVertices() << endl;
    cout << "# of edges: " << G.NumEdges() << endl;
    cout << endl;


    // Navigation from building to building
    string startBuilding, destBuilding;
    BuildingInfo startBuildingData, destBuildingData;
    Coordinates nearestNodeCoor;

    cout << "Enter start (partial name or abbreviation), or #> ";
    getline(cin, startBuilding);

    //Main Menu
    while (startBuilding != "#")
    {
        //Getting the restionation.
        cout << "Enter destination (partial name or abbreviation)> ";
        getline(cin, destBuilding);


        // Find the building if, otherwise, output error message and quit.
        if (!findBuilding(startBuilding, Buildings, startBuildingData)){
            cout << "Start building not found" << endl;
            cout << "\nEnter start (partial name or abbreviation), or #> ";
            getline(cin, startBuilding);
            continue;
        }
        // Find the dest building, otherwise, output error message and quit.
        if (!findBuilding(destBuilding, Buildings, destBuildingData)){
            cout << "Destination building not found" << endl;
            cout << "\nEnter start (partial name or abbreviation), or #> ";
            getline(cin, startBuilding);
            continue;
        }

        cout << "Starting point:\n "
             << startBuildingData.Fullname
             << "\n ("
             << startBuildingData.Coords.Lat << ", "
             << startBuildingData.Coords.Lon << ")" << endl;

        cout << "Destination point:\n "
             << destBuildingData.Fullname
             << "\n ("
             << destBuildingData.Coords.Lat << ", "
             << destBuildingData.Coords.Lon << ")" << endl;

        // Closest Node to the start destination.
        long long nearestStartNodeID = findNearestNode(startBuildingData, Footways, Nodes, nearestNodeCoor);
        cout << endl;
        cout << "Nearest start node:\n "
             << nearestStartNodeID
             << "\n ("
             << nearestNodeCoor.Lat << ", "
             << nearestNodeCoor.Lon << ")" << endl;
        // Closest Node to the final destination.
        long long nearestDestNodeID = findNearestNode(destBuildingData, Footways, Nodes, nearestNodeCoor);
        cout << "Nearest destination node:\n "
             << nearestDestNodeID
             << "\n ("
             << nearestNodeCoor.Lat << ", "
             << nearestNodeCoor.Lon << ")" << endl;

        cout << endl;
        dijkstraAlgo(G,nearestStartNodeID, nearestDestNodeID);
        cout << endl;
        cout << endl;

        cout << "Enter start (partial name or abbreviation), or #> ";
        getline(cin, startBuilding);
    }

    // Quitting the program.
    cout << "** Done **" << endl;

    return 0;
}
