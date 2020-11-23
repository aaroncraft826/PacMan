#include "Character.h"
#include <iostream>

void Character::setXPos(int x) {
	xPos = x;
	pic.x = x - (TILE_SIZE / 3);
	updateTile();
}

void Character::setYPos(int y) {
	yPos = y;
	pic.y = y - (TILE_SIZE / 3);
	updateTile();
}

void Character::setNextDirection(direction d) {
	nextDir = d;
}

void Character::move(int a, Tile map[MAP_X_LENGTH][MAP_Y_LENGTH]) {
	Tile mainTile = map[xTile][yTile];
	std::set<direction> tileDirections = mainTile.getPossibleDirections();

	if (mainTile.getType() == 'i' && tileDirections.find(nextDir) != tileDirections.end() && mainTile.getXPos() == xPos && mainTile.getYPos() == yPos && notBackwards()) {// && mainTile.getXPos() == xPos && mainTile.getXPos() == yPos
		dir = nextDir;
		nextDir = NA; 
		/*std::cout << mainTile.getXPos() << "\t" << xPos << std::endl;
		std::cout << mainTile.getYPos() << "\t" << yPos << "\n" << std::endl;*/
	}else if ((tileDirections.find(dir) != tileDirections.end()) || (mainTile.getXPos() != xPos || mainTile.getYPos() != yPos)) {
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
	else if(tileDirections.find(nextDir) != tileDirections.end()){
		dir = nextDir;
		nextDir = NA;
	}
	else {
		dir = NA;
		nextDir = NA;
	}
}

void Character::updateTile() {
	xTile = xPos / TILE_SIZE;
	yTile = yPos / TILE_SIZE;
}

int Character::getXTile() {
	return xTile;
}

int Character::getYTile() {
	return yTile;
}

SDL_Rect* Character::getPic() {
	return &pic;
}

SDL_Color Character::getColor() {
	return color;
}
direction Character::getDirection() {
	return dir;
}

bool Character::notBackwards() {
	switch (dir) {
		case UP:
			return nextDir == DOWN ? false : true;
			break;
		case DOWN:
			return nextDir == UP ? false : true;
			break;
		case LEFT:
			return nextDir == RIGHT ? false : true;
			break;
		case RIGHT:
			return nextDir == LEFT ? false : true;
			break;
	}
	return true;
}