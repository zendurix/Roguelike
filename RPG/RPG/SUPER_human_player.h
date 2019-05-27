#pragma once

#include <iostream>

#include "CONFIGURATION.h"
#include "Character.h"
#include "GAME.h"

class Place;


class SUPER_human_player :
	public Character
{

public:

	using Character::Character;


	void take_dmg(int damage, Place *field);
	void DIE();
	

};

