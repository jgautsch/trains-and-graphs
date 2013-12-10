// train_sim.cxx
//
// Author: Raul Santelices
// Some Updates by: Richard Molina
//
// Contact: cse30331fa13_tas@listserv.nd.edu
//
// You will implement your solution in this file,
// other than whatever you add to train_sim.h and maybe new files you create
//
// Course: CSE 30331, Fall 2013

#include "train_sim.h"
#include <sstream>

using namespace std;

// Convenient typedefs
typedef TrainSimulator::uint uint;
typedef TrainSimulator::Distance Distance;


// CONSTRUCTOR(S)

// POSTCONDITION: The network is empty -- no stations and no trains.
//   Note 1: You shouldn't need to change this (except if you need to initialize something specifc to your solution
//   Note 2: This contructor is NOT graded
TrainSimulator::TrainSimulator() {}


// SIMULATION ACTIONS

// POSTCONDITION: All traveling trains have moved 1 unit towards their respective destinations.
//                Trains that completed the distance of the connection stop traveling and become
//                stationed (by being enqueued) at the destination station.
void TrainSimulator::step()
{
	for(it_type = trains_map.begin(); it_type != trains_map.end(); it_type++) {
		Train *temp = &(it_type -> second);
		if (temp -> traveling) {
    		temp -> traveled = temp -> traveled + 1;
    		temp -> distance_left = temp -> distance_left - 1;

    		// If the distance left for a traveling train is now zero, arrive it at it's destination
    		// by taking it out of the traveling edge queue and putting it in the stationed queue
    		if ( temp -> distance_left == 0 ) {
    			string origin, destination, name;
    			origin = temp -> origin_station;
    			destination = temp -> destination_station;
    			name = temp -> name;

				// Leave edge queue, join node queue
				stations_graph.popFrontEdgeQueue(origin, destination);

				// Do appropriate behavior based on flags set.
				// ....
				if (temp -> flagged_for_removal) {
					trains_map.erase(name);
				} else {
					stations_graph.addToStationedQueue(destination, name);

    				// update Train info (station, destination_station, origin)
    				temp -> station = temp -> destination_station;
    				temp -> origin_station = temp -> destination_station;
    				temp -> distance_left = 0;
    				temp -> traveling = false;
    				temp -> traveled = 0;
    				this -> resetTrainFlags(temp -> name);

    				if (temp -> flagged_for_wagon_removal) {
    					temp -> wagons = temp -> wagons - 1;
    				}
				}
    		}
    	}
	}
}

// PRECONDITION: stationsConnected(fromStation, toStation) and there is at least one train stationed at 'fromStation'
// POSTCONDITION: The train at the front of the stationed queue in 'fromStation' leaves that queue and
//                enters the traveling queue for the connection to 'toStation' with initial traveled distance 0.
void TrainSimulator::departNextTrain(const string& fromStation, const string& toStation) {

	if (stations_graph.getNodeQueueFront(fromStation) != "") {
		// Get our relevant info
		int travel_distance = stations_graph.getEdgeDistance(fromStation, toStation);
		string train_name = stations_graph.getNodeQueueFront(fromStation);

		// Hold a reference to the train
		Train *temp = &(trains_map.find(train_name) -> second);
		temp -> destination_station = toStation;
		temp -> origin_station = fromStation;
		temp -> distance_left = travel_distance;
		temp -> traveling = true;
		this -> resetTrainFlags(temp -> name);

		// Remove from stationed queue and add to traveling queue

		stations_graph.popFrontNodeQueue(fromStation);
		stations_graph.addToTravelingQueue(fromStation, toStation, train_name);
	}
}

void TrainSimulator::resetTrainFlags(const string& train) {
	Train *temp = &(trains_map.find(train) -> second);
	temp -> flagged_for_removal = false;
	temp -> flagged_for_wagon_removal = false;
}

/***********************************************************/
// STATION FUNCTIONS (CREATION, CONNECTIVITY, & REMOVAL)
/***********************************************************/

// POSTCONDITION: stationExists(name)
//  Note: If station 'name' already existed in the network, nothing changes -- its connections and
//        the status of incoming/stationed/outgoing trains remains the same.
void TrainSimulator::addStation(const string& name) {

	stations_graph.addNode(name);
}

// PRECONDITION: stationExists(name)
// POSTCONDITION: !stationExists(name). All trains stationed at 'name' or traveling to 'name' were removed.
//                All trains traveling from 'name' automatically arrived (got stationed) at their destinations (enqueued in their travel from 'name').
//                All connections to and from station 'name' have been removed.
void TrainSimulator::removeStation(const string& name) {

	stations_graph.removeNode(name);
}

// PRECONDITION: stationExists(from) && stationExists(to) && distance > 0
// POSTCONDITION: A directed link (a railroad) of length 'distance' is created from station 'from' to station 'to'.
//                If a connection already existed from 'from' to 'to', it is replaced by this new connection and distance.
void TrainSimulator::connectStations(const string& from, const string& to, Distance distance) {

	stations_graph.addOrUpdateEdge(from, to, distance);
}

// PRECONDITION: stationsConnected(from, to)
// POSTCONDITION: !stationsConnected(from, to)
//                All trains traveling from 'from' to 'to' have automatically arrived to (got stationed at) their destinations.
//  Note: If the stations were not connected, there is no change to the state of the simulation.
void TrainSimulator::disconnectStations(const string& from, const string& to) {

	stations_graph.removeEdge(from, to);
}

/***********************************************************/
// STATION-SPECIFIC QUERIES
/***********************************************************/

