#include "pch.h"
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <iostream>

#include "CONFIGURATION.h"
#include "GAME.h"
#include "Place.h"
#include "Character.h"
#include "AI.h"
#include "map_functions.h"
#include "Room.h"
#include "RayTracer.h"
#include "BSP_map_gen.h"
#include "RNG.h"



/*
TODO:
- implement node.check_family for each leaf node
- finish making corridors (going up the tree)
- repair making corridors (stop, when hit wall)
- upgrade randomizing rooms (rooms sometimes touch each other)
- clear up memory taken by BSP_tree
- map randomizer - CORRIDORS
- fix draw_shape func in map_rnadomizer.cpp (change square MYHEIGHT, to rectangular MYHEIGHT_length and MYHEIGHT_width)
- separate Character class for user, and ai classes (?)
- ai
- obstacles
- eq, inv
- more stats
}
*/

void some_map(Place *field, int depth);
void ascii();
void colors(bool defines);



int main()
{

	std::this_thread::sleep_for(std::chrono::milliseconds(0));

	//ascii();
	//colors(true);

	char input;

	Place field[MYHEIGHT][MYLENGTH];
	Place *field_ptr = &field[0][0];  // after using this pointer it must be reset to point to &field[0][0] (!!!)
	std::vector <Room*> rooms;
	std::vector <Character*> enemies;


	//some_map(field_ptr, 1);

	// making map with BSP_tree. set to 0, to test other map making methods
	if (1)
	{
		try
		{	// checks if tree can be made with given parameters
			chceck_sizes(MAX_LEVEL, MIN_SIZE);
		}
		catch (const char* msg)
		{
			printf("\nERROR: %s\n", msg);
			return 0;
		}

		BSP_tree tree(MAX_LEVEL, MIN_SIZE);

		int restart_count = 0;
		bool restart = false;
		while (!restart)
		{
			try
			{
				printf("\n TRYING");
				tree.make_full_tree(field_ptr);
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
						field[i][j].RESET();
					}
				rooms.clear();

				printf("\n FAILED, %s", msg);
			}
		}

		//tree.show_tree_details();
		//tree.show_split(field_ptr);
		tree.fill_leaves_with_rooms(field_ptr, &rooms);
		tree.connect_all_rooms(field_ptr);
		printf("\n TREEE \n");
		_getch();
		GAME game;
		game.print_field(field_ptr, true);
		tree.print_number_rooms();

		_getch();
		tree.~BSP_tree();
	}

	Place * ch = NULL; // places hero in random room
	do
	{
		ch = &field[random(0, MYHEIGHT - 1)][random(0, MYLENGTH - 1)];

	} while (!ch->is_in_room);

	Character hero(ch->x, ch->y, '@', 10, 666, 10000, false, "hero", field_ptr, &enemies);

	Character *enemy;
	if(0) // enies in all rooms
	{
		int x, y;
		for (Room *room : rooms)
		{
			x = random(room->cornerNW.x + 1, room->cornerNE.x - 1);
			y = random(room->cornerNE.y + 1, room->cornerSW.y - 1);
			enemy = new Character(x, y, 'G', 3, 2, 1, true, "goblin", field_ptr, &enemies);
		}
	}


	GAME game;

	update_view(field_ptr, &hero);
	game.print_field(field_ptr, true);


	//GAME LOOP
	while (1)
	{
		fflush(stdin);
		input = _getch();
		game.move(&hero, field_ptr, input);
		game.ai_turn(&enemies, &hero, field_ptr);

		//not shure why, but double update_view is working fine
		update_view(field_ptr, &hero);
		update_view(field_ptr, &hero);
		game.print_field_UPDATE(field_ptr);
		
	}

	return 0;
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


void ascii()
{ // prints whole ascii table
	for (int i = 0; i < 256; i++)
	{
		printf("%d  %c \n", i, char(i));
	}
	_getch();
}


void colors(bool defines)
{ // for background colors use 48 instead of 38 in "\u001b[38;5;"
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			int code = i * 16 + j;
			std::string code_str = std::to_string(code);
			std::string coding = "\u001b[38;5;" + code_str + "m ";
			std::string coding2 = "\\u001b[38;5;" + code_str + "m ";
			std::string spaces = "   ";
			if (code == 10)
				spaces = "  ";
			if (code == 100)
				spaces = " ";
			std::cout << code << spaces << coding;
		}
		std::cout << std::endl;

	}
	if (defines)
	{
		for (int i = 0; i < 16; i++)
		{
			for (int j = 0; j < 16; j++)
			{
				int code = i * 16 + j;
				std::string code_str = std::to_string(code);
				std::string coding = "\u001b[38;5;" + code_str + "m ";
				std::string coding2 = "\\u001b[38;5;" + code_str + "m ";

				std::cout << "#define ANSI_COLOR_" << code << " " << coding2 << coding << std::endl;
			}
		}
	}
	_getch();
}

