#pragma once

#include <vector>
#include <cmath>

#include "CONFIGURATION.h"
#include "Place.h"
#include "Character.h"

#include "map_functions.h" 

class Place;
class Character;


bool AreSame(double a, double b);
void update_view(Place* field, Character *character);
void draw_view_circle(Place *field, Character* character, std::vector <Place*> *circle, int& size);
bool trace(Place* field, Place* p1, Place* p2, int view);
