#include <iostream>
#include <fstream>
#include <string>
#include <vector> //set abbstraction
#include <map> //set abbstraction
#include <regex>
#include <utility>
#include "util.h"
#include "hash.h"
#include "hashmap.h"

using namespace std;


typedef struct Station
{
    int id; //456
    string abbrev; //2112 W Peterson
    string fullname; //2112 W Peterson Ave
    vector<double>latlong; //[41.991178, -87.683593] [latitude,longitutde]
    int capacity; //15
    string onlineDate; //5/12/2015

    Station(){
        id = {};
        abbrev = "";
        fullname = "";
        latlong.reserve(2);
        capacity = {};
        onlineDate = "";
    }
}Station;


typedef struct Abbreviation{
    string abbrev;
    string id;
    Abbreviation(){
        abbrev = "";
        id = "";
    }
}Abbr;

typedef struct Trip
{
    string tripid; //Tr10426561 , always starts with Tr
    string startTime; //[6/30/2016, 21:30] [date, time]
    string stopTime; //[6/30/2016, 21:50] [date, time]
    string bikeId; // B3307 , always starts with B
    int duration; //870 seconds
    vector<string> fromTo; //[289,16] [from,to]
    string identifies; // Female
    string birthyear;

    Trip(){
        tripid = "";
        startTime = "";
        stopTime = "";
        bikeId = "";
        duration = {};
        fromTo.reserve(2);
        identifies = "";
        birthyear = "";
    }
}Trip;

typedef struct Bike{
    string id;
    int numOfUses;
    Bike(){
        id = "";
        numOfUses = 0;
    }
}Bike;


/**
 *string_To_Int
 *
 * Checks if the given string hasnumbers;
 *  A string is numeric if it contains 1 or more digits, so let's
 *  use a regular expression to check that.
 *
 *   we are using POSIX basic notation for regular expressions, see
 *   https://en.cppreference.com/w/cpp/regex/ecmascript
 *   where [:d:] means digit, [[:d:]] means match a single digit.
 *   The + means match the preceding sub-expression 1 or more times.
 *
 */
bool isNumeric(string s)
{

    regex pattern("[[:d:]]+", regex::ECMAScript);

    smatch matchResults;  // provides more details on the match

    if (regex_match(s, matchResults, pattern))
        return true;
    else
        return false;

}


/**
 *string_To_Int
 *
 * Converts seconds to minutes and hours
 */
void S2SSMMHH(int& seconds, int& minutes, int& hours){
    minutes = (seconds / 60)%60;
    hours = (minutes/60);
    seconds = seconds%60;
}

/**
 *string_To_Int
 *
 * Converts a string to an integer, unless string is empty, in
 * which case 0 is returned.
 *
 * @param s the string to be converted to int
 */
int string_To_Int(const string& s)
{
    if (s.empty())
        return 0;
    else
        return stoi(s);
}

/**
 *string_To_Double
 *
 * Converts a string to a double, unless string is empty, in
 * which case 0 is returned.
 *
 * @param s the string to be converted to double
 */
double string_To_Double(const string& s)
{
    if (s.empty())
        return 0;
    else
        return stod(s);
}

/**
 * inputDataStations
 *
 * Given a filename readin through the station file.
 * Format:
 * id,abbrev,fullname,latitude,longitude,capacity,online_date
 * 456,2112 W Peterson,2112 W Peterson Ave,41.991178,-87.683593,15,5/12/2015
 *
 * @param filename The filed to be read.
 *
 */
bool inputDataStations(const string& filename, hashmap<int,Station>& stationIdTable, hashmap<string,Abbr>& stationAbbTable){
    ifstream  infile(filename);

    if (!infile.good()) {
        cout << "**Error: unable to open '" << filename << "'..." << endl;
        return false;
    }

    // file is open, start processing:
    string line;

    getline(infile, line);  // input and discard first row --- header row

    int stationsCount = 0;

    while (getline(infile, line))
    {
        stringstream s(line);

        string id,abbrev,fullname,latitude,longitude,capacity,online_date;

        //
        // format: date,xx xx xx xx xx,yy,z
        //
        // NOTE: z may be missing (the multiplier)
        //
        getline(s, id, ',');  // first value => region
        getline(s, abbrev, ',');   // second value => country
        getline(s, fullname, ',');
        getline(s, latitude, ',');
        getline(s, longitude, ',');
        getline(s, capacity, ',');
        getline(s, online_date, ',');

        // we have input the 4 values, and since we are not
        // doing any arithmetic, we can just store them as
        // strings.  If the multiplier is missing then the
        // string is "", which is fine.

        //
        // store into hash table
        //
        Station station;
        Abbr abbrv;

        station.id = string_To_Int(id);
        station.abbrev = abbrev;
        station.fullname = fullname;
        station.latlong.push_back(stod(latitude));
        station.latlong.push_back(stod(longitude));
        station.capacity = string_To_Int(capacity);
        station.onlineDate = online_date;

        abbrv.abbrev = abbrev;
        abbrv.id = id;

        stationIdTable.insert(station.id,station, HashByStationId); //<- change the hash
        stationAbbTable.insert(station.abbrev,abbrv,HashByStationAbbrv);
        stationsCount++;
    }
    cout << "# of stations: " << stationsCount << endl;
    return true;  // we have data to be processed:
}

