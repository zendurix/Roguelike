#include "pch.h"
#include "SUPER_human_player.h"


using namespace std;

void SUPER_human_player::take_dmg(int damage, Place *field)
{
	hp -= damage;
	if (hp <= 0)
	{
		is_alive = false;
		DIE();
	}
}



void SUPER_human_player::DIE()
{
	gotoxy(0, MYHEIGHT);
	cout << name << " YOU DIED!"<<endl;
	cin.get();
	system("cls");
	gotoxy(MYLENGTH / 2, MYHEIGHT / 2);
	cout << "GAME OVER" << "\n\npress anything to continue";
	cin.get();

}