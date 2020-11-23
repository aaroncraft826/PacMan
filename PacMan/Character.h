#pragma once
#include "SDL.h"
#include "Tile.h"
#include "MyDefinitions.h"
#include "MyEnums.h"

class Character{
	public:
		void setXPos(int x);
		void setYPos(int y);
		void setNextDirection(direction d);
		virtual void move(int a, Tile map[MAP_X_LENGTH][MAP_Y_LENGTH]);

		int getXTile();
		int getYTile();

		SDL_Rect* getPic();
		SDL_Color getColor();
		direction getDirection();
	protected:
		//xPos and yPos refer to topLeft position in tile
		int xPos;
		int yPos;
		int xTile;
		int yTile;

		SDL_Rect pic;
		SDL_Color color = { 255, 255, 0, 255 };
		direction dir = NA;
		direction nextDir = NA;

		void updateTile();
		bool notBackwards();
};