//SDL libs
#include <SDL.h>
#include <SDL_image.h>

//STL libs
#include <string>
#include <unordered_map>
#include <time.h>

#include "Particles.h"

//The window we'll be rendering to
SDL_Window* gWindow = NULL;
//The window renderer
SDL_Renderer* gRen = NULL;

void ProcessQuit()
{
	//Destroy gWindow
	SDL_DestroyWindow( gWindow );

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

//Screen dimension constants
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

const std::string kResourcesPath = "resources/";

bool init()
{
	// Try initialize SDL
	if (SDL_Init( SDL_INIT_VIDEO ) < 0)
	{
		printf( "SDL could not be initialized! SDL Error: %s\n", SDL_GetError() );
		return false;
	}

	// Try create window
	gWindow = SDL_CreateWindow( "Fireworks", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
	if (gWindow == NULL)
	{
		printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
		return false;
	}

	// Try create renderer for window
	gRen = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
	if (gRen == NULL)
	{
		printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
		return false;
	}

	// Try init PNG loader
	// Verify if IMG_INIT_PNG presents in return of IMG_Init()
	if (!(IMG_Init( IMG_INIT_PNG ) & IMG_INIT_PNG))
	{
		printf( "SDL_image could not be initialized! SDL_image Error: %s\n", IMG_GetError() );
		return false;
	}

	//Set texture filtering to linear
	if (!SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ))
	{
		printf( "Warning: Linear texture filtering not enabled!" );
		//not a fatal problem, continue
	}

	return true;
}

SDL_Texture* LoadTexture( std::string path )
{
	//The final texture
	SDL_Texture* newTexture = nullptr;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( (kResourcesPath + path).c_str() );
	if (loadedSurface == nullptr)
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
		return nullptr;
	}

	//Create texture from surface pixels
	newTexture = SDL_CreateTextureFromSurface( gRen, loadedSurface );
	if (newTexture == nullptr)
	{
		printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		return nullptr;
	}

	//We don't need this surface anymore
	SDL_FreeSurface( loadedSurface );

	return newTexture;
}

std::unordered_map<std::string, SDL_Texture*> textures;

SDL_Texture* GetTexture( const std::string& name, SDL_Point& size )
{
	//Load each texture only once
	if (textures.find( name ) == textures.end())
	{
		textures[name] = LoadTexture( name );
	}
	SDL_QueryTexture( textures[name], NULL, NULL, &size.x, &size.y );
	return textures[name];
}

int main( int argc, char* args[] )
{
	if (!init())
		return 1;

	srand( time( 0 ) );

	std::unique_ptr<Particles> particles = std::make_unique<Particles>();
	SDL_Rect rect;

	bool quit = false;
	SDL_Event e;

	Uint32 ticks = SDL_GetTicks();

	SDL_RenderPresent( gRen );

	while (!quit)
	{
		while (SDL_PollEvent( &e ) != 0)
		{
			if (e.type == SDL_QUIT || quit)
				ProcessQuit();
			else if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
				case SDL_KeyCode::SDLK_SPACE:
					particles->Shoot( SCREEN_WIDTH, SCREEN_HEIGHT );
					break;

				case SDL_KeyCode::SDLK_ESCAPE:
					quit = true;
					break;

				default:
					break;
				}
			}
		}

		SDL_RenderClear( gRen );
		for (const ParticleBody* particle : particles->GetAllData())
		{
			std::string name;
			switch (particle->kind)
			{
			case Kind::Big:
				name = "big.png";
				break;
			case Kind::Small:
				name = "small.png";
				break;
			case Kind::Spark:
				name = "spark.png";
				break;
			}

			SDL_Point size;
			SDL_Texture* tex = GetTexture( name, size );

			rect.x = (int)particle->x;
			rect.y = (int)particle->y;
			rect.w = size.x;
			rect.h = size.y;

			SDL_RenderCopy( gRen, tex, NULL, &rect );
		}
		SDL_RenderPresent( gRen );
		Uint32 newTicks = SDL_GetTicks();
		particles->Update( (float)(newTicks - ticks) / 1000.f );
		ticks = newTicks;
	}

	return 0;
}