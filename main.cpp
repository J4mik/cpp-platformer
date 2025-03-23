#include "src/debug.hpp"
#include "src/tiles.hpp"

#include <iostream>
#include <fstream>

#include "include/JSON/json.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

using json = nlohmann::json;

unsigned long lastTick;
float FPS = 30;
float rate = 10;

int SCREEN_WIDTH = 1000;
int SCREEN_HEIGHT = 600;
bool running = true;

bool UPdown = false;
bool LEFTdown = false;
bool RIGHTdown = false;

float playerX;
float playerY;

float playerVectY = 0;

float ofsetX;
float ofsetY;

bool flip = false;

unsigned int len = 0;

int main(int argc, char *argv[]) {

	println("running");

	std::ifstream ifs("test.json");
    json jsonLVL = json::parse(ifs);
    
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		print("error initializing SDL: ");
		print(SDL_GetError());
	}
	SDL_Window* win = SDL_CreateWindow("Flashblade", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);

	void SDL_SetWindowResizable(SDL_Window * window, SDL_bool resizable);

	SDL_Rect temp;
	temp.w = 48;
	temp.h = 48;

	Uint32 render_flags = SDL_RENDERER_ACCELERATED;
	SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);
	SDL_Surface* blocks;
	SDL_Surface* playerIMG;

	blocks = IMG_Load(".\\data\\images\\blocks.png");
	playerIMG = IMG_Load(".\\data\\images\\player.png");
	SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, blocks);
	SDL_Texture* block = SDL_CreateTextureFromSurface(rend, playerIMG);
	SDL_FreeSurface(blocks);
	SDL_FreeSurface(playerIMG);
	SDL_Rect clipRect{0, 16, 16, 16};
	SDL_Rect player; 

	SDL_QueryTexture(tex, NULL, NULL, &player.w, &player.h);

	player.w = 12;
	player.h = 21;
	player.x = 0;
	player.y = 0;

	float speed = 300;

	while (running == true) {
		SDL_Event event;

		while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
            	return true;
            } 
			else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d) {
                    RIGHTdown = true;
				}
				if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a) {
                    LEFTdown = true;
				}
				if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w) {
                    UPdown = true;
				}
            } 
			else if (event.type == SDL_KEYUP) {
				if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d) {
                    RIGHTdown = false;
				}
				if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a) {
                    LEFTdown = false;
				}
				if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w) {
                    UPdown = false;
				}
			}
            
        }
		

		if (UPdown == 1 && playerY == 200) {
			playerVectY = + 14;
		}

		playerX += ((speed * LEFTdown / 500) - (speed * RIGHTdown / 500)) * rate;
		playerY += playerVectY / 10 * rate;
		playerVectY += rate / -15;
		ofsetX = (1 - rate / 300) * ofsetX + (rate / 300) * playerX;
		ofsetY = (1 - rate / 350) * ofsetY + (rate / 350) * playerY;

		SDL_GetWindowSizeInPixels(win, &SCREEN_WIDTH, &SCREEN_HEIGHT);
		
		player.x = round((SCREEN_WIDTH - player.w) / 2 + ofsetX - playerX);
		player.y = round((SCREEN_HEIGHT - player.h) / 3 * 2 - playerY + ofsetY);

		// player.y = round(playerY); // testing

		if (playerY <= 200) {
			playerY = 200;
			playerVectY = 0;
		}

		SDL_RenderClear(rend);
		if (LEFTdown - RIGHTdown == 1) {flip = 1;}
		else if (LEFTdown - RIGHTdown == -1) {flip = 0;}
		if (flip == 1) {SDL_RenderCopyEx(rend, block, NULL, &player, 0, 0, SDL_FLIP_HORIZONTAL);}
		else {SDL_RenderCopyEx(rend, block, NULL, &player, 0, 0, SDL_FLIP_NONE);}
		len = jsonLVL["level"].size();
		for (unsigned int i = 0; i < len; ++i) {
			temp.x = round(int(jsonLVL["level"][i]["pos"][0]) * 48 + ofsetX);
			temp.y = round(-int(jsonLVL["level"][i]["pos"][1]) * 48 + ofsetY);
			if (jsonLVL["level"][i]["type"] == "grass") {clipRect.y = 16;}
			else if (jsonLVL["level"][i]["type"] == "path") {clipRect.y = 32;}
			else if (jsonLVL["level"][i]["type"] == "dirt") {clipRect.y = 0;}
			SDL_RenderCopy(rend, tex, &clipRect, &temp);
			// colisions
			if (player.x <= temp.x + temp.w && player.x >= temp.x || player.x + player.w <= temp.x + temp.w && player.x + player.w >= temp.x) {
				if (player.y <= temp.y + temp.h && player.y >= temp.y || player.y + player.h <= temp.y + temp.h && player.y + player.h >= temp.y) {
					std::cout << "collision\n";
					if (player.y + player.h >= temp.y) {
						std::cout << "elevat\n";
						playerY = playerY + (player.y + player.h - temp.y);
						playerVectY = 0;
					}
				}
			}
		}
		SDL_RenderPresent(rend);

		SDL_Delay(1000/72);

		rate = SDL_GetTicks() - lastTick;
		lastTick = SDL_GetTicks();
		FPS = 0.9 * FPS + 100 / rate;

		// std::cout << FPS << ",  ";
		
	}

	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();

	return 0;
}