#include <iostream>
#include <emscripten.h>
#include <SDL.h>

int main (int, char**)
{
	SDL_Init (SDL_INIT_VIDEO);
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_CreateWindowAndRenderer (255, 255, 0, &window, &renderer);
	
	SDL_DestroyRenderer (renderer);
	SDL_DestroyWindow (window);
	SDL_Quit ();

	std::cout << ("hello") << std::endl;
	return EXIT_SUCCESS;
}
