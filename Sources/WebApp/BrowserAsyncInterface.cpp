#include "BrowserAsyncInterface.hpp"

#include "NE_StringUtils.hpp"

#ifdef EMSCRIPTEN
#include <emscripten.h>
#define UNUSED_IN_CPP
#else
#define UNUSED_IN_CPP __pragma (warning (suppress: 4505))
#endif

static std::vector<NUIE::MenuCommandPtr> GetCommandList (const NUIE::MenuCommandStructure& commands)
{
	std::vector<NUIE::MenuCommandPtr> commandList;
	commands.EnumerateCommands ([&] (const NUIE::MenuCommandPtr& command) {
		commandList.push_back (command);
	});
	return commandList;
}

static std::vector<NUIE::MenuCommandPtr> GetChildCommandList (const NUIE::MenuCommandPtr& command)
{
	std::vector<NUIE::MenuCommandPtr> commandList;
	command->EnumerateChildCommands ([&] (const NUIE::MenuCommandPtr& childCommand) {
		commandList.push_back (childCommand);
	});
	return commandList;
}

static NUIE::MenuCommandPtr GetCommandByName (const std::vector<NUIE::MenuCommandPtr>& commandList, const std::wstring& commandName)
{
	for (size_t i = 0; i < commandList.size (); i++) {
		const NUIE::MenuCommandPtr& command = commandList[i];
		if (command->HasChildCommands ()) {
			std::vector<NUIE::MenuCommandPtr> childCommandList = GetChildCommandList (command);
			NUIE::MenuCommandPtr found = GetCommandByName (childCommandList, commandName);
			if (found != nullptr) {
				return found;
			}
		} else {
			if (command->GetName () == commandName) {
				return command;
			}
		}
	}
	return nullptr;
}

UNUSED_IN_CPP
static NUIE::MenuCommandPtr GetCommandByName (const NUIE::MenuCommandStructure& commands, const std::wstring& commandName)
{
	std::vector<NUIE::MenuCommandPtr> commandList = GetCommandList (commands);
	return GetCommandByName (commandList, commandName);
}

static void AddCommandsToJson (std::vector<NUIE::MenuCommandPtr> commandList, std::wstring& json)
{
	json += L"\"commands\" : [";
	for (size_t i = 0; i < commandList.size (); i++) {
		const NUIE::MenuCommandPtr& command = commandList[i];
		json += L"{";
		json += L"\"name\" : \"" + command->GetName () + L"\",";
		json += L"\"isChecked\" : ";
		json += (command->IsChecked () ? L"true" : L"false");
		if (command->HasChildCommands ()) {
			json += L",";
			std::vector<NUIE::MenuCommandPtr> childCommandList = GetChildCommandList (command);
			AddCommandsToJson (childCommandList, json);
		}
		json += L"}";
		if (i < commandList.size () - 1) {
			json += L",";
		}
	}
	json += L"]";
}

UNUSED_IN_CPP
static std::string ConvertMenuCommandsToJson (const NUIE::MenuCommandStructure& commands)
{
	std::wstring json = L"";
	json += L"{";

	std::vector<NUIE::MenuCommandPtr> commandList = GetCommandList (commands);
	AddCommandsToJson (commandList, json);

	json += L"}";
	return NE::WStringToString (json);
}

BrowserAsyncInterface::ContextMenuData::ContextMenuData () :
	position ()
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
#ifdef EMSCRIPTEN
	if (state == State::WaitingForContextMenu) {
		state = State::Normal;
		return GetCommandByName (commands, contextMenuData.selectedCommandName);
	} else {
		state = State::WaitingForContextMenu;
		contextMenuData.position = position;
		contextMenuData.selectedCommandName = L"";
		std::string commandsJson = ConvertMenuCommandsToJson (commands);
		EM_ASM ({
			ContextMenuRequest ($0);
		}, commandsJson.c_str ());
		(void) commands;
		return nullptr;
	}
#else
	(void) position;
	(void) commands;
	return nullptr;
#endif
}

void BrowserAsyncInterface::ContextMenuResponse (NUIE::NodeEditor& nodeEditor, const char* commandNameCStr)
{
	std::string commandNameStr = commandNameCStr;
	contextMenuData.selectedCommandName = NE::StringToWString (commandNameStr);

	const NUIE::Point& position = contextMenuData.position;
	nodeEditor.OnMouseDown (NUIE::EmptyModifierKeys, NUIE::MouseButton::Right, (int) position.GetX (), (int) position.GetY ());
	nodeEditor.OnMouseUp (NUIE::EmptyModifierKeys, NUIE::MouseButton::Right, (int) position.GetX (), (int) position.GetY ());
}
