#include "RedGhost.h"

RedGhost::RedGhost(int x, int y, int xt, int yt) {
	xPos = x;
	yPos = y;

	pic.h = TILE_SIZE * 1.5;
	pic.w = TILE_SIZE * 1.5;
	pic.x = x - (TILE_SIZE / 3);
	pic.y = y - (TILE_SIZE / 3);

	xTile = xt;
	yTile = yt;
}

direction RedGhost::getNextDirection(Tile map[MAP_X_LENGTH][MAP_Y_LENGTH], PlayerCharacter pc) {
	switch (crntState) {
		case CHASE:
			return euclidianDirection(pc.getXTile(), pc.getYTile(), map);//shortestDirection(pc.getXTile(), pc.getYTile(), map);//PROBLEM CHILD
			break;
		case SCATTER:
			return euclidianDirection(MAP_X_LENGTH - 1, 0, map);//shortestDirection(pc.getXTile(), pc.getYTile(), map);//PROBLEM CHILD
			break;
		case FRIGHTENED:
			break;
		default:
			dir = NA;
			break;
		}
}