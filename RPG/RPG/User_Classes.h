#pragma once
#include <string>

#include "CONFIGURATION.h"
#include "SUPER_human_player.h"




class Knight 
	: public SUPER_human_player
{

public:

	char gender;
	


	Knight(int x, int y, std::string name, char gender1, Place *field);
};

