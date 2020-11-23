#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "SDL.h"
#include "SDL_ttf.h"
#include "Intersection.h"
#include "PlayerCharacter.h"
#include "Ghost.h"
#include "Tile.h"
#include "MyDefinitions.h"
#undef main

void start(Tile map[MAP_X_LENGTH][MAP_Y_LENGTH], PlayerCharacter* player, std::vector<Ghost*> &ghostList);
void buildMap(char cmap[MAP_X_LENGTH][MAP_Y_LENGTH], Tile map[MAP_X_LENGTH][MAP_Y_LENGTH], std::string mapFile);
void populate(PlayerCharacter* player, std::vector<Ghost*> &ghostList);
void drawMap(Tile map[MAP_X_LENGTH][MAP_Y_LENGTH]);
void drawGameOver();
void cToTileMap(char cmap[MAP_X_LENGTH][MAP_Y_LENGTH], Tile map[MAP_X_LENGTH][MAP_Y_LENGTH]);

SDL_Window* window;
SDL_Surface* winSurface;
SDL_Renderer* renderer;

int score = 0;
int timer = 7 * FRAMES_PER_SECOND;
int ghostsCaptured = 0;//ghosts captured during a fright
bool running = true;
bool won = false;
state commonState = SCATTER;

int main(int argc, char** args)
{
	//Create Window and Surface
	/*
	* Will be 28 x 36 tiles
	* 24 x 24 tile size
	*/
    window = SDL_CreateWindow("Pacman", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 672, 864, SDL_WINDOW_SHOWN);
	winSurface = SDL_GetWindowSurface(window);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	//load the and create the map
	Tile map[MAP_X_LENGTH][MAP_Y_LENGTH];
	char cmap[MAP_X_LENGTH][MAP_Y_LENGTH];
	buildMap(cmap, map, "MainMap.txt");

	//Black Background
	if (TTF_Init() < 0) {
		std::cout << TTF_GetError << std::endl;
		exit(2);
	}
	SDL_FillRect(winSurface, NULL, SDL_MapRGB(winSurface->format, 0, 0, 0));
	SDL_UpdateWindowSurface(window);
	PlayerCharacter* player = (PlayerCharacter*) malloc(sizeof(PlayerCharacter));
	std::vector<Ghost*> ghostList;
	//Ghost* redGhost = (Ghost*)malloc(sizeof(Ghost));
	populate(player, ghostList);

	start(map, player, ghostList);

	drawGameOver();
	bool gameOverScreenOn = true;
	SDL_Event eve;
	while (gameOverScreenOn) {
		//Event loop
		while (SDL_PollEvent(&eve) != 0) {
			//check event type
			switch (eve.type) {
				case SDL_QUIT:
					gameOverScreenOn = false;
					break;
			}
		}
	}
	//Quits Program
	TTF_Quit();
	free(player);
	for (Ghost* g : ghostList) {
		delete(g);
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
    return EXIT_SUCCESS;
}

void buildMap(char cmap[MAP_X_LENGTH][MAP_Y_LENGTH], Tile map[MAP_X_LENGTH][MAP_Y_LENGTH], std::string mapFile) {
	std::ifstream m(mapFile);
	if (m.is_open()) {
		char sx[8];
		char sy[8];
		m.getline(sx, 8);
		m.getline(sy, 8);

		int xLength = strtol(sx, NULL, 10);
		int yLength = strtol(sy, NULL, 10);

		for (int y = 0; y < MAP_Y_LENGTH; y++) {
			for (int x = 0; x < MAP_X_LENGTH; x++) {
				m.get(cmap[x][y]);
				while (cmap[x][y] == '\n') {
					//std::cout << "REEEEEEEE" << std::endl;
					m.get(cmap[x][y]);
				}
				std::cout << cmap[x][y];
			}
			std::cout << "\n";
		}
		std::cout << std::endl;
		cToTileMap(cmap, map);
		drawMap(map);
	}
}

void cToTileMap(char cmap[MAP_X_LENGTH][MAP_Y_LENGTH], Tile map[MAP_X_LENGTH][MAP_Y_LENGTH]) {

	for (int y = 0; y < MAP_Y_LENGTH; y++) {
		for (int x = 0; x < MAP_X_LENGTH; x++) {

			bool canMoveUp = false;
			bool canMoveDown = false;
			bool canMoveLeft = false;
			bool canMoveRight = false;

			if (y < MAP_Y_LENGTH && cmap[x][y + 1] != 'w' && (cmap[x][y + 1] != 'D' || cmap[x][y] == 'E')) {
				canMoveDown = true;
			}
			if (y > 0 && cmap[x][y - 1] != 'w' && (cmap[x][y - 1] != 'D' || cmap[x][y] == 'E')) {
				canMoveUp = true;
			}
			if (x < MAP_X_LENGTH && cmap[x + 1][y] != 'w' && (cmap[x + 1][y] != 'D' || cmap[x][y] == 'E')) {
				canMoveRight = true;
			}
			if (x > 0 && cmap[x - 1][y] != 'w' && (cmap[x - 1][y] != 'D')) {
				canMoveLeft = true;
			}

			//if (canMoveUp == true) { std::cout << "isTrue" << std::endl; }

			map[x][y].setTile(x*TILE_SIZE, y*TILE_SIZE, cmap[x][y], canMoveUp, canMoveDown, canMoveLeft, canMoveRight);
		}
	}
}

//Creates graphics for map
void drawMap(Tile map[MAP_X_LENGTH][MAP_Y_LENGTH]) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	for (int x = 0; x < MAP_X_LENGTH; x++) {
		for (int y = 0; y < MAP_Y_LENGTH; y++) {
			SDL_Rect r;
			switch (map[x][y].getType()) {
				case 'w'://Create Wall Graphic
					SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
					r.x = (x * TILE_SIZE);
					r.y = (y * TILE_SIZE);
					r.h = TILE_SIZE;
					r.w = TILE_SIZE;
					if (y < MAP_Y_LENGTH && map[x][y + 1].getType() != 'w' && map[x][y + 1].getType() != 'D') {
						r.h = TILE_SIZE / 2;
					}
					if (y > 0 && map[x][y - 1].getType() != 'w' && map[x][y - 1].getType() != 'D') {
						r.h /= 2;
						r.y += r.h;
					}
					if (x < MAP_X_LENGTH && map[x + 1][y].getType() != 'w' && map[x + 1][y].getType() != 'D') {
						r.w = TILE_SIZE / 2;
					}
					if (x > 0 && map[x - 1][y].getType() != 'w' && map[x - 1][y].getType() != 'D') {
						r.w /= 2;
						r.x += r.w;
					}
					SDL_RenderFillRect(renderer, &r);
					break;
				case 'e':
					break;
				case 'd':
					break;
				case 'D'://Create Wall Graphic
					SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
					r;
					r.x = (x * TILE_SIZE);
					r.y = (y * TILE_SIZE);
					r.h = TILE_SIZE;
					r.w = TILE_SIZE;
					if (y < MAP_Y_LENGTH && map[x][y + 1].getType() != 'w' && map[x][y + 1].getType() != 'D') {
						r.h = TILE_SIZE / 2;
					}
					if (y > 0 && map[x][y - 1].getType() != 'w' && map[x][y - 1].getType() != 'D') {
						r.h /= 2;
						r.y += r.h;
					}
					if (x < MAP_X_LENGTH && map[x + 1][y].getType() != 'w' && map[x + 1][y].getType() != 'D') {
						r.w = TILE_SIZE / 2;
					}
					if (x > 0 && map[x - 1][y].getType() != 'w' && map[x - 1][y].getType() != 'D') {
						r.w /= 2;
						r.x += r.w;
					}
					SDL_RenderFillRect(renderer, &r);
					break;
				/*case 'i'://draws intersections green for testing purposes
					SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
					SDL_Rect re;
					re.x = (x * TILE_SIZE);
					re.y = (y * TILE_SIZE);
					re.h = TILE_SIZE;
					re.w = TILE_SIZE;
					SDL_RenderFillRect(renderer, &re);
					break;*/

			}
			//draw item on tile
			if (map[x][y].getItem().doesExist()) {
				SDL_Color itemColor = map[x][y].getItem().getColor();
				SDL_SetRenderDrawColor(renderer, itemColor.r, itemColor.g, itemColor.b, itemColor.a);
				SDL_Rect itemPicture = map[x][y].getItem().getPic();
				SDL_RenderFillRect(renderer, &itemPicture);
			}
		}
	}

	//Draw Text
	TTF_Font* Sans = TTF_OpenFont("OpenSans/OpenSans-Regular.ttf", 24);
	if (Sans == NULL) {
		std::cout << "FONT NOT LOADED" << std::endl;
	}
	SDL_Color white = { 255, 255, 255 };
	std::string scoreText = "Score: " + std::to_string(score);
	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Sans, scoreText.c_str(), white);
	SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
	SDL_Rect Message_rect; //create a rect
	Message_rect.x = 0;
	Message_rect.y = 0;
	Message_rect.w = TILE_SIZE * 8;
	Message_rect.h = TILE_SIZE * 3;
	SDL_RenderCopy(renderer, Message, NULL, &Message_rect);

	SDL_RenderPresent(renderer);
}

