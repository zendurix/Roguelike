#pragma once
// random map generator, based on BSP  (Binary Space Partitioning using binary tree)
// Base idea from	http://www.roguebasin.com/index.php?title=Basic_BSP_Dungeon_generation

// before making tree, try your specifed maxlevel, and min_size with check_sizes function !!!!!!

#include <cmath>
#include <vector>
#include <conio.h> // for testing only

#include "Node.h"
#include "map_functions.h"
#include "Place.h"
#include "Room.h"
#include "GAME.h" // used to check splitting with print_field
#include "RNG.h"

class GAME;
class Place;
class Room;
class Node;

class BSP_tree
{
private:
	const int MIN_SIZE;
	int levelmax;
	int nodes_num;
	Node *root_node;
	std::vector <Node*> nodes;
	std::vector <Node*> nodes_leaves;
	std::vector < std::vector <Node*> > nodes_by_levels[MAX_LEVEL + 1];

public:
	BSP_tree(int levelmax, int min_size);
	~BSP_tree();

	void make_full_tree(Place *main_field);

	void fill_nodes_by_levels();

	bool split_dungeon_BSP(Place *field, std::vector <Node*> *nodes, Node *parent, int level);

	void TEST_show_split(Place *field, bool full = true);

	void show_tree_details();

	void fill_leaves_with_rooms(Place *field, std::vector <Room*> *rooms);

	void connect_all_rooms(Place *field);

	void connect_2_rooms(Place *field, Place *searcher1, Place * searcher2, bool horizontal_connection);

	void print_number_rooms();
	

};

void chceck_sizes(int levelmax, int MIN_SIZE);



