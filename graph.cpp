// graph.cpp
//
// Author: Jon Gautsch
// Contact: jgautsch@nd.edu
//
// Course: CSE 30331, Fall 2013

#include "graph.h"
#include <sstream>

using namespace std;

// Constructor. It doesn't do anything notable.
Graph::Graph() {}

/********************************************************************
 *	removeNode() :
 *		Given a node name, add that node from the graph if it
 *		doesn't already exist
 ********************************************************************/
void Graph::addNode(string name)
{
	if (!my_graph.count(name)) {
		Node temp;						// Instantiate temporary node
		temp.name = name;				// Set temp node's name, from param value
		my_graph[name] = temp;	 		// Add temp node to my_graph
	}
}

/********************************************************************
 *	removeNode() :
 *		Given a node name, delete that node from the graph if it
 *		exists.
 ********************************************************************/
void Graph::removeNode(string name)
{
	if (this -> nodeExists(name)) {			// If the node exists,
		this -> removeAllEdgesFrom(name);	// remove all edges coming from it,
		this -> removeAllEdgesTo(name);		// then remove all edges going to it,
		my_graph.erase(name);				// then remove that node from the graph.
	}
}

/********************************************************************
 *	removeAllEdgesFrom() :
 *		Given a node name, delete all the edges going from that node
 ********************************************************************/
void Graph::removeAllEdgesFrom(string name)
{
	if (this -> nodeExists(name)) {								// If the node exists,
		(my_graph.find(name) -> second).connections.clear();	// clear out its connections map
	}

}

/********************************************************************
 *	removeAllEdgesTo() :
 *		Given a node name, iterate through all nodes and check all
 * 		their edges. If any of those edges go to the node in question,
 *		destroy that edge.
 ********************************************************************/
void Graph::removeAllEdgesTo(string name)
{
	// Iterate through all the nodes in the graph
	for(main_iterator = my_graph.begin(); main_iterator != my_graph.end(); main_iterator++) {

		// If the current node has an edge to the node in question,
		// then delete that edge
		if ((main_iterator -> second).connections.count(name)) {
			(main_iterator -> second).connections.erase(name);
		}
	}
}


/********************************************************************
 *	nodeExists() :
 *		Given a node name, see if there's a node with that name
 *		and if there is, return true, else return false
 *
 * 	@return [bool] True if node exists, false otherwise.
 ********************************************************************/
bool Graph::nodeExists(string name) {
	if (my_graph.count(name) != 0)
		return true;
	else
		return false;
}


/********************************************************************
 *	edgeExists() :
 *		Given two node names, from and to, check if an edge
 *		exists between them, and if one does, return true
 *		else, return false.
 *
 *	@return [bool] True if edge exists, false otherwise.
 ********************************************************************/
bool Graph::edgeExists(string from, string to)
{
	// If the specified 'from' and 'to' nodes both exists,
	if (this -> nodeExists(from) && this -> nodeExists(to)) {

		// See if there's an edge between the 'from' node to the 'to' node
		if ((my_graph.find(from) -> second).connections.count(to))
			return true;
		else
			return false;
	}
	return false;
}


/********************************************************************
 *	addOrUpdateEdge() :
 *		Given two node names, from and to, check if and edge
 *		exists between them, and if on does, delete that edge.
 ********************************************************************/
void Graph::addOrUpdateEdge(string from, string to, int distance)
{
	// If the specified 'from' and 'to' nodes both exists,
	if (this -> nodeExists(from) && this -> nodeExists(to)) {
		// if that connection already exists, update distance
		if (this -> edgeExists(from, to)) {
			((my_graph.find(from) -> second).connections.find(to) -> second).distance = distance;
		} else {
			// connection doesn't exist, so create it
			Edge temp;
			temp.distance = distance;
			temp.to = to;
			temp.from = from;
			(my_graph.find(from) -> second).connections[to] = temp;
		}
	}
}


/********************************************************************
 *	getEdgeDistance() :
 *		Given two node names, from and to, check if an edge
 *		exists between them, and if one does, return the weight
 *		of the edge, or the distance between the nodes.
 *
 *	@return [int] The value of the distance between the nodes
 ********************************************************************/
int Graph::getEdgeDistance(string from, string to)
{
	if (this -> edgeExists(from, to)) {				// if the specified edge exists,
		Edge *temp = this -> getEdge(from, to);		// get a reference to it
		return temp -> distance;					// and return it's weight/distance
	}
	return 0;
}


/********************************************************************
 *	getEdge() :
 *		Given two node names, from and to, find the edge between
 *		them (if one exists), and return a reference to it. A
 *		reference is returned so that it can be edited by the
 *		calling code.
 *
 *	@return [*Edge] A reference to the desired edge
 ********************************************************************/
Edge* Graph::getEdge(string from, string to)
{
	// If the specified edge exists
	if (this -> edgeExists(from, to)) {
		// return a reference to the edge between 'from' and 'to'
		return &((my_graph.find(from) -> second).connections.find(to) -> second);
	}
}

/********************************************************************
 *	removeEdge() :
 *		Given two node names, from and to, check if an edge
 *		exists between them, and if one does, delete that edge.
 ********************************************************************/
