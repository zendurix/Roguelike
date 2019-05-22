#include "pch.h"
#include "BSP_map_gen.h"
// idea for tree based on: http://www.roguebasin.com/index.php?title=Basic_BSP_Dungeon_generation
// makes FULL binary tree to given max level, and splits dungeon
/*    EXAMPLE (maxlevel == 2):            
					0            LEVEL 0:  main root node
				   / \
				 /     \
			   1         2		 LEVEL 1
			  / \       / \
			 /   \     /   \
			3	  4   5     6    LEVEL 2: last level, leaves
										  amount of nodes on that level == 2^maxlevel
										  amount of leaves is amount of rooms to be placed
*/

BSP_tree::BSP_tree(int levelmax1, int min_size)
	: MIN_SIZE(min_size)
{
	levelmax = levelmax1;
	// to actually make a full tree call make_full_tree method
	// this was split into other method, due to restarting problem
}

BSP_tree::~BSP_tree()
{

}


void BSP_tree::make_full_tree(Place *main_field)
{
	Place *main_field_0 = main_field;
	int actual_level;

	nodes_num = 0;
	for (int i = 0; i <= levelmax; i++)
		nodes_num += (int)pow(2, i);
	nodes.reserve(nodes_num);

	root_node = new  Node(levelmax, main_field, MYLENGTH, MYHEIGHT, 0);
	nodes.push_back(root_node);

	for (int i = 0; i < levelmax; i++)
	{
		actual_level = root_node->level;

		for (int j = (int)pow(2, actual_level) - 1; j <= (int)pow(2, actual_level + 1) - 2; j++)
		{
			if (!split_dungeon_BSP(nodes[j]->field_node, &nodes, nodes[j], actual_level))
			{ // RNG failed (loops over 200 times)
				nodes[0]->level = 0;
				nodes[0]->node_count = -1;
				delete root_node;
				nodes.clear();
				main_field = main_field_0;
				throw "false";
			}
		}
	}
	fill_nodes_by_levels();
	main_field = main_field_0;
}


void BSP_tree::fill_nodes_by_levels()
{
	for (int i = 0; i <= levelmax; i++)
	{
		std::vector <Node*> temp;

		for (int j = 0; j < nodes_num; j++)
			if (nodes[j]->level == i)
				temp.push_back(nodes[i]);

		nodes_by_levels->push_back(temp);
	}
}

// splits dungeon to random fields, using BSP tree. it returns false, when loops over 200 times
bool BSP_tree::split_dungeon_BSP(Place *field, std::vector <Node*> *nodes, Node *parent, int level)
{ // field is parent->field-node
	Place *field_0 = field;
	Node *node1 = NULL, *node2 = NULL;
	bool orientation = random(0, 1);
	int length1, length2, height1, height2;
	int x, y;
	int error = 0;

	if (orientation == 0) // horizontal
		do 
		{
			y = random(parent->field_node->y + parent->height / 2 - MIN_SIZE / 2,
				parent->field_node->y + parent->height / 2 + MIN_SIZE / 2);
			length1 = parent->length;
			length2 = length1;
			height1 = y - parent->field_node->y;
			height2 = parent->height - height1;

			//printf("\nlosu y");
			error++;
			if (error > 200)
				return false;

		} while (height1 < MIN_SIZE+1 ||  height2 < MIN_SIZE+1);

	else if (orientation == 1)  // vertical
		do
		{
			x = random(parent->field_node->x + parent->length / 2 - MIN_SIZE / 2,
				parent->field_node->x + parent->length / 2 - MIN_SIZE / 2);
			length1 = x - parent->field_node->x;
			length2 = parent->length - length1;
			height1 = parent->height;
			height2 = parent->height;

			//printf("\nlosu x");
			error++;
			if (error > 200)
				return false;

		} while (length1 < MIN_SIZE+1 || length2 < MIN_SIZE+1);

	node1 = new Node(levelmax, field, length1, height1, orientation);
	
	field += (orientation == 0) ? (height1)* MYLENGTH : length1;

	node2 = new Node(levelmax, field, length2, height2, orientation);

	make_childreen(node1, node2, parent);
	nodes->push_back(node1);
	nodes->push_back(node2);

	field = field_0;
	return true;
}

