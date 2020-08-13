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
		WaitingForParametersResponse
	};

	class ContextMenuData
	{
	public:
		ContextMenuData ();

		NUIE::MenuCommandStructure	commandStructure;
		int							selectedCommandId;
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
	void					ContextMenuResponse (int commandId);

	void					DoubleClickRequest (const NUIE::Point& position);

	bool					ParameterSettingsRequest (NUIE::ParameterInterfacePtr parameters);
	void					ParameterSettingsResponse ();

private:
	NUIE::NodeEditor&	nodeEditor;

	State				state;
	ContextMenuData		contextMenuData;
};

#endif