/**
 * inputDataTrips
 *
 * Given a filename readin through the station file.
 * Format:
 * tripid,starttime,stoptime,bikeid,duration,from,to,identifies,birthyear
 * Tr10426561,6/30/2016 23:35,7/1/2016 0:02,B5229,1620,329,307,Male,1968
 *
 * @param filename The filed to be read.
 *
 */
bool inputDataTrips(const string& filename, hashmap<string,Trip>& tripTable, hashmap<string,Bike>& bikeTable){
    ifstream  infile(filename);

    if (!infile.good()) {
        cout << "**Error: unable to open '" << filename << "'..." << endl;
        return false;
    }

    // file is open, start processing:
    string line;

    getline(infile, line);  // input and discard first row --- header row

    int tripsCount = 0;
    int bikesCount = 0; //only unique entries

    while (getline(infile, line))
    {
        stringstream s(line);

        string tripid, starttime, stoptime,bikeid,duration,from,to,identifies,birthyear;

        getline(s, tripid, ',');
        getline(s, starttime, ',');
        getline(s, stoptime, ',');
        getline(s, bikeid, ',');
        getline(s, duration, ',');
        getline(s, from, ',');
        getline(s, to, ',');
        getline(s, identifies, ',');
        getline(s, birthyear, ',');
        // we have input the 4 values, and since we are not
        // doing any arithmetic, we can just store them as
        // strings.  If the multiplier is missing then the
        // string is "", which is fine.

        //Strct instances;
        Trip trip;
        Bike bike;

        trip.tripid = tripid;
        trip.startTime = starttime;
        trip.stopTime = stoptime;
        trip.bikeId = bikeid;
        trip.duration = string_To_Int(duration);
        trip.fromTo.push_back(from);
        trip.fromTo.push_back(to);
        trip.identifies = identifies;
        trip.birthyear = birthyear;

        bike.id = bikeid;

        tripTable.insert(tripid,trip,HashByTripId);

        if(bikeTable.search(bikeid,bike,HashByBikeId)){
            bike.numOfUses += 1;
            bikeTable.insert(bikeid,bike,HashByBikeId);
        }else{
            Bike newBikeEntry;
            newBikeEntry.id = bikeid;
            newBikeEntry.numOfUses = 1;
            bikeTable.insert(bikeid,newBikeEntry,HashByBikeId);
            bikesCount++;
        }
        tripsCount++;
    }
    cout << "# of trips: " << tripsCount << endl;
    cout << "# of bikes: " << bikesCount << endl;
    return true;  // we have data to be processed:
}



/**
 * getHelp
 *
 * Outputting the help menu for the user.
 *
 */
void getHelp(){
    cout << "\nAvailable Commands:"
         << "\nEnter a station id (e.g. 341)"
         << "\nEnter a station abbrevation (e.g. Adler)"
         << "\nEnter a trip id (e.g. Tr10426561)"
         << "\nEnter a bike id (e.g. B5218)"
         << "\nNearby stations (e.g. nearby 41.86 -87.62 0.5)"
         << "\nSimilar trips (e.g. similar Tr10424639 0.3)" << endl;
}


/**
 * lookupStationById
 *
 * Using the hashmap build in function to search the table.
 *
 */
void lookupStationById(const string& userInput,hashmap<int,Station>& stationIdTable) {
    Station data = {};
    int key = string_To_Int(userInput);
    if(stationIdTable.search(key, data, HashByStationId)) {
        cout << "Station:"
             << "\n ID: " << data.id
             << "\n Abbrev: " << data.abbrev
             << "\n Fullname: " << data.fullname
             << "\n Location: (" << data.latlong[0] << ", " << data.latlong[1] << ")"
             << "\n Capacity: " << data.capacity
             << "\n Online date: " << data.onlineDate << endl;
    }else{
        cout << "Station not found" << endl;
    }
}

/**
 * lookupStationByAbbrv
 *
 * Using the hashmap build in function to search the table.
 *
 */
