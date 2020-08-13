#include "BrowserAsyncInterface.hpp"
#include "JSONConversion.hpp"

#ifdef EMSCRIPTEN
#include <emscripten.h>
#define UNUSED_IN_CPP
#else
#ifdef _MSC_VER
#define UNUSED_IN_CPP __pragma (warning (suppress: 4505))
#else
#define UNUSED_IN_CPP __attribute__ ((unused))
#endif
#endif

UNUSED_IN_CPP
static NUIE::MenuCommandPtr GetCommandById (const std::vector<NUIE::MenuCommandPtr>& commandList, int commandId, int& currentId)
{
	for (size_t i = 0; i < commandList.size (); i++) {
		const NUIE::MenuCommandPtr& command = commandList[i];
		if (command->HasChildCommands ()) {
			std::vector<NUIE::MenuCommandPtr> childCommandList = command->GetChildCommands ();
			NUIE::MenuCommandPtr found = GetCommandById (childCommandList, commandId, currentId);
			if (found != nullptr) {
				return found;
			}
		} else {
			if (commandId == currentId) {
				return command;
			}
		}
		currentId++;
	}
	return nullptr;
}

UNUSED_IN_CPP
static NUIE::MenuCommandPtr GetCommandById (const NUIE::MenuCommandStructure& commands, int commandId)
{
	int currentId = FirstCommandId;
	std::vector<NUIE::MenuCommandPtr> commandList = commands.GetCommands ();
	return GetCommandById (commandList, commandId, currentId);
}

BrowserAsyncInterface::ContextMenuData::ContextMenuData () :
	position (),
	selectedCommandId (InvalidCommandId)
{

}

BrowserAsyncInterface::BrowserAsyncInterface () :
	state (State::Normal),
	contextMenuData ()
{

}

bool BrowserAsyncInterface::AreEventsSuspended () const
{
	return state != State::Normal;
}

NUIE::MenuCommandPtr BrowserAsyncInterface::ContextMenuRequest (const NUIE::Point& position, const NUIE::MenuCommandStructure& commands)
{
	if (commands.IsEmpty ()) {
		return nullptr;
	}
#ifdef EMSCRIPTEN
	if (state == State::Normal) {
		state = State::WaitingForContextMenuResponse;
		contextMenuData.position = position;
		contextMenuData.selectedCommandId = InvalidCommandId;
		std::string commandsJson = ConvertMenuCommandsToJson (commands);
		EM_ASM ({
			ContextMenuRequest ($0, $1, $2);
		}, position.GetX (), position.GetY (), commandsJson.c_str ());
		return nullptr;
	} else if (state == State::ContextMenuResponseArrived){
		state = State::Normal;
		return GetCommandById (commands, contextMenuData.selectedCommandId);
	}
#else
	(void) position;
#endif
	return nullptr;
}

void BrowserAsyncInterface::ContextMenuResponse (NUIE::NodeEditor& nodeEditor, int commandId)
{
#ifdef EMSCRIPTEN
	state = State::ContextMenuResponseArrived;
	contextMenuData.selectedCommandId = commandId;

	// Trigger context menu callback again
	NUIE::Point position = contextMenuData.position;
	nodeEditor.OnMouseDown (NUIE::EmptyModifierKeys, NUIE::MouseButton::Right, (int) position.GetX (), (int) position.GetY ());
	nodeEditor.OnMouseUp (NUIE::EmptyModifierKeys, NUIE::MouseButton::Right, (int) position.GetX (), (int) position.GetY ());
#else
	(void) nodeEditor;
	(void) commandId;
#endif
}

void BrowserAsyncInterface::OnDoubleClick (const NUIE::Point& position)
{
#ifdef EMSCRIPTEN
	EM_ASM ({
		OnDoubleClick ($0, $1);
	}, position.GetX (), position.GetY ());
#else
	(void) position;
#endif
}