// POSTCONDITION: Returned true if station 'name' is in the network (i.e., 'name' was added before and not removed since then).
//                Returned false otherwise
bool TrainSimulator::stationExists(const string& name) {

	return stations_graph.nodeExists(name);
}

// PRECONDITION: stationExists(from) && stationExists(to)
// POSTCONDITION: Returned true if station 'name' is in the network (i.e., 'name' was added before and not removed since then).
//                Returned false otherwise
bool TrainSimulator::stationsConnected(const string& from, const string& to) {

	return stations_graph.edgeExists(from, to);
}

// PRECONDITION: stationExists(from) && stationExists(to) && stationsConnected(from, to)
// POSTCONDITION: Returned distance 'distance' provided when connectStations(from, to, distance) was last called
Distance TrainSimulator::connectionLength(const string& from, const string& to) {

	return stations_graph.getEdgeDistance(from, to);
}

/***********************************************************/
// TRAIN FUNCTIONS (INSERTION, MODIFICATION, & REMOVAL)
/***********************************************************/

// PRECONDITION: wagons > 0 && stationExists(station)
// POSTCONDITION: trainExists(name) && numberWagons(name) == wagons && whereStationed(name) == station
//   Note 1: The train will be located in one and only one queue at a time (i.e., a "traveling-to" or "stationed" queue)
//   Note 2: If the train existed, the only effect was to update its number of wagons with parameter 'wagons'. Its queue position remains the same.
//   Note 3: A train will always have at least one wagon
void TrainSimulator::addTrain(const string& name, uint wagons, const string& station) {

	// If that train exists, then update the number of wagons
	if (trainExists(name)) {
		trains_map[name].wagons = wagons;
	} else {
		// else if doesn't exist yet, create the train
		// Train temp(name, wagons, 0);
		Train temp;
		temp.name = name;
		temp.wagons = wagons;
		temp.traveled = 0;
		temp.traveling = false;
		temp.flagged_for_removal = false;
		temp.flagged_for_wagon_removal = false;
		temp.station = station;
		temp.destination_station = station;
		temp.origin_station = station;
		temp.distance_left = 0;

		// Now add it to the trains_map
		trains_map[name] = temp;
		// and add stationed queue !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		stations_graph.addToStationedQueue(station, name);
	}
}

// PRECONDITION: trainExists(name)
// POSTCONDITION: !trainExists(name)
//  Note 1: The train was removed from stationed queue it was located
//  Note 2: If train is traveling, flag train for removal and remove when arrives at station.
void TrainSimulator::removeTrain(const string& name) {
	if (trainExists(name)) {
		if (trains_map[name].traveling) {
			trains_map[name].flagged_for_removal = true;
		} else {
			trains_map.erase(name);

			stations_graph.findAndEliminateNodeQueueValue(name);
		}
	}

}

// PRECONDITION: trainExists(train) && numberWagons(train) > 0
// POSTCONDITION: numberWagons(train) was incremented by 1
//  Note: If train is traveling, flag train for wagon removal and remove when arrives at station.
void TrainSimulator::addWagon(const string& train) {

	(trains_map.find(train) -> second).wagons++;
}

// PRECONDITION: trainExists(train) && numberWagons(train) > 1
// POSTCONDITION: numberWagons(train) was decremented by 1
//  Note: If train is traveling, flag train for wagon removal and remove when arrives at station.
void TrainSimulator::removeWagon(const string& train) {
	if (trainExists(train)) {
		(trains_map.find(train) -> second).wagons--;
	}
}

/***********************************************************/
// TRAIN-SPECIFIC QUERIES
/***********************************************************/

// POSTCONDITION: Returned true if train 'name' is in the network (i.e., 'name' was added before and not removed since then).
//                Returned false otherwise
bool TrainSimulator::trainExists(const string& name) const {

	return (trains_map.count(name) ? true : false);
}

// PRECONDITION: trainExists(name)
// POSTCONDITION: Returned true if train 'name' is traveling from some station to another.
//                Returned false otherwise (i.e., the train is stationed at some station)
bool TrainSimulator::trainTraveling(const string& name) const {

	return (trains_map.find(name) -> second).traveling;
}

// PRECONDITION: trainExists(train)
// POSTCONDITION: Returned number > 0 of wagons currently in train 'train'.
uint TrainSimulator::numberWagons(const string& train) const {

	return (trains_map.find(train) -> second).wagons;
}

// PRECONDITION: !trainTraveling(train)
// POSTCONDITION: Returned name of station in which train is stationed.
const string& TrainSimulator::whereStationed(const string& train) const {

	return (trains_map.find(train) -> second).station;
}

// PRECONDITION: trainTraveling(train)
// POSTCONDITION: Returned name of station from which train is traveling.
const string& TrainSimulator::trainOrigin(const string& train) const {

	return (trains_map.find(train) -> second).origin_station;
}

// PRECONDITION: trainTraveling(train)
// POSTCONDITION: Returned name of station to which train is traveling.
const string& TrainSimulator::trainDestination(const string& train) const {

	return (trains_map.find(train) -> second).destination_station;
}

// PRECONDITION: trainTraveling(train)
// POSTCONDITION: Returned distance >= 0 traveled so far by the train.
Distance TrainSimulator::distanceTraveled(const string& train) const {

	return (trains_map.find(train) -> second).traveled;
}

/***********************************************************/
// TRAIN NETWORK PRINTING
/***********************************************************/

// *** NOT GRADED; use this to help you visualize your solution
void TrainSimulator::print(ostream& outs /*= cout*/) {

	stations_graph.print();
}