void Graph::removeEdge(string from, string to)
{
	// If node exists,
	if (my_graph.count(from)) {
		// And the edge specified exists,
		if (this -> edgeExists(from, to)) {
			// Erase that connection/edge
			(my_graph.find(from) -> second).connections.erase(to);
		}
	}
}

/********************************************************************
 * 	print() :
 * 		Iterate through the graph printing the node names
 *		Then iterate through all the edges and print the
 *		<from> -> <to> relationships
 ********************************************************************/
void Graph::print()
{
	cout << "size: " << my_graph.size() << endl;
	cout << "Nodes:" << endl;
	for(main_iterator = my_graph.begin(); main_iterator != my_graph.end(); main_iterator++) {
		cout << " - " << (main_iterator -> second).name << endl;
	}
	cout << "Edges:" << endl;
	for(main_iterator = my_graph.begin(); main_iterator != my_graph.end(); main_iterator++) {
		if ((main_iterator -> second).connections.size() != 0) {
			Node temp = (main_iterator -> second);
			for(sub_iterator = temp.connections.begin(); sub_iterator != temp.connections.end(); sub_iterator++) {
				cout << " - ";
				cout << temp.name << " -> " << (sub_iterator -> second).to << "; " << (sub_iterator -> second).distance << endl;
			}
		}
	}
}


/*  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  **/
/**  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  */
// 		QUEUE RELATED FUNCTIONS
/**  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  */
/*  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  **/




/********************************************************************
 * 	addToStationedQueue() :
 * 		Given a node name and a value, get the node and
 *		add the value to that node's queue
 ********************************************************************/
void Graph::addToStationedQueue(string node, string value)
{
	(my_graph.find(node) -> second).stationed.push(value);
}


/********************************************************************
 * 	addToTravelQueue() :
 * 		Given a value, and start/end node names, add the value
 *		to the specified edge's traveling queue
 ********************************************************************/
void Graph::addToTravelingQueue(string from, string to, string value)
{
	// First get a reference the relevant edge
	Edge *temp = this -> getEdge(from, to);

	// Then push the value onto that edge's traveling queue
	(temp -> traveling).push(value);
}


/********************************************************************
 * 	getNodeQueueFront() :
 *		Given the name of a node, get the node and return the
 *			value at the front of its queue
 *
 *	@return [string] The value at the front of the specified Node
 ********************************************************************/
string Graph::getNodeQueueFront(string node)
{
	if ( !(my_graph.find(node) -> second).stationed.empty() )
		return (my_graph.find(node) -> second).stationed.front();
	else
		return "";
}

/********************************************************************
 * 	getEdgeQueueFront() :
 * 		Given two node names, from and to, get the edge
 * 		between those node names, and get the value at
 *		the front of its queue.
 *
 *	@return [string] The value at the front of the specified Edge
 ********************************************************************/
string Graph::getEdgeQueueFront(string from, string to)
{
	if ( !((this -> getEdge(from, to)) -> traveling).empty() )
		return ((this -> getEdge(from, to)) -> traveling).front();
	else
		return "";
}


/********************************************************************
 * 	popFontNodeQueue() :
 * 		Given a node name value, find the node with that name,
 *		and pop the front value from its queue.
 ********************************************************************/
void Graph::popFrontNodeQueue(string node)
{
	if ( !(my_graph.find(node) -> second).stationed.empty() )
		(my_graph.find(node) -> second).stationed.pop();
}


/********************************************************************
 * 	popFrontEdgeQueue() :
 * 		Given the from and to values, pop the front of the queue
 *		hat exists on the edge between those two node names
 ********************************************************************/
void Graph::popFrontEdgeQueue(string from, string to)
{
	if ( !((this -> getEdge(from, to)) -> traveling).empty() )
		((this -> getEdge(from, to)) -> traveling).pop();
}

/********************************************************************
 * 	findAndEliminateNodeQueueValue() :
 * 		Given a value, find the Node that contains that value in
 * 		its queue, and eliminate that value from the queue
 ********************************************************************/
void Graph::findAndEliminateNodeQueueValue(string value)
{
	deque<string>::iterator i;

	for(main_iterator = my_graph.begin(); main_iterator != my_graph.end(); main_iterator++) {

		Node *temp = &(main_iterator -> second);
		int j, size;
		string temp_value;

		// The following 3 step process is totally contrived, but the assignment that
		// this is being immediately used for requires the use of a queue, even
		// though their limited functionality (not being able to iterate over them)
		// makes them a less than ideal data structure for the problem being solved.
		// To get around the inconvenience, empty the queue into a deque, iterate
		// over it removing the marked value, then empty whats left back into the queue.

		// Empty the queue into the deque
		size = (temp -> stationed).size();
		for (j = 0; j < size; j++)
		{
			temp_value = (temp -> stationed).front();
			(temp -> stationed).pop();
			(temp -> srsly).push_back(temp_value);
		}

		// Do what you need to do elimination wise
		for( i = (temp -> srsly).begin(); i != (temp -> srsly).end();) {
			if ((*i) == value) {
				i = (temp -> srsly).erase(i); // erase returns the next iterator
			} else
				++i;
		}

		// Now empty the deque back into the queue container
		size = (temp -> srsly).size();
		for (j = 0; j < size; j++)
		{
			temp_value = (temp -> srsly).front();
			(temp -> srsly).pop_front();
			(temp -> stationed).push(temp_value);
		}

	}
}