#include "Item.h"

int Item::itemCount = 0;

bool Item::doesExist() {
	return exists;
}

int Item::getPointValue() {
	return pointValue;
}

Effect Item::consume() {
	exists = false;
	itemCount--;
	return itemEffect;
}

SDL_Rect Item::getPic() {
	return pic;
}

SDL_Color Item::getColor() {
	return col;
}