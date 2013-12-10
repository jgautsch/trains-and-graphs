// train_sim.h
//
// Author: Raul Santelices
//
// Contact: cse30331fa13_tas@listserv.nd.edu
//
// In this file you can add variables, functions, classes, etc. as needed.
// Do NOT remove or modify variables, functions, etc. already provided here.
//
// Course: CSE 30331, Fall 2013


#ifndef TRAIN_SIM_H
#define TRAIN_SIM_H

#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <utility>
#include <vector>
#include <map>
#include "graph.h"

using namespace std;


// Class representing a network of train stations and trains stationed or moving among connected stations
// Rules for PA3:
//  1. For the network of stations, you must either (a) write your own graph code from scratch or (b) use the Boost graph library
//  2. You must use the Train struct (defined below) to store each train's information
//  3. You must use stl::queue containers to store trains (or pointers to trains) and place them in station vertices in your graph
//       (Hint: if you use Boost graphs, use vertex properties to associate the queues to vertices)
//  4. In addition to train_sim.cxx, you can modify train_sim.h (do NOT change what is already here though) and the Makefile and even add new files
class TrainSimulator {
public:
	// *** DO NOT MODIFY ANY OF THE FOLLOWING TYPEDEFS AND FUNCTIONS ***

	// TYPEDEFS
	typedef unsigned int uint;
	typedef uint Distance;

	// CONSTRUCTORS
	TrainSimulator(); // *** NOT GRADED because it's given to you

	// SIMULATION ACTIONS
	void step();
	void departNextTrain(const string& fromStation, const string& toStation);

	// STATION CREATION, CONNECTIVITY, & REMOVAL
	void addStation(const string& name);
	void removeStation(const string& name);
	void connectStations(const string& from, const string& to, Distance distance);
	void disconnectStations(const string& from, const string& to);

	// STATION-SPECIFIC QUERIES
	bool stationExists(const string& name);
	bool stationsConnected(const string& from, const string& to);
	Distance connectionLength(const string& from, const string& to);

	// TRAIN INSERTION, MODIFICATION, & REMOVAL
	void addTrain(const string& name, uint wagons, const string& station);
	void removeTrain(const string& name);
	void addWagon(const string& train);
	void removeWagon(const string& train);

	// TRAIN-SPECIFIC QUERIES
	bool trainExists(const string& name) const;
	bool trainTraveling(const string& name) const;
	uint numberWagons(const string& train) const;
	const string& whereStationed(const string& train) const;
	const string& trainOrigin(const string& train) const;
	const string& trainDestination(const string& train) const;
	Distance distanceTraveled(const string& train) const;

	void resetTrainFlags(const string& train);

	void print(ostream& outs = cout);  // *** NOT GRADED; use it to help yourself visualize your solution

	//  *** you can add your own public members variables, as needed

private:
	// PRIVATE TYPES
	struct Train {  // *** IMPORTANT: You must use this structure to handle trains and to store them (or pointers to them) in stl::queue containers ***

		//  *** do NOT change the following three variables
		string name; 				// name of the train; doesn't change
		uint wagons; 				// number of wagons in this train; always greater than 0
		Distance traveled; 			// distance traveled so far (if traveling)

		bool traveling;				// boolean for if the train is traveling
		bool flagged_for_removal;	// flag for if the train will be removed upon arriving at station
		bool flagged_for_wagon_removal; // flagged for removing wagon

		string station;				// the station it's at, or traveling to
		string destination_station; // the station it's traveling to
		string origin_station;		// the station it originated from
		int distance_left;			// the distance to the station it's traveling to
	};

	map<string, Train> trains_map;
	map<string, Train>::iterator it_type;

	Graph stations_graph;

};

#endif // TRAIN_SIM_H
