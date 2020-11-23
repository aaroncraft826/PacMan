#include "PowerPellet.h"
#include "MyDefinitions.h"

PowerPellet::PowerPellet(int xPos, int yPos) {
	exists = true;
	itemCount++;

	pointValue = 50;
	itemEffect = Effect(pointValue, FRIGHTENED);

	pic.x = xPos + (TILE_SIZE / 8);
	pic.y = yPos + (TILE_SIZE / 8);
	pic.h = TILE_SIZE / 2;
	pic.w = TILE_SIZE / 2;

	col = { 255, 255, 255, 255 };

}