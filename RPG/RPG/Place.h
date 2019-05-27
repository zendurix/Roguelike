#pragma once		

#include <stdio.h>
#include <cstdlib>

#include "CONFIGURATION.h"
#include "Character.h"
#include "room.h"

#define ANSI_COLOR_RED     "\u001b[38;5;9m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_GREY    "\u001b[38;5;246m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define ANSI_GRAY_BACKGROUND "\u001b[48;5;241m"

class Character;
class Room;

class Place
{
public:
	static int count_x, count_y;

	int x;
	int y;
	unsigned char print_format;
	unsigned char prev_print_format;
	bool is_obstacle;
	bool is_in_room;
	bool is_wall;
	bool is_doors;

	bool changed_print;
	bool visible;
	bool was_seen;
	bool view_circle;

	Character *character_here;
	Room *room_here;
	

	Place();
	~Place();

	void print_place();
	void go_here(Character* character);
	void leave_here();
	void make_obstacle();
	void unmake_obstacle();
	void RESET();
};