void lookupStationByAbbrv(const string& userInput,hashmap<int,Station>& stationIdTable, hashmap<string,Abbr>& stationAbbTable){
    Abbr data;
    const string& key = userInput;
    if(stationAbbTable.search(key, data,HashByStationAbbrv)){
        lookupStationById(data.id, stationIdTable);
    }else{
        cout << "Station not found" << endl;
    }
}


/**
 * lookupTrip
 *
 * Using the hashmap build in function to search the table.
 *
 */
void lookupTrip(const string& userInput, hashmap<string,Trip>& tripTable,hashmap<int,Station>& stationIdTable){
    Trip data;
    Station stationData;
    const string& key = userInput;
    if(tripTable.search(key, data, HashByTripId)){
        int s = data.duration;
        int h = 0;
        int m = 0;
        S2SSMMHH(s,m,h);
        cout << "Trip:"
             << "\n ID: " << data.tripid
             << "\n Starttime: " << data.startTime
             << "\n Bikeid: " << data.bikeId
             << "\n Duration: ";
        if ( h > 0){
            cout << h << " hours, ";
        }
        if(m > 0){
            cout << m << " minutes, ";
        }
        if(s >= 0){
            cout << s << " seconds";
        }
        stationIdTable.search(string_To_Int(data.fromTo[0]), stationData, HashByStationId);
        cout << "\n From station: " << stationData.abbrev << " (" << data.fromTo[0] << ")";
        stationIdTable.search(string_To_Int(data.fromTo[1]), stationData, HashByStationId);
        cout << "\n To station: " << stationData.abbrev << " (" << data.fromTo[1] << ")"
             << "\n Rider identifies as: " << data.identifies
             << "\n Birthyear: " << data.birthyear << endl;
    }
    else{
        cout << "Trip not found" << endl;
    }
}

/**
 * lookupBikeUsage
 *
 * Using the hashmap build in function to search the table.
 *
 */
void lookupBikeUsage(const string& userInput, hashmap<string,Bike>& bikeTable){
    Bike data;
    const string& key = userInput;
    if(bikeTable.search(key, data, HashByBikeId)){
        cout << "Bike:"
             << "\n ID: " << data.id
             << "\n Usage: " << data.numOfUses << endl;
    }
    else cout << "Bike not found" << endl;
}

/**
* llookupNearbyStations(
*
*   Mapping each station in the range to ist distance.
 *  The issue only comes for distances which are the same.
 *  Could not find a solution.
*
*/
void lookupNearbyStations(const string& userInput,hashmap<int,Station>& stationIdTable){

    map<double,Station> mappedStations;
    Station station;

    stringstream s(userInput);

    string option, latitude, longitude, range;

    getline(s, option, ' ');
    getline(s, latitude, ' ');
    getline(s, longitude, ' ');
    getline(s, range, ' ');

    //Looping through to mapp the values.
    for(int i = 0; i < stationIdTable.getN(); ++i) {
        if (stationIdTable.hasData(i)) {
            station = stationIdTable.getEntryValue(i);
            double dist = distBetween2Points(string_To_Double(latitude), string_To_Double(longitude),
                                             station.latlong[0], station.latlong[1]);
            if (dist <= string_To_Double(range)) {
                mappedStations.insert(pair<double, Station>(dist, station));
            }
        }
    }
    //Irerating through the map;
    bool check = true;
    map<double, Station>::iterator pointer;
    cout << "Stations within " << range << " miles of (" << latitude << ", " << longitude << ")" << endl;
    for (pointer = mappedStations.begin(); pointer != mappedStations.end(); ++pointer) {
        cout << " station " << pointer->second.id << ": " << pointer->first << " miles" << endl;
        check = false;
    }
    if (check) {
        cout << " none found" << endl;
    }
}



