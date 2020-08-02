#include "BrowserAsyncInterface.hpp"

BrowserAsyncInterface::BrowserAsyncInterface () :
	state (State::Normal)
{

}

bool BrowserAsyncInterface::AreEventsSuspended () const
{
	return state != State::Normal;
}

void BrowserAsyncInterface::ContextMenuRequest (const NUIE::Point& /*position*/)
{
	state = State::WaitingForContextMenu;
}

void BrowserAsyncInterface::ContextMenuResponse (int /*commandIndex*/)
{
	state = State::Normal;
}
