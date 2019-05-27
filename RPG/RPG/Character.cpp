#include "pch.h"
#include "Character.h"




Character::Character(int x1, int y1, char print_format1, int hp1, int strenght1,
	 int level1, bool is_ai1, std::string name1 ,Place *field, std::vector<Character*> *enemies)
{
	pos_x = x1;
	pos_y = y1;
	print_format = print_format1;
	hp = hp1;
	strenght = strenght1;
	int level = level1;
	is_ai = is_ai1;
	is_alive = true;
	moved = false;
	name = name1;

	view = 10;

	if (!is_ai)
		view = VIEW_TEST;


	field += pos_y * MYLENGTH + pos_x;
	field->go_here(this);
	stays_on = field;
	field -= pos_y * MYLENGTH + pos_x;

	if (is_ai)
		enemies->push_back(this);
}

Character::~Character() {}

void Character::move(char direction)
{
	switch(direction)
	{
		case '8':
		{
			pos_y--;
			break;
		}
		case '9':
		{
			pos_y--;
			pos_x++;
			break;
		}
		case '6':
		{
			pos_x++;
			break;
		}
		case '3':
		{
			pos_y++;
			pos_x++;
			break;
		}
		case '2':
		{
			pos_y++;
			break;
		}
		case '1':
		{
			pos_y++;
			pos_x--;
			break;
		}
		case '4':
		{
			pos_x--;
			break;
		}
		case '7':
		{
			pos_y--;
			pos_x--;
			break;
		}	   
		case '5':    // '5' is for wait one turn
			break;
	}

	if (pos_x < 0)
		pos_x = 0;

	if (pos_y < 0)
		pos_y = 0;

	if (pos_x == MYLENGTH)
		pos_x = MYLENGTH -1;

	if (pos_y == MYHEIGHT)
		pos_y = MYHEIGHT-1;

}

void Character::take_dmg(int damage, Place *field)
{
	hp -= damage;
	if (hp <= 0)
	{
		is_alive = false;
		field += pos_y * MYLENGTH + pos_x;
		field->character_here = NULL;
		field -= pos_y * MYLENGTH + pos_x;
	}
}

void Character::attack(Character *defender, Place *field)
{
	defender->take_dmg(strenght, field);
	gotoxy(0, MYHEIGHT);
	printf("                                          ");
	gotoxy(0, MYHEIGHT);
	std::cout << name << " hit " << defender->name << " for " << strenght << " dmg"<<std::endl;
}
  

