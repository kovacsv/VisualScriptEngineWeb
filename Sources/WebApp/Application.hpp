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
	virtual ~Application ();

	void						Init ();
	void						Start ();
	void						Shut ();

	void						ResizeWindow (int width, int height);
	void						ExecuteCommand (const char* command);
	void						CreateNode (int nodeIndex, int xPosition, int yPosition);
	void						OpenFile (const char* buffer, int size);
	bool						NeedToSave () const;

	void						ContextMenuResponse (int commandId);
	void						ParameterSettingsResponse (const char* changedParametersJson);

	NUIE::NodeEditor&			GetNodeEditor ();
	BrowserInterface&			GetBrowserInterface ();

	virtual NUIE::UINodePtr		GetNodeByIndex (int nodeIndex, const NUIE::Point& position) const = 0;

private:
	SDL_Window*				window;
	SDL_Renderer*			renderer;

	AppUIEnvironment		uiEnvironment;
	NUIE::NodeEditor		nodeEditor;
	BrowserInterface		browserInterface;
};

#endif
