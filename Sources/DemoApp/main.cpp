#include <iostream>
#ifdef EMSCRIPTEN
	#include <emscripten.h>
#endif
#include <SDL.h>
#include <SDL_ttf.h>

class SDLContext
{
public:
	SDLContext () :
		renderer (nullptr)
	{

	}

	SDL_Renderer*	renderer;
};

static int x = 0, y = 0;

static bool MainLoop (SDLContext* context)
{
	SDL_Event sdlEvent;
	if (SDL_PollEvent (&sdlEvent)) {
		switch (sdlEvent.type) {
			case SDL_QUIT:
				return false;
			case SDL_MOUSEMOTION:
				{
					x = sdlEvent.motion.x;
					y = sdlEvent.motion.y;
				}
				break;
		}
	}

	SDL_SetRenderDrawColor (context->renderer, 0, 255, 0, 255);
	SDL_RenderClear (context->renderer);

	SDL_SetRenderDrawColor (context->renderer, 0, 0, 255, 255);
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 100; j++) {
			SDL_Rect r;
			r.x = x + i * 10;
			r.y = y + j * 10;
			r.w = 5;
			r.h = 5;
			SDL_RenderFillRect (context->renderer, &r);
		}
	}

	SDL_RenderDrawLine (context->renderer, 10, 10, 400, 100);

	TTF_Font* font = TTF_OpenFont ("Assets/FreeSans.ttf", 25);
	SDL_Color color = { 0, 0, 0, 255 };
	SDL_Surface * surface = TTF_RenderText_Blended (font, "Black Dog Runs at Night", color);
	SDL_Texture * texture = SDL_CreateTextureFromSurface (context->renderer, surface);

	int texW = 0;
	int texH = 0;
	SDL_QueryTexture (texture, NULL, NULL, &texW, &texH);
	SDL_Rect dstrect = { 0, 0, texW, texH };

	SDL_RenderCopy (context->renderer, texture, NULL, &dstrect);
	SDL_DestroyTexture (texture);
	SDL_FreeSurface (surface);
	TTF_CloseFont (font);

	SDL_RenderPresent (context->renderer);

	return true;
}

#ifdef EMSCRIPTEN
static void EmscriptenMainLoop (void* arg)
{
	SDLContext* context = (SDLContext*) arg;
	MainLoop (context);
}
#endif

int main (int, char**)
{
	SDLContext context;

	SDL_Init (SDL_INIT_VIDEO);
	TTF_Init ();

	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_CreateWindowAndRenderer (640, 480, 0, &window, &renderer);
	
	context.renderer = renderer;

#ifdef EMSCRIPTEN
	emscripten_set_main_loop_arg (EmscriptenMainLoop, &context, 0, true);
#else
	while (true) {
		if (!MainLoop (&context)) {
			break;
		}
	}
#endif

	SDL_DestroyRenderer (renderer);
	SDL_DestroyWindow (window);

	TTF_Quit ();
	SDL_Quit ();

	std::cout << ("hello") << std::endl;
	return EXIT_SUCCESS;
}
