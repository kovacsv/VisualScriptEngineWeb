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

	BrowserAsyncInterface ();

	bool					AreEventsSuspended () const;

	NUIE::MenuCommandPtr	ContextMenuRequest (const NUIE::Point& position, const NUIE::MenuCommandStructure& commands);
	void					ContextMenuResponse (NUIE::NodeEditor& nodeEditor, int commandId);

	void					OnDoubleClick (const NUIE::Point& position);

private:
	State				state;
	ContextMenuData		contextMenuData;
};

#endif