void drawGameOver() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	TTF_Font* Sans = TTF_OpenFont("OpenSans/OpenSans-Regular.ttf", 24);
	if (Sans == NULL) {
		std::cout << "FONT NOT LOADED" << std::endl;
	}
	SDL_Color white = { 255, 255, 255 };

	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Sans, "GAME OVER", white);
	SDL_Texture* GameOver = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
	SDL_Rect GameOver_rect; //create a rect
	GameOver_rect.x = TILE_SIZE * MAP_X_LENGTH / 4;
	GameOver_rect.y = 0;
	GameOver_rect.w = (TILE_SIZE * MAP_X_LENGTH / 2) - 1;
	GameOver_rect.h = (TILE_SIZE * MAP_Y_LENGTH / 2) - 1;
	SDL_RenderCopy(renderer, GameOver, NULL, &GameOver_rect);

	std::string scoreText = "Score: " + std::to_string(score);
	SDL_Surface* sM = TTF_RenderText_Solid(Sans, scoreText.c_str(), white);
	SDL_Texture* Score = SDL_CreateTextureFromSurface(renderer, sM);
	SDL_Rect Score_rect; //create a rect
	Score_rect.x = (TILE_SIZE * MAP_X_LENGTH / 3) + 48;
	Score_rect.y = (TILE_SIZE * MAP_Y_LENGTH / 2) + 1;
	Score_rect.w = (TILE_SIZE * MAP_X_LENGTH / 4) - 1;
	Score_rect.h = (TILE_SIZE * MAP_Y_LENGTH / 4) - 1;
	SDL_RenderCopy(renderer, Score, NULL, &Score_rect);


	SDL_RenderPresent(renderer);
}

