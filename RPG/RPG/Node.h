#pragma once
#include <cmath>
#include <vector>

#include "Place.h"
#include "Room.h"

class Place;
class Room;

class Node
{
public:
	static int level;
	static int node_count;
	int node_index;
	int level_index;

	Node *parent;
	Node *sister;
	Node *childreen[2]; //array containing pointers to two childreen

	std::vector <int> family;

	Place *field_node; // pointer to first element, of node field

	Room *room;

	int length; // x axis
	int height; // y axis

	bool horizontal; // if false, than vertical

	bool is_root; // first node of tree (no parent)
	bool is_leaf; // node in last tree level (no childreen)


	Node(int levelmax, Place *field_node1, int length1, int height1, int orientation);
	~Node();

	void check_family();
	void give_room_number();
	
};


void make_childreen(Node *sister1, Node *sister2, Node *parent1);
