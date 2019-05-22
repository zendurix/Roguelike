#pragma once
#include <string>
#include <vector>

class Place;
void gotoxy(int x, int y);

class Character
{
public:

	Place *stays_on;
	int pos_x;
	int pos_y;
	char print_format;
	int hp;
	int atk;
	int view;
	int level;
	std::string name;

	bool moved;
	bool is_ai;
	bool is_alive;




	Character(int x1, int y1, char print_format1, int hp1, int atk1,
		int level, bool is_ai1, std::string name1, Place *field, std::vector<Character*> *enemies);
	~Character();

	void move(char direction);
	void take_dmg(int damage, Place *field);
	void attack(Character *defender, Place *field);
};

