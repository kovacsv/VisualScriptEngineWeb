#include <iostream>
#ifdef EMSCRIPTEN
	#include <emscripten.h>
#endif
#include <SDL.h>

class SDLContext
{
public:
	SDLContext () :
		renderer (nullptr)
	{

	}

	SDL_Renderer*	renderer;
};

void MainLoop (void* arg)
{
	SDLContext* context = (SDLContext*) arg;
	SDL_SetRenderDrawColor (context->renderer, 0, 255, 0, 255);
	SDL_RenderClear (context->renderer);

	SDL_RenderPresent (context->renderer);
}

int main (int, char**)
{
	SDLContext context;

	SDL_Init (SDL_INIT_VIDEO);
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_CreateWindowAndRenderer (255, 255, 0, &window, &renderer);
	
	context.renderer = renderer;

#ifdef EMSCRIPTEN
	emscripten_set_main_loop_arg (MainLoop, &context, 0, true);
#endif

	SDL_DestroyRenderer (renderer);
	SDL_DestroyWindow (window);
	SDL_Quit ();

	std::cout << ("hello") << std::endl;
	return EXIT_SUCCESS;
}
