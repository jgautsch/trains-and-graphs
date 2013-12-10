// graph.cpp
//
// Author: Jon Gautsch
//
// Contact: jgautsch@nd.edu
//
// Course: CSE 30331, Fall 2013

#include "graph.h"
#include <sstream>

using namespace std;

Graph::Graph() {}

void Graph::addNode(string name)
{
	if (!my_graph.count(name)) {
		Node temp;
		temp.name = name;
		my_graph[name] = temp;
	}
}


void Graph::removeNode(string name)
{
	if (this -> nodeExists(name)) {
		this -> removeAllEdgesFrom(name);
		this -> removeAllEdgesTo(name);
		my_graph.erase(name);
	}
}


void Graph::removeAllEdgesFrom(string name)
{
	if (this -> nodeExists(name)) {
		(my_graph.find(name) -> second).connections.clear();
	}

}


void Graph::removeAllEdgesTo(string name)
{
	for(main_iterator = my_graph.begin(); main_iterator != my_graph.end(); main_iterator++) {
		if ((main_iterator -> second).connections.count(name)) {
			(main_iterator -> second).connections.erase(name);
		}
	}
}


bool Graph::nodeExists(string name) {
	if (my_graph.count(name) != 0)
		return true;
	else
		return false;
}


bool Graph::edgeExists(string from, string to)
{
	if (this -> nodeExists(from) && this -> nodeExists(to)) {
		if ((my_graph.find(from) -> second).connections.count(to))
			return true;
		else
			return false;
	}
	return false;
}


void Graph::addOrUpdateEdge(string from, string to, int distance)
{
	if (this -> nodeExists(from) && this -> nodeExists(to)) {
		// If that connection already exists, update distance
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



int Graph::getEdgeDistance(string from, string to)
{
	if (this -> edgeExists(from, to)) {
		Edge *temp = this -> getEdge(from, to);
		return temp ->distance;
	}
	return 0;
}

Edge* Graph::getEdge(string from, string to)
{
	if (this -> edgeExists(from, to)) {
		return &((my_graph.find(from) -> second).connections.find(to) -> second);
	}
}


void Graph::removeEdge(string from, string to)
{
	if (my_graph.count(from)) {
		if (this -> edgeExists(from, to)) {
			(my_graph.find(from) -> second).connections.erase(to);
		}
	}
}


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


/**************************************************************/
// 		QUEUE RELATED FUNCTIONS
/**************************************************************/
void Graph::addToStationedQueue(string node, string value)
{
	(my_graph.find(node) -> second).stationed.push(value);
}

string Graph::getNodeQueueFront(string node)
{
	if ( !(my_graph.find(node) -> second).stationed.empty() )
		return (my_graph.find(node) -> second).stationed.front();
	else
		return "";
}

void Graph::addToTravelingQueue(string from, string to, string value)
{
	Edge *temp = this -> getEdge(from, to);
	(temp -> traveling).push(value);
}

string Graph::getEdgeQueueFront(string from, string to)
{
	if ( !((this -> getEdge(from, to)) -> traveling).empty() )
		return ((this -> getEdge(from, to)) -> traveling).front();
	else
		return "";
}

void Graph::popFrontNodeQueue(string node)
{
	if ( !(my_graph.find(node) -> second).stationed.empty() )
		(my_graph.find(node) -> second).stationed.pop();
}

void Graph::popFrontEdgeQueue(string from, string to)
{
	if ( !((this -> getEdge(from, to)) -> traveling).empty() )
		((this -> getEdge(from, to)) -> traveling).pop();
}

void Graph::findAndEliminateNodeQueueValue(string value)
{
	deque<string>::iterator i;



	for(main_iterator = my_graph.begin(); main_iterator != my_graph.end(); main_iterator++) {
		Node *temp = &(main_iterator -> second);


		// Empty the queue into the deque
		int j, size;
		string temp_value;
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

		size = (temp -> srsly).size();
		for (j = 0; j < size; j++)
		{
			temp_value = (temp -> srsly).front();
			(temp -> srsly).pop_front();
			(temp -> stationed).push(temp_value);
		}





	}
}