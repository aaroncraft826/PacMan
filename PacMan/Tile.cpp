#include "Tile.h"

Tile::Tile(int xPos, int yPos, char type, bool canMoveUp, bool canMoveDown, bool canMoveLeft, bool canMoveRight) {
	this->xPos = xPos;
	this->yPos = yPos;
	this->type = type;
	if (canMoveUp) { possibleDirections.insert(UP); }
	if (canMoveDown) { possibleDirections.insert(DOWN); }
	if (canMoveLeft) { possibleDirections.insert(LEFT); }
	if (canMoveRight) { possibleDirections.insert(RIGHT); }
}

Tile::Tile() {

}

void Tile::setTile(int xPos, int yPos, char type, bool canMoveUp, bool canMoveDown, bool canMoveLeft, bool canMoveRight) {
	this->xPos = xPos;
	this->yPos = yPos;
	this->type = type;
	if (canMoveUp == true) { possibleDirections.insert(UP); }
	if (canMoveDown == true) { possibleDirections.insert(DOWN); }
	if (canMoveLeft == true) { possibleDirections.insert(LEFT); }
	if (canMoveRight == true) { possibleDirections.insert(RIGHT); }

	switch (type) {
		case 'd':
			tileItem = PacDot(xPos, yPos);
			break;
		case 'I':
			this->type = 'i';
			tileItem = PacDot(xPos, yPos);
			break;
		case 'P':
			tileItem = PowerPellet(xPos, yPos);
			break;
	}
}

//returns point value and consumes tile's item
Effect Tile::consumeItem() {
	return tileItem.consume();
}

char Tile::getType() {
	return type;
}

int Tile::getXPos() {
	return xPos;
}

int Tile::getYPos() {
	return yPos;
}

std::set<direction> Tile::getPossibleDirections() {
	return possibleDirections;
}

Item Tile::getItem() {
	return tileItem;
}