void populate(PlayerCharacter* player, std::vector<Ghost*> &ghostList) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
	PlayerCharacter p((14 * TILE_SIZE), (26 * TILE_SIZE), 14, 26, 255, 255, 0, 255);
	*player = p;

	Ghost* rg = new Ghost((14 * TILE_SIZE), (14 * TILE_SIZE), 14, 14, MAP_X_LENGTH - 1, 0, 0 * FRAMES_PER_SECOND, 255, 0, 0, 255);
	rg->setState(SCATTER);
	ghostList.push_back(rg);

	Ghost* pg = new Ghost((11 * TILE_SIZE), (17 * TILE_SIZE), 11, 17, 0, 0, 5 * FRAMES_PER_SECOND, 255, 192, 203, 255);
	pg->setState(REST);
	ghostList.push_back(pg);

	Ghost* bg = new Ghost((13 * TILE_SIZE), (17 * TILE_SIZE), 13, 17, MAP_X_LENGTH - 2, MAP_Y_LENGTH - 2, 10 * FRAMES_PER_SECOND, 0, 255, 255, 255);
	bg->setState(REST);
	ghostList.push_back(bg);

	Ghost* og = new Ghost((15 * TILE_SIZE), (17 * TILE_SIZE), 15, 17, 0, MAP_Y_LENGTH - 1, 15 * FRAMES_PER_SECOND, 255, 140, 0, 255);
	og->setState(REST);
	ghostList.push_back(og);
	//SDL_RenderPresent(renderer);
}

