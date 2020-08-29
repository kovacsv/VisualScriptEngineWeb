#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <SDL.h>
#include <SDL_ttf.h>

#include "AppEnvironment.hpp"
#include "BrowserInterface.hpp"

class Application
{
public:
	Application ();

	void					Init ();
	void					Start ();
	void					Shut ();

	SDL_Rect				GetWindowRect () const;
	void					ResizeWindow (int width, int height);

	NUIE::NodeEditor&		GetNodeEditor ();
	BrowserInterface&	GetBrowserInterface ();

private:
	SDL_Window*				window;
	SDL_Renderer*			renderer;

	AppUIEnvironment		uiEnvironment;
	NUIE::NodeEditor		nodeEditor;
	BrowserInterface	browserInterface;
};

#endif