// test how split_dungeon_BSP works
void BSP_tree::show_split(Place *field, bool full)
{
	Place *field_0 = field;
	GAME gam;

	for (int i = 0; i < nodes_num; i++)
	{
		if (nodes[i]->is_leaf)
		{
			Room *room;
			room = new Room;

			make_room("rectangle", field, nodes[i]->field_node->x + 1, nodes[i]->field_node->y + 1,
				nodes[i]->length - 2, nodes[i]->height - 2, room, "", false);

			if (!full)
			{
				printf("\n======================================\n");
				gam.print_field(field, false);
				printf("\n room w %d", i);

				fflush(stdin);
				_getch();
			}
		}
	}
	printf("\n======================================\n");
	gam.print_field(field, false);
	fflush(stdin);
	_getch();
	field = field_0;
}

// show for each split: x, y, length and height
void BSP_tree::show_tree_details()
{
	printf("\nTREE DETAILS TEST");
	int surface_total = 0;
	for (int i = 0; i < nodes_num; i++)
	{
		printf(" \n --------------\nnode %d, is leaf %d", i, nodes[i]->is_leaf);
		if (nodes[i]->parent != NULL)
			printf("\n parent length: %d   height : %d ", nodes[i]->parent->length, nodes[i]->parent->height);
		printf("\n x: %d   y : %d ", nodes[i]->field_node->x, nodes[i]->field_node->y);
		printf("\n length: %d   height : %d ", nodes[i]->length, nodes[i]->height);
		if (nodes[i]->is_leaf)
			surface_total += nodes[i]->length * nodes[i]->height;
	}
	printf("\nTREE SURFACE %d, should be equal to %d", surface_total, MYHEIGHT * MYLENGTH);
	fflush(stdin);
	_getch();
}

// makes random sized and placed rooms in each leaves field of tree
void BSP_tree::fill_leaves_with_rooms(Place *field, std::vector <Room*> *rooms)
{
	Place *field_0 = field;
	make_whole_map_obstacle(field);
	GAME gam;
	int length1, height1;
	int x1, y1;
	for (int i = 0; i < nodes_num; i++)
	{
		if (nodes[i]->is_leaf)
		{
			Room *room;
			room = new Room;

			if (nodes[i]->length > MIN_SIZE + 10)
				length1 = random(MIN_SIZE + 4, MIN_SIZE + 8);
			else if (nodes[i]->length > MIN_SIZE + 5)
				length1 = random(MIN_SIZE + 2, MIN_SIZE + 3);
			else
				length1 = random(MIN_SIZE - 1, MIN_SIZE);


			if (nodes[i]->height > MIN_SIZE + 7)
				height1 = random(MIN_SIZE + 2, MIN_SIZE + 5);
			else if (nodes[i]->height > MIN_SIZE + 4)
				height1 = random(MIN_SIZE, MIN_SIZE + 2);
			else
				height1 = random(MIN_SIZE - 1, MIN_SIZE);


			if (nodes[i]->field_node->x + 1 < (nodes[i]->field_node->x + nodes[i]->length) - (length1 + 1))
				x1 = random(nodes[i]->field_node->x + 1, (nodes[i]->field_node->x + nodes[i]->length) - (length1 + 1));
			else
				x1 = nodes[i]->field_node->x + 1;


			if (nodes[i]->field_node->y + 1 < (nodes[i]->field_node->y + nodes[i]->height) - (height1 + 1))
				y1 = random(nodes[i]->field_node->y + 1, (nodes[i]->field_node->y + nodes[i]->height) - (height1 + 1));
			else
				y1 = nodes[i]->field_node->y + 1;


			make_room("rectangle", field, x1, y1, length1, height1, room, "", false);

			nodes[i]->room = room;
			rooms->push_back(room);
		}
	}
	for (int i = 0; i, i < nodes_num; i++)
		if (nodes[i]->is_leaf)
			nodes[i]->check_family();
	field = field_0;
}

