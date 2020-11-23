#pragma once
#include "Ghost.h"
class RedGhost : public Ghost{
	public:
		RedGhost(int x, int y, int xt, int yt);
	protected:
		direction getNextDirection(Tile map[MAP_X_LENGTH][MAP_Y_LENGTH], PlayerCharacter pc) override;
};

