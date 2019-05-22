#pragma once

class Character;
class Place;
class GAME;

class AI
{
public:

	AI();
	~AI();

	int distance_meter(Character char1, Character char2);
	void do_smth(Character *ai, Character *you, Place *field);
	void move_to_target(Character *ai, Character *you, Place *field);

};

