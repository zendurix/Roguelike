#include "pch.h"
#include "RayTracer.h"

const double ACCURACY = 0.01;  //0.1 seems to be best for now
const double EPSILON = ACCURACY / 10; 



bool AreSame(double a, double b)
{ // function to compare double
	return fabs(a - b) < EPSILON;
}

void update_view(Place* field, Character *character)
{
	Place* field_0 = field;
	Place* p1 = field_0 + character->pos_y * MYLENGTH + character->pos_x;

	std::vector <Place*> view_circle;
	int size = 0;

	for (int i = 0; i < MYHEIGHT; i++)
		for (int j = 0; j < MYLENGTH; j++, field++)
		{

			/*
			if (field->view_circle)
			{
				field->view_circle = false;
				field->changed_print = true;
			}
			*/
			

			if (field->visible)
			{
				field->visible = false;
				field->changed_print = true;
			}
		}

	field = field_0;

	draw_view_circle(field, character, &view_circle, size);

	for (int i = 0; i < size; i++)
		trace(field, p1, view_circle[i], character->view);

	field = field_0;
}

void draw_view_circle(Place *field, Character* character, std::vector <Place*> *circle, int& size)
{
	Place* field_0 = field;
	Place *center = field_0 + character->pos_y * MYLENGTH + character->pos_x;
	int radius = character->view;

	for (int i = 0; i < MYHEIGHT; i++)
		for (int j = 0; j < MYLENGTH; j++, field++)
			if ((int)(distance_meter(*center, *field) + ROUNDNESS) == radius)
			{
				circle->push_back(field);
				size++;
				field->view_circle = true;
				field->changed_print;
			}
			else if ((int)(distance_meter(*center, *field) + ROUNDNESS) < radius &&
				(i == 0 || i == MYHEIGHT - 1 || j == 0 || j == MYLENGTH - 1))
			{
				circle->push_back(field);
				size++;
				field->view_circle = true;
				field->changed_print;
			}

	field = field_0;
}

bool trace(Place* field, Place* p1, Place* p2, int view)
{
	Place* field_0 = field;
	bool found_obstacle = false;
	double distance = distance_meter(*p1, *p2);
	if ((int)distance > view)
	{
		p2->visible = false;
		return false;
	}

	// TRIANGLE
	double a = abs(p1->x - p2->x);
	double b = abs(p1->y - p2->y);
	double c = sqrt(pow(a, 2) + pow(b, 2));
	double alpha = atan(b / a);

	double a_check = 0.1; //////////WOW 0.1
	double b_check;
	int b_int;

	field = p1;
	int count = 0; 

	if (p1->x == p2->x)
	{//straight vertical line
		while (count < view)
		{ 
			if (p1->y <= p2->y)
			{
				if (field->y == MYHEIGHT - 1)
				{
					field->visible = false;
					field->changed_print = true;
					return false;
				}
				else
					field += MYLENGTH;
			}
			else
			{

				if (field->y == 0)
				{
					field->visible = false;
					field->changed_print = true;
					return false;
				}
				else
					field -= MYLENGTH;
			}

			if (field->is_obstacle) 
				if (!found_obstacle)
				{
					found_obstacle = true;
					field->visible = true;
					field->was_seen = true;
					field->changed_print = true;

				}
			if (!found_obstacle)
			{
				field->visible = true;
				field->was_seen = true;
				field->changed_print = true;

			}

			count++;
		}

		field = field_0;
		return false;
	}

	// non straight line
	while(a_check <= a)
	{
		b_check = tan(alpha) * a_check;
		b_int = (int)b_check;
		
		if (p1->y <= p2->y)
			field += b_int * MYLENGTH;
		else
			field -= b_int * MYLENGTH;

		if (field->is_obstacle)
			if (!found_obstacle)
			{
				found_obstacle = true;
				field->visible = true;
				field->was_seen = true;
				field->changed_print = true;
			}
		if(!found_obstacle)
		{
			field->visible = true;
			field->was_seen = true;
			field->changed_print = true;
		}
		
		if (p1->y <= p2->y)
			field -= b_int * MYLENGTH;
		else
			field += b_int * MYLENGTH;

		if (AreSame(a_check, round(a_check)))
		{
			if (p1->x <= p2->x)
				field++;
			else
				field--;
		}

		a_check += ACCURACY;
	}

	field = field_0;
	return false;
}

