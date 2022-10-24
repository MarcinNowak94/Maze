#pragma once
#include "stdafx.h"

enum move {
	LEFT = 'a',
	RIGHT = 'd',
	DOWN = 's',
	UP = 'w',
	ESC = 27 /*ASCII for ESC*/
};

//enum arrows{	LEFT = 75,	//Arrow left
//			RIGHT= 77,	//Arrow right
//			DOWN = 80,	//Arrow down
//			UP	 = 72,	//Arrow up
//			ESC  = 27 /*ASCII for ESC*/};

enum tiles {
	HERO = 'H',
	WALL = '#',
	CORRIDOR = '.',
	EXIT = 'O'
};

enum directions {
	movLEFT = 1,
	movRIGHT = -1
};