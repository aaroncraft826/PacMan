#pragma once
#include "SDL.h"
#include "Effect.h"

//Items are objects that are to be collected by Pacman and are to be put inside a Tile
class Item {
	public:
		bool doesExist();
		int getPointValue();
		Effect consume();// returns effect and causes whatever effect
		SDL_Rect getPic();
		SDL_Color getColor();

		static int getItemCount() {
			return itemCount;
		}
	protected:
		bool exists = false;
		int pointValue;
		SDL_Rect pic;
		SDL_Color col;
		Effect itemEffect = Effect(0, NOSTATE);

		static int itemCount;
};
