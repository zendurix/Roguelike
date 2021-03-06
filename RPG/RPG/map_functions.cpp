#include "pch.h"
#include "map_functions.h"



bool make_random_map_BSP(Place *field, std::vector<Room*> *rooms)
{
	Place *field_0 = field;
	try
	{
		chceck_sizes(MAX_LEVEL, MIN_SIZE);
	}
	catch (const char* msg)
	{
		printf("\nERROR: %s\n", msg);
		_getch();
		return false;
	}

	BSP_tree tree(MAX_LEVEL, MIN_SIZE);

	int restart_count = 0;
	bool restart = false;
	while (!restart)
	{
		try
		{
			tree.make_full_tree(field);
			restart = true;
			printf("\n SUCCES  RESTART_COUNT: %d", restart_count);
		}
		catch (const char* msg)
		{
			restart = false;
			restart_count++;
			for (int i = 0; i < MYHEIGHT; i++)
				for (int j = 0; j < MYLENGTH; j++)
				{
					field->RESET();
					field++;
				}
			rooms->clear();
			field = field_0;
		}
	}

	//tree.show_tree_details();
	//tree.TEST_show_split(field_ptr);
	tree.fill_leaves_with_rooms(field, rooms);
	tree.connect_all_rooms(field);
	printf("\n TREEE \n");
	_getch();
	GAME game;
	game.print_field(field, true);
	//tree.print_number_rooms();

	_getch();
	tree.~BSP_tree();
	field = field_0;
	return true;
}

void some_map(Place *field, int depth)
{
	Place *field_0 = field;
	Place *field_next_pos = field;
	std::vector <Room> rooms;

	for (int i = 0; i < MYHEIGHT; i++)
		for (int j = 0; j < MYLENGTH; j++, field++)
			field->make_obstacle();   //make whole field obstacle
	field = field_0;

	Room room;
	Room room2;
	Room room3;

	make_room("rectangle", field, 2, 2, 80, 30, &room);

	make_room("circle", field, 27, 10, 5, 0, &room3);
	make_room("line", field, 19, 13, 5, 0, NULL);

	make_room("rectangle", field, 13, 6, 6, 6, &room2);
	draw_shape("line", field, 4, 12, 6);
	draw_shape("line", field, 7, 7, 1);
	draw_shape("line", field, 5, 5, 1);
	draw_shape("line", field, 8, 5, 1);
	draw_shape("line", field, 9, 3, 1);
	draw_shape("line", field, 4, 8, 6, 0, "vertical");
	make_room("line", field, 12, 8, 5, 0, NULL);



	field = field_0;
}

void draw_shape(std::string shape_name, Place *field, int x, int y,
	int length1, int length2, std::string orientation, bool filled)
{
	Place *field_0 = field;
	field += y * MYLENGTH + x;

	if (shape_name == "line")
	{
		for (int i = 0; i < length1; i++)
		{
			field->make_obstacle();
			if (orientation == "horizontal")
				field++;
			else if (orientation == "vertical")
				field += MYLENGTH;
		}
	}

	else if (shape_name == "square") ////////////// REPAIR THIS MYHEIGHT PROBLEM
	{
		if (!filled)
		{
			//up
			for (int i = 0; i < length1; i++)
			{
				field->make_obstacle();
				field++;
			}
			field = field_0 + y * MYLENGTH + x;
			field += length1 * MYLENGTH;

			//down
			for (int i = 0; i < length1; i++)
			{
				field->make_obstacle();
				field++;
			}
			field = field_0 + y * MYHEIGHT + x;

			//left
			for (int i = 0; i < length1; i++)
			{
				field->make_obstacle();
				field += MYLENGTH;
			}
			field = field_0 + y * MYHEIGHT + x;
			field += length1;

			//right
			for (int i = 0; i <= length1; i++)
			{
				field->make_obstacle();
				field += MYLENGTH;
			}
		}
		else
		{
			for (int i = 0; i < length1; i++)
			{
				for (int j = 0; j < length1; j++)
				{
					field->make_obstacle();
					field++;
				}
				field += MYLENGTH - length1;
			}
		}

	}

	else if (shape_name == "circle")
	{
		field = field_0;
		Place *center = field_0 + y * MYLENGTH + x;
		for (int i = 0; i < MYHEIGHT; i++)
			for (int j = 0; j < MYLENGTH; j++, field++)
				if (filled)
				{
					if ((int)(distance_meter(*center, *field) + ROUNDNESS) <= length1)
						field->make_obstacle();
				}
				else
				{
					if ((int)(distance_meter(*center, *field) + ROUNDNESS) == length1)
						field->make_obstacle();
				}
	}

	field = field_0;
}


