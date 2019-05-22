#include "pch.h"
#include "Room.h"
#include <stdlib.h>
#include <vector>


#include "Place.h"
#include "Character.h"
#include "map_functions.h"


Room::Room()
{
	conected = false;
}


Room::~Room() {}

void Room::make_corner(Place *field, int corner_num)
{
	switch (corner_num)
	{
		case 1:
		{
			cornerNW.corner_ptr = field;
			cornerNW.x = field->x;
			cornerNW.y = field->y;
		}
		case 2:
		{
			cornerNE.corner_ptr = field;
			cornerNE.x = field->x;
			cornerNE.y = field->y;
		}
		case 3:
		{
			cornerSW.corner_ptr = field;
			cornerSW.x = field->x;
			cornerSW.y = field->y;
		}
		case 4:
		{
			cornerSE.corner_ptr = field;
			cornerSE.x = field->x;
			cornerSE.y = field->y;
		}

	}
}
