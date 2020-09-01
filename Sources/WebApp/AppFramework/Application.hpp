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

	void					Init (const std::string& windowName);
	void					Start ();
	void					Shut ();

	void					ResizeWindow (int width, int height);
	void					ExecuteCommand (const char* command);
	void					CreateNode (int groupId, int nodeId, int xPosition, int yPosition);

	bool					SaveFile ();
	bool					OpenFile (const std::vector<char>& buffer);
	bool					NeedToSave () const;

	void					ContextMenuResponse (int commandId);
	void					ParameterSettingsResponse (const char* changedParametersJson);

	NUIE::NodeEditor&		GetNodeEditor ();
	BrowserInterface&		GetBrowserInterface ();

private:
	CustomAppInterface&		customAppInterface;
	BrowserInterface		browserInterface;

	AppNodeTree				appNodeTree;
	AppUIEnvironment		uiEnvironment;
	NUIE::NodeEditor		nodeEditor;

	SDL_Window*				window;
	SDL_Renderer*			renderer;
};

#endif
