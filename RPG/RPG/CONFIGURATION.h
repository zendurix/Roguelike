#pragma once


constexpr int MYHEIGHT = 40;   // y axis this is height
constexpr int MYLENGTH = 100;  // x axis

// MAKING TREE (generating random map):
constexpr int MAX_LEVEL = 4;
constexpr int MIN_SIZE = 5;

// how round circles (for view, and rooms will be) optimal is 0.5
constexpr float ROUNDNESS = 0.9f; 

// standard floor tile in rooms
constexpr char STD_FLOOR = '.';

constexpr int VIEW_TEST = 20;

constexpr bool TESTING = true;
/* if TESTING == true
*	- whole level is visible to player
*
*/

// with this on true, printed field will be wider, and more square
constexpr bool PRINT_SPACES = false;


// INFO
/*
*	project started 22.04.2019
*	by Michal Majda
*
*
*/


// CONTROLS
/*
*	- moving.......................numpad
*	- manually update screen.......'U'
*
*
*/
