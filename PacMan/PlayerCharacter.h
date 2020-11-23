#pragma once
#include "Character.h"
class PlayerCharacter: public Character{
	public:
		PlayerCharacter(int x, int y, int xt, int yt, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
		int getScore();
		void addScore(int x);
		void move(int a, Tile map[MAP_X_LENGTH][MAP_Y_LENGTH]) override;

		bool pelletHasBeenEaten();
	private:
		int score = 0;
		bool pelletEaten = false;
};

