#include "stdafx.h"
//Sztuczna Inteligencja, LAB1: Maze Solver; Sekcja3 5ION 2022/23
#include "Solver.h"

Solver::Solver(){

}

Solver::~Solver(){

}

move Solver::advance() {
	return facing;
};
void Solver::rotate(directions direction){
	if (direction == movLEFT) {
		switch (facing)
		{
		case LEFT: facing = DOWN;
			break;
		case RIGHT: facing = UP;
			break;
		case DOWN: facing = RIGHT;
			break;
		case UP: facing = LEFT;
			break;
		case ESC: std::cout << '\a\a';
			break;
		default: std::cout << '\a';
			break;
		}
	}
};
bool Solver::facingWall(tiles tile) {
	std::cout << "\nTried to move to " << tile << "is it wall ?";
	if (tile = WALL) {
		std::cout << "true";
		return true;
	};
	std::cout << "false";
	return false;
};