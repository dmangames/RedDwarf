/*Dmangames 2021*/

//Using SDL, SDL_image, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <SDL_ttf.h>
#include "InputHandler.h"

enum GameState {
	MAIN_MENU,
	IN_GAME,
	GAME_OVER
};

////CLASS HEADERS////
/////////////////////

//Texture wrapper class
class LTexture
{
public:
	//Initializes variables
	LTexture();

	//Deallocates memory
	~LTexture();

	//Loads image at specified path
	bool loadFromFile(std::string path);

	//Creates image from font string
	bool loadFromRenderedText(std::string textureText, SDL_Color textColor);

	//Deallocates texture
	void free();

	//Set color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	//Set blending
	void setBlendMode(SDL_BlendMode blending);

	//Set alpha modulation
	void setAlpha(Uint8 alpha);

	//Renders texture at given point
	void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE, int scale = 1);

	//Gets image dimensions
	int getWidth();
	int getHeight();

private:
	//The actual hardware texture
	SDL_Texture* mTexture;

	//Image dimensions
	int mWidth;
	int mHeight;
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//END CLASS HEADERS////////
///////////////////////////

GameState gameState = MAIN_MENU;

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;



//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//input handler
InputHandler* inputHandler = NULL;

//Player actor
GameActor* player = NULL;
//Idle animation
const int IDLE_ANIMATION_FRAMES = 2;
//Walking animation
const int WALKING_ANIMATION_FRAMES = 8;
SDL_Rect gSpriteClips[IDLE_ANIMATION_FRAMES + WALKING_ANIMATION_FRAMES];
LTexture gSpriteSheetTexture;

//Main menu texture
LTexture gMainMenuTexture;

//Globally used font
TTF_Font* gFont = NULL;

//Rendered text texture
LTexture gTextTexture;


//Current animation frame
int frame = 0;



LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile(std::string path)
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor)
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		//Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if (mTexture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}

	//Return success
	return mTexture != NULL;
}

void LTexture::free()
{
	//Free texture if it exists
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	//Modulate texture rgb
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending)
{
	//Set blending function
	SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha)
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip, int scale)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w * scale;
		renderQuad.h = clip->h * scale;
	}

	//Render to screen
	SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}

				//Initialize SDL_ttf
				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}
			}
		}
	}

	//Init input handler
	inputHandler = new InputHandler();

	//Init player
	player = new GameActor("Player");

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load sprite sheet texture
	if (!gSpriteSheetTexture.loadFromFile("images/dwarf.png"))
	{
		printf("Failed to load walking animation texture!\n");
		success = false;
	}
	else
	{
		//Set sprite clips
		//IDLE
		gSpriteClips[0].x = 0;
		gSpriteClips[0].y = 0;
		gSpriteClips[0].w = 32;
		gSpriteClips[0].h = 32;

		gSpriteClips[1].x = 32;
		gSpriteClips[1].y = 0;
		gSpriteClips[1].w = 32;
		gSpriteClips[1].h = 32;

		//WALK LEFT
		gSpriteClips[2].x = 64;
		gSpriteClips[2].y = 0;
		gSpriteClips[2].w = 32;
		gSpriteClips[2].h = 32;

		gSpriteClips[3].x = 96;
		gSpriteClips[3].y = 0;
		gSpriteClips[3].w = 32;
		gSpriteClips[3].h = 32;

		gSpriteClips[4].x = 128;
		gSpriteClips[4].y = 0;
		gSpriteClips[4].w = 32;
		gSpriteClips[4].h = 32;

		gSpriteClips[5].x = 160;
		gSpriteClips[5].y = 0;
		gSpriteClips[5].w = 32;
		gSpriteClips[5].h = 32;

		gSpriteClips[6].x = 192;
		gSpriteClips[6].y = 0;
		gSpriteClips[6].w = 32;
		gSpriteClips[6].h = 32;

		gSpriteClips[7].x = 224;
		gSpriteClips[7].y = 0;
		gSpriteClips[7].w = 32;
		gSpriteClips[7].h = 32;

		gSpriteClips[8].x = 256;
		gSpriteClips[8].y = 0;
		gSpriteClips[8].w = 32;
		gSpriteClips[8].h = 32;

		gSpriteClips[9].x = 288;
		gSpriteClips[9].y = 0;
		gSpriteClips[9].w = 32;
		gSpriteClips[9].h = 32;
	}

	if (!gMainMenuTexture.loadFromFile("images/red_dwarf_main_menu_art.png")) {
		printf("Failed to load main menu texture!\n");
		success = false;
	}

	//Open the font
	gFont = TTF_OpenFont("fonts/lazy.ttf", 28);
	if (gFont == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	else
	{

	}

	return success;
}

void close()
{
	//Free loaded images
	gSpriteSheetTexture.free();
	gMainMenuTexture.free();
	gTextTexture.free();

	//Free global font
	TTF_CloseFont(gFont);
	gFont = NULL;

	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Release input handler
	delete inputHandler;
	inputHandler = NULL;

	//Release player
	delete player;
	player = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void mainMenu() {
	gMainMenuTexture.render(0, 0);

	//Render text
	SDL_Color textColor = { 255, 255, 255 };
	gTextTexture.loadFromRenderedText("Press any key to start", textColor);
	gTextTexture.render(500, 500);

	//Update screen
	SDL_RenderPresent(gRenderer);
}

void playGame() {
	Command* command = inputHandler->handleInput();

	if (command) {
		command->execute(*player);
	}

	//Set red dwarf facing direction based on key state
	SDL_RendererFlip direction = SDL_FLIP_NONE;
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
	if (currentKeyStates[SDL_SCANCODE_LEFT]) {
		direction = SDL_FLIP_NONE;
	}
	if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
		direction = SDL_FLIP_HORIZONTAL;
	}

	//Clear screen
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);

	//Render current frame
	SDL_Rect* currentClip = &gSpriteClips[frame / 4 + IDLE_ANIMATION_FRAMES];
	//gSpriteSheetTexture.render((SCREEN_WIDTH - currentClip->w) / 2, (SCREEN_HEIGHT - currentClip->h) / 2, currentClip, 0, 0, direction);
	gSpriteSheetTexture.render(player->getX(), player->getY(), currentClip, 0, 0, direction);

	//Update screen
	SDL_RenderPresent(gRenderer);

	//Go to next frame
	++frame;

	//Cycle animation
	if (frame / 4 >= WALKING_ANIMATION_FRAMES)
	{
		frame = 0;
	}
}

int main(int argc, char* args[])
{
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Load media
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;



			//While application is running
			while (!quit)
			{
				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
					//User pressed any button while in main menu
					else if (e.type == SDL_KEYDOWN && gameState == MAIN_MENU) {
						gameState = IN_GAME;
					}
				}
				switch (gameState) {
					case MAIN_MENU:
						mainMenu();
						break;
					case IN_GAME:
						playGame();
						break;
					default:
						break;
				}
				
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}