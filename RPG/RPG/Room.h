#pragma once
#include <vector>
class Place;
class Character;


class Room
{
public:
	typedef struct
	{
		int x;
		int y;
		Place *corner_ptr;
	}Corner;

	Corner cornerNW; //1			   1---------------2
	Corner cornerNE; //2    		   |    ROOM       |
	Corner cornerSW; //3			   |    inside     |
	Corner cornerSE; //4			   3---------------4

	int room_index;

	std::vector <Place*> in_room;
	std::vector <Place*> walls;
	bool conected;

	Room();
	~Room();
	
	void make_corner(Place *field, int corner_num);
};

