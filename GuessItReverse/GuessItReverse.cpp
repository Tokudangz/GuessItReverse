#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <iostream>
#include <SDL_ttf.h>
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <iostream>
#include <SDL_ttf.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window* window = NULL;
SDL_Renderer* screen = NULL;
TTF_Font* myFont = NULL;
SDL_Event event;

bool is_quit = false;
SDL_Color color = { 250, 230, 50 };
int textHeight;
int textWidth;
bool gameOver = false;
int digit;
bool choosing = true;
int choose = 0;
int first = 1;
int last = 100;
int guess = 0;
bool guessing = true;
bool correct;
int score = 0;
std::string renderChoose = "0";
std::string renderGuess = "0";
std::string renderResult = "Computer Guess";
std::string renderPick = "Pick A Number";
void init();
void handleInput();
void showInput();
void RenderText(std::string output, int h);

int main(int argc, char* args[])
{
	init();
	while (!is_quit)
	{
		handleInput();
		SDL_SetRenderDrawColor(screen, 20, 200, 255, 0);
		SDL_RenderClear(screen);
		showInput();
		SDL_RenderPresent(screen);
	}

	SDL_DestroyRenderer(screen);
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();
	return 0;
}

void init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << SDL_GetError() << std::endl;
		exit(1);
	}

	window = SDL_CreateWindow("GuessIt", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		std::cout << SDL_GetError() << std::endl;
		exit(1);
	}

	if (!TTF_Init())
	{
		std::cout << SDL_GetError() << std::endl;
	}

	screen = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (screen == NULL)
	{
		std::cout << SDL_GetError() << std::endl;
		exit(1);
	}

	myFont = TTF_OpenFont("8-BIT-WONDER.ttf", 24);
	if (myFont == NULL)
	{
		std::cout << TTF_GetError() << std::endl;
		return;
	}
}

void RenderText(std::string output, int h)
{
	SDL_Surface* LoadSurface = TTF_RenderText_Solid(myFont, output.c_str(), color);
	if (LoadSurface == NULL)
	{
		std::cout << SDL_GetError() << std::endl;
		return;
	}

	textHeight = LoadSurface->h;
	textWidth = LoadSurface->w;

	SDL_Texture* newTexture = SDL_CreateTextureFromSurface(screen, LoadSurface);
	if (newTexture == NULL)
	{
		std::cout << SDL_GetError() << std::endl;
		return;
	}

	SDL_FreeSurface(LoadSurface);
	SDL_Rect renderQuad = { SCREEN_WIDTH / 2 - textWidth / 2, h, textWidth, textHeight
	};
	SDL_RenderCopy(screen, newTexture, NULL, &renderQuad);
}

void handleInput()
{
	while (SDL_PollEvent(&event) != 0)
	{
		if (!gameOver)
		{
			digit = -1;
			if (event.type == SDL_QUIT)
			{
				gameOver = true;
				is_quit = true;
			}

			if (event.type == SDL_KEYDOWN)
			{
				if (choosing)
				{
					switch (event.key.keysym.sym)
					{
					case SDLK_0:
						digit = 0;
						break;
					case SDLK_1:
						digit = 1;
						break;
					case SDLK_2:
						digit = 2;
						break;
					case SDLK_3:
						digit = 3;
						break;
					case SDLK_4:
						digit = 4;
						break;
					case SDLK_5:
						digit = 5;
						break;
					case SDLK_6:
						digit = 6;
						break;
					case SDLK_7:
						digit = 7;
						break;
					case SDLK_8:
						digit = 8;
						break;
					case SDLK_9:
						digit = 9;
						break;
					case SDLK_BACKSPACE:
						choose /= 10;
						renderChoose = std::to_string(choose);
						break;
					case SDLK_RETURN:
						choosing = false;
					}

					if (digit != -1)
					{
						choose = choose * 10 + digit;
						renderChoose = std::to_string(choose);
					}
				}
			}

			if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				guess = (first + last) / 2;
				if (guessing)
				{
					if (event.button.button == SDL_BUTTON_LEFT)
					{
						renderGuess = std::to_string(guess);
						if (guess == choose)
						{
							guessing = false;
							renderResult = "Correct";
							gameOver = true;
							score++;
						}
						else if (choose > guess)
						{
							first = guess;
							score++;
						}
						else
						{
							last = guess;
							score++;
						}
					}
				}
			}
		}
		else
		{
			if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_RETURN)
				{
					renderResult = "PLAY AGAIN";
					renderGuess = "Y       N";
					renderPick = "Computer Guess Count";
					renderChoose = std::to_string(score);
				}

				if (event.key.keysym.sym == SDLK_y)
				{
					gameOver = false;
					renderPick = "Pick A Number";
					renderGuess = "0";
					renderResult = "Computer Guess";
					renderChoose = "0";
					choosing = true;
					choose = 0;
					first = 1;
					last = 100;
					guess = 0;
					guessing = true;
					score = 0;
				}
				else if (event.key.keysym.sym == SDLK_n || event.type == SDL_QUIT)
				{
					gameOver = true;
					is_quit = true;
				}
			}
		}
	}
}

void showInput()
{
	RenderText(renderResult, 200);
	RenderText(renderGuess, 300);
	RenderText(renderChoose, 100);
	RenderText(renderPick, 50);
}