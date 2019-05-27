#pragma once
#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>

#include "CONFIGURATION.h"
#include "Place.h"
#include "GAME.h"
#include "ITEM.h"

class Place;
class ITEM;
void gotoxy(int x, int y);

class Character
{
public:

	Place *stays_on;
	int pos_x;
	int pos_y;
	char print_format;
	int hp;
	int strenght;
	int view;
	int level;
	std::string name;

	bool moved;
	bool is_ai;
	bool is_alive;




	Character(int x1, int y1, char print_format1, int hp1, int strenght1,
		int level, bool is_ai1, std::string name1, Place *field, std::vector<Character*> *enemies);
	~Character();

	void move(char direction);
	virtual void take_dmg(int damage, Place *field);
	void attack(Character *defender, Place *field);
};

