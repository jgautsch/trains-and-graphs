// graph.h
//
// Author: Jon Gautsch
//
// Contact: jgautsch@nd.edu
//
// Course: CSE 30331, Fall 2013

#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <map>

using namespace std;

struct Edge {
	string to;
	string from;
	int distance;
	queue<string> traveling;
};

struct Node {
	string name;
	map<string, Edge> connections;
	deque<string> srsly;
	queue<string> stationed;
};

class Graph {
public:


	Graph();
	Edge* getEdge(string from, string to);
	Node getNode(string name);

	void addNode(string name);
	void removeNode(string name);
	void removeAllEdgesFrom(string name);
	void removeAllEdgesTo(string name);
	bool nodeExists(string name);
	bool edgeExists(string from, string to);
	void addOrUpdateEdge(string from, string to, int distance);
	int getEdgeDistance(string from, string to);
	void removeEdge(string from, string to);
	void print();

	void addToStationedQueue(string node, string value);
	string getNodeQueueFront(string node);
	void addToTravelingQueue(string from, string to, string value);
	string getEdgeQueueFront(string from, string to);
	void popFrontNodeQueue(string node);
	void popFrontEdgeQueue(string from, string to);
	void findAndEliminateNodeQueueValue(string value);

private:

	map<string, Node>::iterator main_iterator;
	map<string, Edge>::iterator sub_iterator;

	map<string, Node> my_graph;

};

