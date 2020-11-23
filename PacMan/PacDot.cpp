#include "PacDot.h"
#include "MyDefinitions.h"

//constructor takes in position TOP LEFT of the TILE
PacDot::PacDot(int xPos, int yPos) {
	exists = true;
	itemCount++;

	pointValue = 10;
	itemEffect = Effect(pointValue, NOSTATE);

	pic.x = xPos + (TILE_SIZE / 4);
	pic.y = yPos + (TILE_SIZE / 4);
	pic.h = TILE_SIZE / 4;
	pic.w = TILE_SIZE / 4;

	col = { 255, 255, 0, 255 };
}