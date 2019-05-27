#include "pch.h"
#include "User_Classes.h"



Knight::Knight(int x, int y, std::string name, char gender1, Place *field)
	: SUPER_human_player(x, y, '@', 20, 4, 1, false, name,field, NULL)
{
	view = VIEW_TEST;
	gender = gender1;

}