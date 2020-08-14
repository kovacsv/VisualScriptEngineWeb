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
	commandStructure (),
	selectedCommandId (InvalidCommandId)
{

}

BrowserAsyncInterface::ParameterSettingsData::ParameterSettingsData () :
	paramInterface (nullptr)
{

}

BrowserAsyncInterface::BrowserAsyncInterface (NUIE::NodeEditor& nodeEditor) :
	nodeEditor (nodeEditor),
	state (State::Normal),
	contextMenuData (),
	paramSettingsData ()
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
		contextMenuData.commandStructure = commands;
		contextMenuData.selectedCommandId = InvalidCommandId;
		std::string commandsJson = ConvertMenuCommandsToJson (commands);
		EM_ASM ({
			ContextMenuRequest ($0, $1, $2);
		}, position.GetX (), position.GetY (), commandsJson.c_str ());
		return nullptr;
	}
#else
	(void) position;
#endif
	return nullptr;
}

void BrowserAsyncInterface::ContextMenuResponse (int commandId)
{
#ifdef EMSCRIPTEN
	state = State::Normal;
	NUIE::MenuCommandPtr command = GetCommandById (contextMenuData.commandStructure, commandId);
	nodeEditor.ExecuteMenuCommand (command);
	contextMenuData.commandStructure = NUIE::MenuCommandStructure ();
#else
	(void) commandId;
#endif
}

void BrowserAsyncInterface::DoubleClickRequest (const NUIE::Point& position)
{
#ifdef EMSCRIPTEN
	EM_ASM ({
		OnDoubleClick ($0, $1);
	}, position.GetX (), position.GetY ());
#else
	(void) position;
#endif
}

bool BrowserAsyncInterface::ParameterSettingsRequest (NUIE::ParameterInterfacePtr parameters)
{
#ifdef EMSCRIPTEN
	if (state == State::Normal) {
		state = State::WaitingForParametersResponse;
		paramSettingsData.paramInterface = parameters;
		std::string parametersJson = ConvertParametersToJson (parameters);
		EM_ASM ({
			ParameterSettingsRequest ($0);
		}, parametersJson.c_str ());
		return false;
	}
#else
	(void) parameters;
#endif

	return false;
}

void BrowserAsyncInterface::ParameterSettingsResponse (const std::string& changedParametersJsonStr)
{
#ifdef EMSCRIPTEN
	state = State::Normal;
	if (ProcessChangedParametersJson (changedParametersJsonStr, paramSettingsData.paramInterface)) {
		nodeEditor.ApplyParameterChanges (paramSettingsData.paramInterface);
	}
	paramSettingsData.paramInterface = nullptr;
#else
	(void) changedParametersJsonStr;
#endif
}
