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
		ContextMenuResponseArrived
	};

	class ContextMenuData
	{
	public:
		ContextMenuData ();

		NUIE::Point		position;
		int				selectedCommandId;
	};

	BrowserAsyncInterface (NUIE::NodeEditor& nodeEditor);

	bool					AreEventsSuspended () const;

	NUIE::MenuCommandPtr	ContextMenuRequest (const NUIE::Point& position, const NUIE::MenuCommandStructure& commands);
	void					ContextMenuResponse (int commandId);

	void					OnDoubleClick (const NUIE::Point& position);

private:
	NUIE::NodeEditor&	nodeEditor;

	State				state;
	ContextMenuData		contextMenuData;
};

#endif
