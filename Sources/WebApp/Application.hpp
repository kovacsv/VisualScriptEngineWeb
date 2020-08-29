#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <SDL.h>
#include <SDL_ttf.h>

#include "AppEnvironment.hpp"

class Application
{
public:
	Application (CustomAppInterface& customAppInterface);
	virtual ~Application ();

	void					Init ();
	void					Start ();
	void					Shut ();

	void					ResizeWindow (int width, int height);
	void					ExecuteCommand (const char* command);
	void					CreateNode (int nodeIndex, int xPosition, int yPosition);
	void					OpenFile (const char* buffer, int size);
	bool					NeedToSave () const;

	void					ContextMenuResponse (int commandId);
	void					ParameterSettingsResponse (const char* changedParametersJson);

	NUIE::NodeEditor&		GetNodeEditor ();
	BrowserInterface&		GetBrowserInterface ();

private:
	CustomAppInterface&		customAppInterface;
	BrowserInterface		browserInterface;

	AppUIEnvironment		uiEnvironment;
	NUIE::NodeEditor		nodeEditor;

	SDL_Window*				window;
	SDL_Renderer*			renderer;
};

#endif
