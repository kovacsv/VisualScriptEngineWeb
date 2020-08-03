#include "BrowserAsyncInterface.hpp"

#include "NE_StringUtils.hpp"

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

static const int InvalidCommandId = -1;
static const int FirstCommandId = 1;

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

UNUSED_IN_CPP
static void AddCommandsToJson (std::vector<NUIE::MenuCommandPtr> commandList, int& currentId, std::wstring& json)
{
	json += L"\"commands\" : [";
	for (size_t i = 0; i < commandList.size (); i++) {
		const NUIE::MenuCommandPtr& command = commandList[i];
		json += L"{";
		json += L"\"name\" : \"" + NE::ReplaceAll (command->GetName (), L"\"", L"\\\"") + L"\",";
		json += L"\"id\" : " + std::to_wstring (currentId) + L",";
		json += L"\"isChecked\" : ";
		json += (command->IsChecked () ? L"true" : L"false");
		if (command->HasChildCommands ()) {
			json += L",";
			std::vector<NUIE::MenuCommandPtr> childCommandList = command->GetChildCommands ();
			AddCommandsToJson (childCommandList, currentId, json);
		}
		json += L"}";
		if (i < commandList.size () - 1) {
			json += L",";
		}
		currentId++;
	}
	json += L"]";
}

UNUSED_IN_CPP
static std::string ConvertMenuCommandsToJson (const NUIE::MenuCommandStructure& commands)
{
	std::wstring json = L"";
	json += L"{";

	std::vector<NUIE::MenuCommandPtr> commandList = commands.GetCommands ();
	int currentId = FirstCommandId;
	AddCommandsToJson (commandList, currentId, json);

	json += L"}";
	return NE::WStringToString (json);
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