/**
* lookupSimilarTrips
*
*   Mapping each station in the range to ist distance.
 *  Since we have access to the trips we have to mark both the from and to stations
 *  the same as th previos function but extended.
*
*/
void lookupSimilarTrips(const string& userInput,hashmap<int,Station>& stationIdTable, hashmap<string, Trip>& tripsByIdTable){
    stringstream s(userInput);

    string similar, key, range;

    getline(s, similar, ' ');
    getline(s, key, ' ');
    getline(s, range, ' ');

    cout << "Trips that follow a similar path (+/-" << range << " miles) as " << key << endl;

    Trip trip;
    Station station;
    double tolat = 0;
    double tolong = 0;
    double fromlat = 0;
    double fromlong = 0;
    map<int, double> mappedBeginStations;
    map<int, double> mappedEndStations;
    map<int, double>::iterator pointer; //In order to go though the map.
    if(tripsByIdTable.search(key, trip, HashByTripId)){

        int from = string_To_Int(trip.fromTo[0]);
        int to = string_To_Int(trip.fromTo[1]);
        if(stationIdTable.search(from, station, HashByStationId)){
            fromlat = station.latlong[0];
            fromlong = station.latlong[1];
        }
        if(stationIdTable.search(to, station, HashByStationId)){
            tolat = station.latlong[0];
            tolong = station.latlong[1];
        }
        //Looping through to mapp the values. Hadnling both to and from.
        for(int i = 0; i < stationIdTable.getN(); ++i) {
            if (stationIdTable.hasData(i)) {
                station = stationIdTable.getEntryValue(i);
                double dist1 = distBetween2Points(fromlat, fromlong,station.latlong[0], station.latlong[1]);
                double dist2 = distBetween2Points(tolat, tolong, station.latlong[0], station.latlong[1]);
                if (dist1 <= string_To_Double(range)) {
                    mappedBeginStations.insert(pair<int, double>(station.id,dist1));
                }
                if (dist2 <= string_To_Double(range)) {
                    mappedEndStations.insert(pair<int, double>(station.id,dist2));
                }
            }
        }

        cout << "\n nearby starting points: ";
        for (pointer =  mappedBeginStations.begin(); pointer != mappedBeginStations.end(); ++pointer) {
            cout << pointer->first << " ";
        }
        cout << "\n nearby ending points: ";
        for (pointer= mappedEndStations.begin(); pointer != mappedEndStations.end(); ++pointer){
            cout << pointer->first << " ";
        }
        int count = 0;
        for(int i = 0; i < tripsByIdTable.getN(); ++i){
            if(tripsByIdTable.hasData(i)){
                trip = tripsByIdTable.getEntryValue(i);
                int f = string_To_Int(trip.fromTo[0]);
                int t  = string_To_Int(trip.fromTo[1]);
                if(mappedBeginStations.find(f) != mappedBeginStations.end()) {
                    if (mappedEndStations.find(t) != mappedEndStations.end()) {
                        count += 1;
                    }
                }
            }
        }
        cout << "\n trip count: " <<  count << endl;
    }
    else{
        cout << " no such trip" << endl;
    }
}


int main(){
    cout << "** DIVVY analysis program **" << endl;
    cout << endl;


    const int Nstations = 10000;
    const int Ntrips = 2500000;
    const int Nbikes = 50000;
    hashmap<int,Station> stationsByIdTable(Nstations);
    hashmap<string, Abbr> stationsByAbbrevTable(Nstations);
    hashmap<string, Trip> tripsByIdTable(Ntrips);
    hashmap<string, Bike> bikesByIdTable(Nbikes);

//     input the stations and trips;
    string stationsFilename, tripsFilename;

    cout << "Enter stations file> ";
    getline(cin,stationsFilename);
    cout << "Enter trips file> ";
    getline(cin,tripsFilename);

    cout << "\nReading " << stationsFilename << endl;
    cout << "Reading " << tripsFilename << endl;
    cout << endl;

    bool successStations = inputDataStations(stationsFilename, stationsByIdTable, stationsByAbbrevTable);
    bool successTrips = inputDataTrips(tripsFilename, tripsByIdTable, bikesByIdTable);
    if (!successStations)
    {
        cout << "No data, file" <<  stationsFilename <<"not found?" << endl;
        return 0;
    }else if(!successTrips){
        cout << "No data, file not found?" << endl;
        return 0;
    }

    string userInput;
    cout << endl;
    cout << "Please enter a command, help, or #> ";
    getline(cin, userInput);
    //user menu
    while (userInput!= "#")
    {
        if(userInput == "help"){
            getHelp();
        }else if(isNumeric(userInput)){
            lookupStationById(userInput,stationsByIdTable);
        }else if((userInput.length() >= 7) && userInput.substr(0,7)=="similar"){
            lookupSimilarTrips(userInput, stationsByIdTable,tripsByIdTable);
        }else if((userInput.length() >= 6) && userInput.substr(0,6)=="nearby"){
            lookupNearbyStations(userInput, stationsByIdTable);
        }else if(userInput.substr(0,2)=="Tr"){
            int len = userInput.length();
            if(isNumeric((userInput.substr(2,(len-2))))){
                lookupTrip(userInput,tripsByIdTable, stationsByIdTable);
            }
        }else if(userInput.substr(0,1)=="B"){
            int len = userInput.length();
            if(isNumeric((userInput.substr(1,(len-1))))){
                lookupBikeUsage(userInput, bikesByIdTable);
            }
        }else{
            lookupStationByAbbrv(userInput,stationsByIdTable, stationsByAbbrevTable);
        }

        cout << endl;
        cout << "Please enter a command, help, or #> ";
        getline(cin, userInput);
    }


    cout << endl;
    return 0;
}
