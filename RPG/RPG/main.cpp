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
#include "User_Classes.h"
#include "AI.h"
#include "map_functions.h"
#include "Room.h"
#include "RayTracer.h"
#include "BSP_map_gen.h"
#include "RNG.h"
#include "ITEM.h"


void some_map(Place *field, int depth);
void ascii();
void colors(bool defines);
void sleep_miliseconds(int miliseconds);



int main()
{
	char input;

	Place field[MYHEIGHT][MYLENGTH];
	Place *field_ptr = &field[0][0];  // after using this pointer it must be reset to point to &field[0][0] (!!!)
	std::vector <Room*> rooms;
	std::vector <Character*> enemies;
	
	//some_map(field_ptr, 1);
		
	if (!make_random_map_BSP(field_ptr, &rooms))
		return 0;

	Place * ch = NULL; // places hero in random room
	do
	{
		ch = &field[random(0, MYHEIGHT - 1)][random(0, MYLENGTH - 1)];

	} while (!ch->is_in_room);

	Knight hero(ch->x, ch->y, "hero", 'M', field_ptr);

	Character *enemy;
	if(1) // enemies in all rooms
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
			std::cout << code -1<< spaces << coding;
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


void sleep_miliseconds(int miliseconds)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(miliseconds));
}