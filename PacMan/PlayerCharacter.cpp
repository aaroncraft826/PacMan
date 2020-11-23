#include "PlayerCharacter.h"
#include <algorithm>
#include <iostream>

PlayerCharacter::PlayerCharacter(int x, int y, int xt, int yt, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
	xPos = x;
	yPos = y;

	pic.h = TILE_SIZE * 1.5;
	pic.w = TILE_SIZE * 1.5;
	pic.x = x - (TILE_SIZE / 3);
	pic.y = y - (TILE_SIZE / 3);

	xTile = xt;
	yTile = yt;

	color = {r, g, b, a};
}

int PlayerCharacter::getScore() {
	return score;
}

void PlayerCharacter::addScore(int x) {
	score += x;
}

void PlayerCharacter::move(int a, Tile map[MAP_X_LENGTH][MAP_Y_LENGTH]) {
	Tile mainTile = map[xTile][yTile];
	std::set<direction> tileDirections = mainTile.getPossibleDirections();

	if (mainTile.getItem().doesExist() == true) {
		Effect consumedEffect = map[xTile][yTile].consumeItem();
		//std::cout << consumedEffect.getPointValue() << std::endl;
		addScore(consumedEffect.getPointValue());
		pelletEaten = consumedEffect.getNewState() == FRIGHTENED;
	}

	if (tileDirections.find(nextDir) != tileDirections.end() && mainTile.getXPos() == xPos && mainTile.getYPos() == yPos) {//mainTile.getType() == 'i'
		dir = nextDir;
		nextDir = NA;
	}
	else if ((tileDirections.find(dir) != tileDirections.end()) || (mainTile.getXPos() != xPos || mainTile.getYPos() != yPos)) {
		switch (dir) {
		case UP:
			yPos -= a;
			pic.y -= a;
			break;
		case DOWN:
			yPos += a;
			pic.y += a;
			break;
		case LEFT:
			xPos -= a;
			pic.x -= a;
			break;
		case RIGHT:
			xPos += a;
			pic.x += a;
			break;
		}
		updateTile();
	}
	else if (tileDirections.find(nextDir) != tileDirections.end()) {
		dir = nextDir;
		nextDir = NA;
	}
	else {
		dir = NA;
		nextDir = NA;
	}
}

bool PlayerCharacter::pelletHasBeenEaten() {
	if (pelletEaten) {
		pelletEaten = false;
		return true;
	}
	return false;
}