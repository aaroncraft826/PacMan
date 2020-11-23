#pragma once
#include "Character.h"
#include <set>
#include <queue>
#include "PlayerCharacter.h"

class Ghost : public Character {
	public:
		Ghost(int x, int y, int xt, int yt, int xs, int ys, int dt, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
		void move(int a, Tile map[MAP_X_LENGTH][MAP_Y_LENGTH], PlayerCharacter pc);
		void setState(state s);
		state getState();
		void updateDot();
	protected:
		direction getNextDirection(Tile map[MAP_X_LENGTH][MAP_Y_LENGTH], PlayerCharacter pc);

		direction euclidianDirection(int xt, int yt, Tile map[MAP_X_LENGTH][MAP_Y_LENGTH], bool getShortest);
		bool isOppositeDirection(direction d1, direction d2);

		state crntState = REST;

		int scatterX = 0;
		int scatterY = 0;
		int dotTimer = 0;
};