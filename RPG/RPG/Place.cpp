#include "pch.h"
#include "Place.h"


int Place:: count_x = 0;
int Place:: count_y = 0;

Place::Place()
{
	x = count_x;
	y = count_y;

	print_format = STD_FLOOR;
	prev_print_format = STD_FLOOR;
	is_obstacle = false;
	is_in_room = false;
	is_wall = false;
	is_doors = false;
	changed_print = false;

	character_here = NULL;
	room_here = NULL;

	view_circle = false;
	visible = false;
	was_seen = false;

	count_x++;
	if (count_x == MYLENGTH)
	{
		count_x = 0;
		count_y++;
	}

}


Place::~Place(){}


void Place::print_place()
{
	//if (view_circle)
	//	printf(ANSI_COLOR_GREEN "%c" ANSI_COLOR_RESET, print_format);

	if (TESTING)
		printf("%c", print_format);

	else
	{
		if (visible)
		{
			if (character_here != NULL && !character_here->is_ai)
				printf(ANSI_COLOR_GREEN "%c" ANSI_COLOR_RESET, print_format);
			else if (character_here != NULL && character_here->is_ai)
				printf(ANSI_COLOR_RED "%c" ANSI_COLOR_RESET, print_format);
			else
			{
				//if (is_wall)
					//printf(ANSI_COLOR_MAGENTA "%c" ANSI_COLOR_RESET, print_format);
				//else if (is_obstacle)
				//	printf(ANSI_GRAY_BACKGROUND "%c" ANSI_COLOR_RESET, print_format);

				//else
					printf("%c", print_format);
			}
		}
		else if (was_seen)
			printf(ANSI_COLOR_GREY "%c" ANSI_COLOR_RESET, prev_print_format);

		else
			printf(ANSI_COLOR_CYAN " " ANSI_COLOR_RESET); // , print_format);

	}



	// if this space is deleted, gotoxy function in GAME.cpp must be changed also
	if (PRINT_SPACES)
		printf(" ");  // space in x axis, for more square map
	if (x == MYLENGTH - 1)
		printf("\n");
}

void Place::go_here(Character* character)
{
	character_here = character;
	print_format = character_here->print_format;
}

void Place::leave_here()
{
	character_here = NULL;
	print_format = prev_print_format;
}

void Place::make_obstacle()
{
	is_obstacle = true;
	print_format = '#';
	prev_print_format = '#';
}

void Place::unmake_obstacle()
{
	is_obstacle = false;
	print_format = STD_FLOOR;
	prev_print_format = STD_FLOOR;
	is_in_room = true;
}

void Place::RESET()
{// restart field, to begining, clean state
	print_format = STD_FLOOR;
	prev_print_format = STD_FLOOR;
	is_obstacle = false;
	is_in_room = false;
	is_wall = false;
	is_doors = false;
	changed_print = false;

	character_here = NULL;
	room_here = NULL;

	view_circle = false;
	visible = false;
	was_seen = false;
}