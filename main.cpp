#include "src/player.hpp"
#include "src/collision.hpp"

#include <iostream>
#include <fstream>
#include <cmath>

#include "include/JSON/json.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

using json = nlohmann::json;

unsigned long lastTick;
double FPS = 30;
double deltaTime = 10;

struct {
	int width = 1000;
	int height = 600;
} screen;

bool running = true;

struct {
	bool UP;
	bool LEFT;
	bool RIGHT;
	bool W;
	bool a;
	bool d;
} key;

int lowest = 0;

struct {
	int x;
	int y;
} move;

bool flip = false;

double playerX;
double playerY;

double playerVectX = 0;
double playerVectY = 0;

bool jump = false;

double ofsetX = 0;
double ofsetY = 0;

std::ifstream ifs("test.json");
json jsonLVL = json::parse(ifs);

json level = jsonLVL["level"];
unsigned int len = jsonLVL["level"].size();




int main(int argc, char *argv[]) {
    
	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		std::cerr << "error initialising SDL, Error:" << SDL_GetError << std::endl;
	}
	SDL_Window* win = SDL_CreateWindow("Flashblade", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen.width, screen.height, SDL_WINDOW_RESIZABLE);

	void SDL_SetWindowResizable(SDL_Window * window, SDL_bool resizable);

	SDL_Rect temp;
	temp.w = 48;
	temp.h = 48;

	Uint32 render_flags = SDL_RENDERER_ACCELERATED;
	SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);
	SDL_Surface* blocks;
	SDL_Surface* playerSprite;

	blocks = IMG_Load(".\\data\\images\\blocks.png");
	playerSprite = IMG_Load(".\\data\\images\\player.png");
	SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, blocks);
	SDL_Texture* block = SDL_CreateTextureFromSurface(rend, playerSprite);
	SDL_FreeSurface(blocks);
	SDL_FreeSurface(playerSprite);
	SDL_Rect clipRect{0, 16, 16, 16};
	SDL_Rect player; 

	SDL_QueryTexture(tex, NULL, NULL, &player.w, &player.h);

	player.w = 12;
	player.h = 21;
	player.x = 0;
	player.y = 0;

	double speed = 300;

	while (running == true) {
		SDL_Event event;

		while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
            	return true;
            } 
			if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
					case SDLK_RIGHT: // || SDLK_d:
						key.RIGHT = 1;
						break;
					
					case SDLK_LEFT: // || SDLK_a:
						key.LEFT = 1;
						break;
					
					case SDLK_UP: // || SDLK_w:
						key.UP = 1;
						break;

					default:
						break;
				}
            } 
			else if (event.type == SDL_KEYUP) {
				switch (event.key.keysym.sym) {
					case SDLK_RIGHT: // || SDLK_d:
						key.RIGHT = 0;
						break;
					
					case SDLK_LEFT: // || SDLK_a:
						key.LEFT = 0;
						break;
					
					case SDLK_UP: // || SDLK_w:
						key.UP = 0;
						break;

					default:
						break;
				}
			}
            
        }

		deltaTime = SDL_GetTicks() - lastTick;
		lastTick = SDL_GetTicks();
		FPS = 0.9 * FPS + 100 / deltaTime;
		

		if (key.UP == 1 && jump == 1) {
			playerVectY = + 14;
		}
		jump = 0;
		
		playerVectX = playerVectX - (key.RIGHT - key.LEFT) * (deltaTime / 100);
			playerX += playerVectX * deltaTime / 3;
			playerVectX *= 1 / (1 +  deltaTime / 50);
			playerX += playerVectX * deltaTime / 3;
		// // for tile in tilesAroundPos(pos):
		// // 	if colliding(tile):
		// // 		if playerVectX * deltaTime > 0:
		// // 			playerX = tile.pos.x - player.width
		// // 		else:
		// // 			playerX = tile.pos.x + tile.width
		// // 		playerVectX = 0

		playerY += playerVectY / 20 * deltaTime;
		playerVectY += deltaTime / -15;
		playerY += playerVectY / 20 * deltaTime;
	

		lowest = 32000;
		move.x = 0;
		move.y = 0;

		for (unsigned int i = 0; i < len; ++i) {
			temp.x = round(int(jsonLVL["level"][i]["pos"][0]) * 48 + ofsetX);
			temp.y = round(-int(jsonLVL["level"][i]["pos"][1]) * 48 + ofsetY);
			if (!collision::detect(player.x + j, player.y + n, temp.x, temp.y)) {
			}

			playerX += move.x;
			playerY += move.y;
		}

		player.x = round((screen.width - player.w) / 2 + ofsetX - playerX);
		player.y = round((screen.height - player.h) / 3 * 2 - playerY + ofsetY);

		// if (collide.horizontal) {
		// 	playerX += playerVectX * deltaTime / 3;
		// 	playerVectX *= 1 / (1 +  deltaTime / 50);
		// 	playerX += playerVectX * deltaTime / 3;
		// }
		// if (collide.vertical) {
		// 	playerY += playerVectY / 20 * deltaTime;
		// 	playerVectY += deltaTime / -15;
		// 	playerY += playerVectY / 20 * deltaTime;
		// }
		// collide.vertical = 1;
		// collide.horizontal = 1;



		// for tile in tilesAroundPos(pos):
		// 	if colliding(tile):
		// 		if playerVectY / 10 * deltaTime > 0:
		// 			playerY = tile.pos.y - player.height
		// 		else:
		// 			playerY = tile.pos.y + tile.height
		// 		playerVectY = 0

		ofsetX = (1 - deltaTime / 300) * ofsetX + (deltaTime / 300) * playerX;
		ofsetY = (1 - deltaTime / 350) * ofsetY + (deltaTime / 350) * playerY;

		// collision::detect();

		SDL_GetWindowSizeInPixels(win, &screen.width, &screen.height);
		
		player.x = round((screen.width - player.w) / 2 + ofsetX - playerX);
		player.y = round((screen.height - player.h) / 3 * 2 - playerY + ofsetY);

		// player.y = round(playerY); // testing

		if (playerY <= 200) {
			playerY = 200;
			playerVectY = 0;
			jump = 1;
		}

		playerVectX = playerVectX - (key.RIGHT - key.LEFT) * (deltaTime / 100);

		SDL_RenderClear(rend);
		if (key.LEFT - key.RIGHT == 1) {flip = 1;}
		else if (key.LEFT - key.RIGHT == -1) {flip = 0;}
		if (flip == 1) {SDL_RenderCopyEx(rend, block, NULL, &player, 0, 0, SDL_FLIP_HORIZONTAL);}
		else {SDL_RenderCopyEx(rend, block, NULL, &player, 0, 0, SDL_FLIP_NONE);}

		for (unsigned int i = 0; i < len; ++i) {
			temp.x = round(int(jsonLVL["level"][i]["pos"][0]) * 48 + ofsetX);
			temp.y = round(-int(jsonLVL["level"][i]["pos"][1]) * 48 + ofsetY);
			if (jsonLVL["level"][i]["type"] == "grass") {clipRect.y = 16;}
			else if (jsonLVL["level"][i]["type"] == "path") {clipRect.y = 32;}
			else if (jsonLVL["level"][i]["type"] == "dirt") {clipRect.y = 0;}
			SDL_RenderCopy(rend, tex, &clipRect, &temp);
			// if (collision::detect(player.x, player.y, temp.x, temp.y)) {
			// 	playerX -= playerVectX * deltaTime / 3;
			// 	playerY -= playerVectY / 20 * deltaTime;
			// std::cout << "colision\n";
			// 	jump = 1;
			// }
			
		}
		player.x = round((screen.width - player.w) + ofsetX - playerX);
		player.y = round((screen.height - player.h) / 3 * 4 - playerY + ofsetY);

		if (key.LEFT - key.RIGHT == 1) {flip = 1;}
		else if (key.LEFT - key.RIGHT == -1) {flip = 0;}
		if (flip == 1) {SDL_RenderCopyEx(rend, block, NULL, &player, 0, 0, SDL_FLIP_HORIZONTAL);}
		else {SDL_RenderCopyEx(rend, block, NULL, &player, 0, 0, SDL_FLIP_NONE);}
		SDL_RenderPresent(rend);

		// SDL_Delay(1000/72);

		

		// std::cout << FPS << ",  "; // testing framerate
		
	}

	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();

	return 0;
}