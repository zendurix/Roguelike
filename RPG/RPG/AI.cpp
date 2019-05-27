#include "pch.h"
#include "AI.h"
#include <math.h>  
#include <algorithm>

#include "CONFIGURATION.h"
#include "Character.h"
#include "Place.h"
#include "GAME.h"


AI::AI() {}
AI::~AI() {}

int AI::distance_meter(Character char1, Character char2)
{
	return (int)sqrt(pow(char2.pos_x - char1.pos_x, 2.0) + pow(char2.pos_y - char1.pos_y, 2.0));
}

void AI::do_smth(Character *ai, Character *you, Place *field)
{
	if (ai->stays_on->is_obstacle)
	{
		ai->hp = 0;
		return;

	}
	int distance = distance_meter(*ai, *you);
	if (distance == 1)
		ai->attack(you, field);
	else
		move_to_target(ai, you, field);
}

void AI::move_to_target(Character *ai, Character *you, Place *field)
{
	Place * field_0 = field;
	GAME game;
	int timer = 0;

	field += (ai->pos_y)*MYLENGTH + (ai->pos_x);
	field->leave_here();

	// [direction, distance]
	int directions[8] = { 8, 9, 6, 3, 2, 1, 4, 7 };
	int distance[8] =   { 0, 0, 0, 0, 0, 0, 0, 0 };

	
	ai->pos_y -= 1;
	distance[0] = distance_meter(*ai, *you);
	ai->pos_y += 1;

	ai->pos_y += 1;
	distance[4] = distance_meter(*ai, *you);
	ai->pos_y -= 1;

	ai->pos_x -= 1;
	distance[6] = distance_meter(*ai, *you);
	ai->pos_x += 1;

	ai->pos_x += 1;
	distance[2] = distance_meter(*ai, *you);
	ai->pos_x -= 1;

	ai->pos_y -= 1;
	ai->pos_x += 1;
	distance[1] = distance_meter(*ai, *you);
	ai->pos_y += 1;
	ai->pos_x -= 1;

	ai->pos_y += 1;
	ai->pos_x += 1;
	distance[3] = distance_meter(*ai, *you);
	ai->pos_y -= 1;
	ai->pos_x -= 1;

	ai->pos_y += 1;
	ai->pos_x -= 1;
	distance[5] = distance_meter(*ai, *you);
	ai->pos_y -= 1;
	ai->pos_x += 1;

	ai->pos_y -= 1;
	ai->pos_x -= 1;
	distance[7] = distance_meter(*ai, *you);
	ai->pos_y += 1;
	ai->pos_x += 1;


	ai->moved = false;
	int best = 0;
	while (1)
	{
		if (timer > 20)
			break;
		if (ai->moved)
			break;
		if (timer > 0 && !ai->moved)
			distance[best] = 999999;
		for (int i = 0; i < 8; i++)
		{
			if ( distance[i] == *std::min_element(distance, distance + 8) )
			{
				best = i;
				break;
			}
		}		
		game.move(ai, field_0, char(directions[best] + 48));
		timer++;
	}
}



