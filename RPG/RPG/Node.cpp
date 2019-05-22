#include "pch.h"
#include "Node.h"

#include <stdio.h> // FOR TESTING

int Node::level = 0;
int Node::node_count = -1;

Node::Node(int levelmax, Place *field_node1, int length1, int height1, int orientation)
{	
	field_node = field_node1;
	field_node->print_format = 'X';
	length = length1;
	height = height1;

	horizontal = orientation;

	room = NULL;

	node_count++;
	node_index = node_count;

	if (node_index == 1  || node_index == 3  ||
		node_index == 7  || node_index == 15 ||
		node_index == 31 || node_index == 63  )
		level++;
	level_index = level;

	if (level == 0)
	{
		is_root = true;
		parent = NULL;
		sister = NULL;
	}
	else
		is_root = false;	

	if (level == levelmax)
	{
		is_leaf = true;
		childreen[0] = NULL;
		childreen[1] = NULL;
	}
	else
		is_leaf = false;	
}

Node::~Node() {}


// lists all "parents and grandparents" of node, in family vector
void Node::check_family()
{
	Node *family_searcher = parent;
	while (family_searcher != NULL)
	{
		family.push_back(family_searcher->node_index);
		family_searcher = family_searcher->parent;
	}
}

void make_childreen(Node *sister1, Node *sister2, Node *parent1)
{
	sister1->sister = sister2;
	sister2->sister = sister1;

	sister1->parent = parent1;
	sister2->parent = parent1;

	parent1->childreen[0] = sister1;
	parent1->childreen[1] = sister2;
}

void Node::give_room_number()
{
	room->room_index = node_index;
}

