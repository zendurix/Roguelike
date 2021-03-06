#pragma once
#include <vector>

#include "CONFIGURATION.h"
#include "BSP_map_gen.h"
#include "Place.h"
#include "Room.h"

class BSP_tree;
class Place;
class Room;


bool make_random_map_BSP(Place *field, std::vector<Room*> *rooms);

void some_map(Place *field, int depth);

void draw_shape(std::string shape_name, Place *field, int x, int y,
	int length1, int length2 = 0, std::string orientation = "horizontal", bool filled = false);

void make_room(std::string shape_name, Place *field, int x, int y, int length1, int length2,
	Room *room, std::string orientation = "horizontal", bool bsp_room = false);

double distance_meter(Place p1, Place p2);

void make_whole_map_obstacle(Place *field);