// connect all rooms with corridors
void BSP_tree::connect_all_rooms(Place *field)
{
	Place *searcher1 = NULL, *searcher2 = NULL;
	Node *node1 = NULL, *node2 = NULL;
	bool horizontal_connection;
	bool found = false;
	bool child;

	for (int j = levelmax; j > 0; j--)
	{
		for (int i = (int)pow(2, j) - 1; i < (int)pow(2, j + 1) - 2; i += 2)
		{
			found = false;
			while (!found)
			{
				node1 = nodes[i];
				node2 = nodes[i + 1];
				while (node1->childreen[0] != NULL)
				{
					child = random(0, 1);
					node1 = node1->childreen[child];
					node2 = node2->childreen[child];
				}

				if (nodes[i]->horizontal)
				{
					if (node2->room->cornerNW.x >= node1->room->cornerNE.x)
						found = true;
				}
				else  // vertical
				{
					if (node2->room->cornerNE.y >= node1->room->cornerSE.y)
						found = true;
				}
			}

			horizontal_connection = (node2->room->cornerNE.y - node1->room->cornerSE.y >=
				node2->room->cornerNW.x - node1->room->cornerNE.x) ? 0 : 1;


			if (horizontal_connection) //(nodes[i]->horizontal)
			{
				searcher1 = node1->room->cornerNE.corner_ptr + 2 * MYLENGTH;
				searcher2 = node2->room->cornerNW.corner_ptr + 2 * MYLENGTH;
			}
			else // vertical
			{
				searcher1 = node1->room->cornerSW.corner_ptr + 2;
				searcher2 = node2->room->cornerNW.corner_ptr + 2;
			}

			connect_2_rooms(field, searcher1, searcher2, horizontal_connection); // node1->horizontal);
		}		
			   
	}	   
}


