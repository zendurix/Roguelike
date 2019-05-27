#pragma once

#include <string>




class ITEM
{



public:
	std::string name;
	std::string type;
	std::string material;


	int hp;
	int weight;
	int value;





	ITEM();
	~ITEM();
};