void make_room(std::string shape_name, Place *field, int x, int y, int length1,
	int length2, Room *room, std::string orientation, bool bsp_room)
{
	Place *field_0 = field;
	if (!bsp_room)
		field += y * MYLENGTH + x;

	// this is for making corridors, when used use *room = NULL in constructor
	if (shape_name == "line")
	{
		for (int i = 0; i < length1; i++)
		{
			field->unmake_obstacle();
			field->is_in_room = true;
			if (orientation == "horizontal")
				field++;
			else if (orientation == "vertical")
				field += MYLENGTH;
		}
	}

	else if (shape_name == "circle")
	{
		field = field_0;
		Place *center = field_0 + y * MYLENGTH + x;
		for (int i = 0; i < MYHEIGHT; i++)
			for (int j = 0; j < MYLENGTH; j++, field++)
			{
				if ((int)(distance_meter(*center, *field) + ROUNDNESS) <= length1)
				{
					field->unmake_obstacle();
					field->is_in_room = true;
				}
				if ((int)(distance_meter(*center, *field) + ROUNDNESS) == length1 + 1 ||
					(int)(distance_meter(*center, *field) + ROUNDNESS) == length1)
				{
					field->is_wall = true;
					field->make_obstacle();
				}
			}

	}

	// THIS is main room making options
	else if (shape_name == "rectangle")
	{
		//MAKE WALLS
		field -= MYLENGTH;
		field--;
		// upper wall
		for (int i = 0; i < length1 + 2; i++)
		{
			field->is_wall = true;
			field->print_format = 196;
			field->prev_print_format = 196;
			field->room_here = room;
			room->walls.push_back(field);
			if (i == 0)
			{
				field->print_format = 218;
				field->prev_print_format = 218;
				room->make_corner(field, 1);
			}
			if (i == length1 + 1)
			{
				field->print_format = 191;
				field->prev_print_format = 191;
				room->make_corner(field, 2);
			}
			field->is_obstacle = true;
			field++;
		}
		field = field_0 + y * MYLENGTH + x - 1;
		field += length2 * (MYLENGTH);

		// down wall
		for (int i = 0; i < length1 + 2; i++)
		{
			field->is_wall = true;
			field->print_format = char(196);
			field->prev_print_format = 196;
			field->room_here = room;
			room->walls.push_back(field);
			if (i == 0)
			{
				field->print_format = 192;
				field->prev_print_format = 192;
				room->make_corner(field, 3);
			}
			if (i == length1 + 1)
			{
				field->print_format = 217;
				field->prev_print_format = 217;
				room->make_corner(field, 4);
			}
			field->is_obstacle = true;
			field++;
		}
		field = field_0 + y * MYLENGTH + x - 1;

		// left wall
		for (int i = 0; i < length2; i++)
		{
			field->is_wall = true;
			field->print_format = 179;
			field->prev_print_format = 179;
			field->is_obstacle = true;
			field->room_here = room;
			room->walls.push_back(field);
			field += MYLENGTH;
		}
		field = field_0 + y * MYLENGTH + x + 1;
		field += length1 - 1;

		//right wall
		for (int i = 0; i < length2; i++)
		{
			field->is_wall = true;
			field->print_format = 179;
			field->prev_print_format = 179;
			field->is_obstacle = true;
			field->room_here = room;
			room->walls.push_back(field);
			field += MYLENGTH;
		}
		field = field_0 + y * MYLENGTH + x;

		// MAKE ROOM INSIDE
		for (int i = 0; i < length2; i++)
		{
			for (int j = 0; j < length1; j++)
			{
				field->unmake_obstacle();
				field->is_in_room = true;
				field->room_here = room;
				field++;
			}
			field += MYLENGTH - length1;
		}

	}

	field = field_0;
}


double distance_meter(Place p1, Place p2)
{
	return sqrt(pow(p2.x - p1.x, 2.0) + pow(p2.y - p1.y, 2.0));
}


void make_whole_map_obstacle(Place *field)
{
	Place *field_0 = field;
	for (int i = 0; i < MYHEIGHT; i++)
		for (int j = 0; j < MYLENGTH; j++, field++)
			field->make_obstacle();
	field = field_0;
}