void BSP_tree::connect_2_rooms(Place *field,  Place *searcher1, Place * searcher2, bool horizontal_connection)
{
	Place *field_0 = field;
	int x_search, y_search;
	int height_search, length_search;
	int distance, distance1, distance2;
	bool goUp = false;
	bool goLeft = false;
	bool straight = true;
	bool found = false;

	if (horizontal_connection)
	{
		x_search = searcher1->x;
		distance = searcher2->x - searcher1->x;		
		distance ++;

		if (searcher1->y == searcher2->y)
			y_search = searcher1->y;

		else if (searcher1->y > searcher2->y) // first room is under second one
		{
			goUp = true;
			while ( (searcher2 + 2*MYLENGTH)->room_here != NULL )
			{
				searcher2 += MYLENGTH;
				if (searcher2->y == searcher1->y)
				{
					found = true;
					break;
				}
			}

			if (found)
				y_search = searcher2->y;
			else
			{
				straight = false;
				y_search = searcher1->y;
				height_search = searcher1->y - searcher2->y;
			}

		}

		else // if (searcher1->y < searcher2->y)  // second room is under first one
		{
			goUp = false;
			while ( (searcher1 + 2*MYLENGTH)->room_here != NULL )
			{
				searcher1 += MYLENGTH;
				if (searcher1->y == searcher2->y)
				{
					found = true;
					break;
				}
			}

			if (found)
				y_search = searcher1->y;
			else
			{
				straight = false;
				y_search = searcher1->y;
				height_search = searcher2->y - searcher1->y;
			}
		}

		if (straight)
			make_room("line", field, x_search, y_search, distance, 0, NULL, "horizontal");
		else
		{
			distance1 = (distance >= 4) ? ( (distance >= 8) ? distance - 4 : distance - 1 ) : 3;
			distance2 = (distance >= 4) ? ( (distance >= 8) ? distance - distance1 : distance - 2 ) : 2;

			if (goUp)
			{
				make_room("line", field, x_search, y_search, distance1+1, 0, NULL, "horizontal");
				make_room("line", field, x_search + distance1, y_search - height_search, height_search, 0, NULL, "vertical");
				make_room("line", field, x_search + distance1, y_search - height_search, distance2, 0, NULL, "horizontal");
			}
			else // go down
			{
				make_room("line", field, x_search, y_search, distance1+1, 0, NULL, "horizontal");
				make_room("line", field, x_search + distance1, y_search, height_search, 0, NULL, "vertical");
				make_room("line", field, x_search + distance1, y_search + height_search, distance2, 0, NULL, "horizontal");
			}			
		}
	}

	else // vertical_connection
	{
		y_search = searcher1->y;
		distance = searcher2->y - searcher1->y;
		distance ++;

		if (searcher1->x == searcher2->x)
			x_search = searcher1->x;

		else if (searcher1->x > searcher2->x) // first room is after second one
		{
			goLeft = true;
			while ( (searcher2+2)->room_here != NULL )
			{
				searcher2++;
				if (searcher2->x == searcher1->x)
				{
					found = true;
					break;
				}
			}

			if (found)
				x_search = searcher2->x;
			else
			{
				straight = false;
				x_search = searcher1->x ;
				length_search = searcher1->x - searcher2->x;
			}

		}

		else // if (searcher1->x < searcher2->x)  // second room is after first one
		{
			goLeft = false; // go right
			while ((searcher1+2)->room_here != NULL)
			{
				searcher1++;
				if (searcher1->x == searcher2->x)
				{
					found = true;
					break;
				}
			}

			if (found)
				x_search = searcher1->x;
			else
			{
				straight = false;
				x_search = searcher1->x;
				length_search = searcher2->x - searcher1->x ;
			}
		}

		if (straight)
			make_room("line", field, x_search, y_search, distance, 0, NULL, "vertical");
		else
		{
			distance1 = (distance >= 4) ? ((distance >= 8) ? distance - 4 : distance - 1) : 3;
			distance2 = (distance >= 4) ? ((distance >= 8) ? distance - distance1 : distance - 2) : 2;

			if (goLeft)
			{
				make_room("line", field, x_search, y_search, distance1+1, 0, NULL, "vertical");
				make_room("line", field, x_search - length_search, y_search + distance1, length_search, 0, NULL, "horizontal");
				make_room("line", field, x_search - length_search, y_search + distance1, distance2, 0, NULL, "vertical");
			}
			else // go right
			{
				make_room("line", field, x_search, y_search, distance1+1, 0, NULL, "vertical");
				make_room("line", field, x_search, y_search + distance1, length_search, 0, NULL, "horizontal");
				make_room("line", field, x_search + length_search, y_search + distance1, distance2, 0, NULL, "vertical");
			}
		}
	}
	field = field_0;
}


void BSP_tree::print_number_rooms()
{
	int x, y;
	for (int i = 0; i < nodes_num; i++)
		if (nodes[i]->room != NULL)
			nodes[i]->give_room_number();

	for (int i = 0; i < nodes_num; i++)
	{
		if (nodes[i]->is_leaf)
		{
			x = nodes[i]->room->cornerNW.x + 1;
			y = nodes[i]->room->cornerNW.y + 1;
			gotoxy(x, y);
			printf("%d", nodes[i]->room->room_index);
			y++;
			for (int j = 0; j < nodes[i]->family.size(); j++)
			{
				gotoxy(x, y);
				printf("%d", nodes[i]->family[j]);
				x += 2;
			}			
		}
	}
}


void chceck_sizes(int levelmax, int MIN_SIZE)
{
	if (MYHEIGHT < (MIN_SIZE+1) * pow(2, levelmax) &&
		MYLENGTH < (MIN_SIZE+1) * pow(2, levelmax))
		throw "\nTOO_SMALL FIELD FOR THAT TREE LEVEL\n change CONFIGURATION.h";
}


