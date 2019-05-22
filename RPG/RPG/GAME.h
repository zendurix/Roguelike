#pragma once
#include <vector>


#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"    // purple
#define ANSI_COLOR_CYAN    "\x1b[36m"    // light blue
#define ANSI_COLOR_RESET   "\x1b[0m"

// for more colors check out ascii() function (in RPG.cpp)

class Character;
class Place;
class AI;

class GAME
{
public:
	GAME();
	~GAME();

	friend void gotoxy(int x, int y);
	void print_field(Place *field, bool clear);
	void print_field_UPDATE(Place *field);
	void move(Character *character, Place *field, char input);
	void ai_turn(std::vector<Character*> *enemies, Character *you, Place *field);
};

