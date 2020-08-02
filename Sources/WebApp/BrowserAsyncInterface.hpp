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

	BrowserAsyncInterface ();

	bool	AreEventsSuspended () const;

	void	ContextMenuRequest (const NUIE::Point& position);
	void	ContextMenuResponse (int commandIndex);

private:
	State	state;
};

#endif