void update(Tile map[MAP_X_LENGTH][MAP_Y_LENGTH], PlayerCharacter* player, std::vector<Ghost*> &ghostList) {
	drawMap(map);

	player->PlayerCharacter::move(2, map);
	if (player->pelletHasBeenEaten()) {
		commonState = FRIGHTENED;
		timer = 7 * FRAMES_PER_SECOND;
		for (Ghost* gh : ghostList) {
			if (gh->getState() == SCATTER || gh->getState() == CHASE) { gh->setState(FRIGHTENED); }
		}
	}
	if (Item::getItemCount() <= 0) {
		SDL_Delay(500);
		running = false;
	}
	SDL_Color playerColor = player->getColor();
	SDL_SetRenderDrawColor(renderer, playerColor.r, playerColor.g, playerColor.b, playerColor.a);
	SDL_RenderFillRect(renderer, player->getPic());
	score = player->getScore();
	//std::cout << "score: " << player->getScore() << std::endl;

	for (Ghost* g : ghostList) {
		switch (commonState) {
			case SCATTER:
				if (g->getState() == CHASE || g->getState() == FRIGHTENED) { g->setState(SCATTER); }
				break;
			case CHASE:
				if (g->getState() == SCATTER || g->getState() == FRIGHTENED) { g->setState(CHASE); }
				break;
			case FRIGHTENED:
				//if (g->getState() == SCATTER || g->getState() == CHASE) { g->setState(FRIGHTENED); }
				break;
		}
		if (g->getState() == FRIGHTENED) {
			g->Ghost::move(1, map, *player);
		}
		else if (g->getState() == RETREAT) {
			g->Ghost::move(4, map, *player);
		}
		else{
			g->Ghost::move(2, map, *player);
		}
		g->Ghost::updateDot();
		SDL_Color ghostColor = g->getColor();
		if (g->getState() == FRIGHTENED) {
			SDL_SetRenderDrawColor(renderer, 0, 0, 255, 0);
		}
		else {
			SDL_SetRenderDrawColor(renderer, ghostColor.r, ghostColor.g, ghostColor.b, ghostColor.a);
		}
		SDL_RenderFillRect(renderer, g->getPic());

		//Check if pacman and ghost is touching
		if (g->getXTile() == player->getXTile() && g->getYTile() == player->getYTile()) {
			if (g->getState() == FRIGHTENED) {//PACMAN GOT THE GHOST
				int multiplier = pow(2, ghostsCaptured);
				player->addScore(200 * multiplier);
				ghostsCaptured = ghostsCaptured == 4 ? 0 : ghostsCaptured + 1;
				g->setXPos(g->getXTile() * TILE_SIZE);
				g->setYPos(g->getYTile() * TILE_SIZE);
				g->setState(RETREAT);
			}
			else if(g->getState() != RETREAT){//Trigger GAMEOVER
				SDL_Delay(500);
				running = false;
			}
		}
	}

	SDL_RenderPresent(renderer);
}

void updateTimer(PlayerCharacter* player, std::vector<Ghost*>& ghostList) {
	timer--;
	if (timer == 0) {
		switch (commonState) {
			case FRIGHTENED:
				commonState = SCATTER;
				std::cout << "Common State: SCATTER" << std::endl;
				timer = 7 * FRAMES_PER_SECOND;
				for (Ghost* gho : ghostList) {
					if (gho->getState() == FRIGHTENED) {
						gho->setXPos(gho->getXTile() * TILE_SIZE);
						gho->setYPos(gho->getYTile() * TILE_SIZE);
					}
				}
				break;
			case SCATTER:
				commonState = CHASE;
				std::cout << "Common State: CHASE" << std::endl;
				timer = 20 * FRAMES_PER_SECOND;
				break;
			case CHASE:
				commonState = SCATTER;
				std::cout << "Common State: SCATTER" << std::endl;
				timer = 7 * FRAMES_PER_SECOND;
				break;
		}
	}
}

void start(Tile map[MAP_X_LENGTH][MAP_Y_LENGTH], PlayerCharacter* player, std::vector<Ghost*> &ghostList) {
	SDL_Event ev;
	running = true;
	//Main Loop
	while (running) {
		//Event loop
		while (SDL_PollEvent(&ev) != 0) {
			//check event type
			switch (ev.type) {
			case SDL_QUIT:
				running = false;
				break;
			case SDL_KEYDOWN:
				//up, down, left and right keys
				switch (ev.key.keysym.sym) {
					case SDLK_UP:
						//std::cout << "up key pressed" << std::endl;
						player->setNextDirection(UP);
						break;
					case SDLK_DOWN:
						//std::cout << "down key pressed" << std::endl;
						player->setNextDirection(DOWN);
						break;
					case SDLK_LEFT:
						//std::cout << "left key pressed" << std::endl;
						player->setNextDirection(LEFT);
						break;
					case SDLK_RIGHT:
						//std::cout << "right key pressed" << std::endl;
						player->setNextDirection(RIGHT);
						break;
				}
				break;
			}
		}

		//about 60 frames per second
		update(map, player, ghostList);
		updateTimer(player, ghostList);
		SDL_Delay(17);
	}
}