#ifndef BROWSER_ASYNC_INTERFACE_HPP
#define BROWSER_ASYNC_INTERFACE_HPP

#include "NUIE_NodeEditor.hpp"

class BrowserAsyncInterface
{
public:
	enum class State
	{
		Normal,
		WaitingForContextMenuResponse,
		ContextMenuResponseArrived,
		WaitingForParametersResponse,
		ParametersResponseArrived
	};

	class ContextMenuData
	{
	public:
		ContextMenuData ();

		int				selectedCommandId;
	};

	class ParameterSettingsData
	{
	public:
		ParameterSettingsData ();

		NUIE::Point		position;
	};

	BrowserAsyncInterface (NUIE::NodeEditor& nodeEditor);

	bool					AreEventsSuspended () const;

	NUIE::MenuCommandPtr	ContextMenuRequest (const NUIE::Point& position, const NUIE::MenuCommandStructure& commands);
	void					ContextMenuResponse (int mouseX, int mouseY, int commandId);

	void					DoubleClickRequest (const NUIE::Point& position);

	bool					ParameterSettingsRequest (NUIE::ParameterInterfacePtr parameters);
	void					ParameterSettingsResponse (int mouseX, int mouseY);

private:
	NUIE::NodeEditor&	nodeEditor;

	State				state;
	ContextMenuData		contextMenuData;
};

#endif
