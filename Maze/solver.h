#pragma once
#include "stdafx.h"
//Sztuczna Inteligencja, LAB1: Maze Solver; Sekcja3 5ION 2022/23

class Solver
{
public:
	Solver();
	~Solver();
	move advance();
	void rotate(directions direction);
	bool facingWall(tiles tile);
private:
	move facing = UP;
	
};