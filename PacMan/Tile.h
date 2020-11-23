#pragma once
#include <set>
#include "MyEnums.h"
#include "Item.h";
#include "PacDot.h";
#include "PowerPellet.h";


class Tile{
	public:
		Tile(int xPos, int yPos, char type, bool canMoveUp, bool canMoveDown, bool canMoveLeft, bool canMoveRight);
		Tile();

		void setTile(int xPos, int yPos, char type, bool canMoveUp, bool canMoveDown, bool canMoveLeft, bool canMoveRight);

		char getType();
		int getXPos();
		int getYPos();

		Item getItem();
		std::set<direction> getPossibleDirections();

		Effect consumeItem();

		Tile* prevTile = NULL;
		direction prevDirection = NA;
		int cost;

		friend bool operator<(const Tile& p1, const Tile& p2) { return p1.cost < p2.cost; }
		friend bool operator>(const Tile& p1, const Tile& p2) { return p1.cost > p2.cost; }
		friend bool operator==(const Tile& p1, const Tile& p2) { return p1.cost == p2.cost; }
	protected:
		std::set<direction> possibleDirections;

		//top left of the tile
		int xPos;
		int yPos;

		char type;

		Item tileItem;

};

