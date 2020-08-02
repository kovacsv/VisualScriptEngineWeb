#ifndef BROWSER_ASYNC_INTERFACE_HPP
#define BROWSER_ASYNC_INTERFACE_HPP

#include "NUIE_NodeEditor.hpp"

class BrowserAsyncInterface
{
public:
	enum class State
	{
		Normal,
		WaitingForContextMenu
	};

	class ContextMenuData
	{
	public:
		ContextMenuData ();

		NUIE::Point position;
	};

	BrowserAsyncInterface ();

	bool					AreEventsSuspended () const;

	void					ContextMenuRequest (const NUIE::Point& position, const NUIE::MenuCommandStructure& commands);
	void					ContextMenuResponse (int commandIndex);
	const ContextMenuData&	GetContextMenuData () const;

private:
	State				state;
	ContextMenuData		contextMenuData;
};

#endif
