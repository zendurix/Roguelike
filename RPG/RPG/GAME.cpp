#include "pch.h"
#include "GAME.h"
#include <stdlib.h>
#include <vector>
#include <windows.h>


#include "Place.h"
#include "CONFIGURATION.h"
#include "Character.h"
#include "AI.h"



GAME::GAME() {}
GAME::~GAME() {}


void gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void GAME::print_field(Place *field, bool clear)
{
	Place *field_0 = field;
	if (clear)
		(system("cls"));
	for (int i = 0; i < MYHEIGHT; i++)
		for (int j = 0; j < MYLENGTH; j++)
		{
			field->print_place();
			field++;
		}
	field = field_0;
}

void GAME::print_field_UPDATE(Place *field)
{
	Place *field_0 = field;
	for (int i = 0; i < MYHEIGHT; i++)
		for (int j = 0; j < MYLENGTH; j++)
		{
			if (field->changed_print)
			{
				// if space in print_field in Field.cpp is deleted, than delete *2    // gotoxy(field->x *2 , field->y);
				if (PRINT_SPACES)
					gotoxy(field->x * 2, field->y);
				else
					gotoxy(field->x  , field->y);

				field->print_place();
				field->changed_print = false;
			}
			field++;
		}
	gotoxy(0, 0);
	field = field_0;
}

void GAME::move(Character *character, Place *field, char input)
{
	character->moved = false;
	Place *field_0 = field;
	Place * field_1 = field + (character->pos_y)*MYLENGTH + (character->pos_x);
	if (input >= '1' && input <= '9')
	{
		field_1->leave_here();
		field_1->changed_print = true;
		int x_prev = character->pos_x;
		int y_prev = character->pos_y;

		character->move(input);

		field += (character->pos_y)*MYLENGTH + (character->pos_x);
		if (field->character_here == NULL && !field->is_obstacle)
		{
			field->go_here(character);
			character->stays_on = field;
			field->changed_print = true;
			character->moved = true;
		}		
		else 
		{
			character->pos_x = x_prev;
			character->pos_y = y_prev;
			field_1->go_here(character);

			if (field->character_here)
			{
				if (!character->is_ai)
				{
					field_1->character_here->attack(field->character_here, field_0);
					field->changed_print = true;
				}
			}
		}
	}

	else
	{
		if (input == 'U')
			print_field(field, true);

	}

	field = field_0;
}

void GAME::ai_turn(std::vector<Character*> *enemies, Character *you, Place *field)
{
	AI ai;
	for (Character *enemy : *enemies)
		if (enemy->is_alive)
			ai.do_smth(enemy, you, field);

}