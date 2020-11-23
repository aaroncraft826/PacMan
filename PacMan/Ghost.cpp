#include "Ghost.h"
#include <iostream>
#include <utility>
#include <queue>
#include <cmath>

Ghost::Ghost(int x, int y, int xt, int yt, int xs, int ys, int dt, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
	xPos = x;
	yPos = y;

	pic.h = TILE_SIZE * 1.5;
	pic.w = TILE_SIZE * 1.5;
	pic.x = x - (TILE_SIZE / 3);
	pic.y = y - (TILE_SIZE / 3);

	xTile = xt;
	yTile = yt;

	scatterX = xs;
	scatterY = ys;

	color = { r, g, b, a };
	dotTimer = dt;
}

void Ghost::move(int a, Tile map[MAP_X_LENGTH][MAP_Y_LENGTH], PlayerCharacter pc) {
	Tile mainTile = map[xTile][yTile];
	std::set<direction> tileDirections = mainTile.getPossibleDirections();
	if (mainTile.getType() == 'i' && mainTile.getXPos() == xPos && mainTile.getYPos() == yPos) {
		dir = getNextDirection(map, pc);
		//std::cout << dir << std::endl;
	}

	if ((tileDirections.find(dir) != tileDirections.end()) || (mainTile.getXPos() != xPos || mainTile.getYPos() != yPos)) {//
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
	else {
		for (direction d : tileDirections) {
			nextDir = d;
			if (notBackwards()) {
				break;
			}
		}
		dir = nextDir;
		nextDir = NA;
	}
}

void Ghost::setState(state s) {
	crntState = s;
}

direction Ghost::getNextDirection(Tile map[MAP_X_LENGTH][MAP_Y_LENGTH], PlayerCharacter pc) {
	switch (crntState) {
		case CHASE:
			return euclidianDirection(pc.getXTile(), pc.getYTile(), map, true);
			break;
		case SCATTER:
			return euclidianDirection(scatterX, scatterY, map, true);
			break;
		case FRIGHTENED:
			return euclidianDirection(pc.getXTile(), pc.getYTile(), map, false);
			break;
		case DEPLOY:
			if ((xPos == DEPLOY_X*TILE_SIZE) && (yPos == DEPLOY_Y * TILE_SIZE)) {
				crntState = SCATTER;
				return euclidianDirection(scatterX, scatterY, map, true);
			}
			return euclidianDirection(DEPLOY_X, DEPLOY_Y, map, true);
		case RETREAT:
			if ((xPos == DEPLOY_X * TILE_SIZE) && (yPos == DEPLOY_Y * TILE_SIZE)) {
				crntState = SCATTER;
				/*setXPos(14 * TILE_SIZE);
				setYPos(17 * TILE_SIZE);*/
				return euclidianDirection(scatterX, scatterY, map, true);
			}
			return euclidianDirection(DEPLOY_X, DEPLOY_Y, map, true);
		default:
			return NA;
			break;
	}
}

direction Ghost::euclidianDirection(int xt, int yt, Tile map[MAP_X_LENGTH][MAP_Y_LENGTH], bool getShortest) {
	direction output = NA;
	Tile mainTile = map[xTile][yTile];
	int dsquare;
	if (getShortest == true) {
		dsquare = 2147483646;
	}
	else {
		dsquare = -1;
	}
	std::set<direction> dirSet = mainTile.getPossibleDirections();
	for (direction d : dirSet) {
		if (isOppositeDirection(dir, d) == false) {
			direction newDirection;
			int newXTile;
			int newYTile;
			switch (d) {
				case UP:
					newDirection = UP;
					newXTile = xTile;
					newYTile = yTile - 1;
					break;
				case DOWN:
					newDirection = DOWN;
					newXTile = xTile;
					newYTile = yTile + 1;
					break;
				case LEFT:
					newDirection = LEFT;
					newXTile = xTile - 1;
					newYTile = yTile;
					break;
				case RIGHT:
					newDirection = RIGHT;
					newXTile = xTile + 1;
					newYTile = yTile;
					break;
			}
			float newdsquare = std::pow((newXTile - xt), 2) + std::pow((newYTile - yt), 2);
			if ((newdsquare < dsquare && getShortest == true) || (newdsquare > dsquare && getShortest == false)) {
				dsquare = newdsquare;
				output = newDirection;
			}
		}
	}
	return output;
}

struct triplet {
	Tile t;
	int c;
	direction d;
	bool operator<(const triplet& p1) {
		return c < p1.c;
	}
};

bool Ghost::isOppositeDirection(direction d1, direction d2) {
	if ((d1 == UP && d2 == DOWN) || (d1 == DOWN && d2 == UP) || (d1 == LEFT && d2 == RIGHT) || (d1 == RIGHT && d2 == LEFT)) {
		return true;
	}
	else {
		return false;
	}
}

state Ghost::getState() {
	return crntState;
}

void Ghost::updateDot() {
	dotTimer--;
	if (dotTimer <= 0 && crntState == REST) {
		crntState = DEPLOY;
	}
}