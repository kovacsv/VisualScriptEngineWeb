#include "BrowserInterface.hpp"
#include "Application.hpp"
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

static Application* gAppForBrowser = nullptr;

void SetAppForBrowser (Application* application)
{
	gAppForBrowser = application;
}

extern "C"
{

void ResizeWindow (int width, int height)
{
	if (gAppForBrowser == nullptr) {
		return;
	}
	gAppForBrowser->ResizeWindow (width, height);
}

void ExecuteCommand (char* command)
{
	if (gAppForBrowser == nullptr) {
		return;
	}
	gAppForBrowser->ExecuteCommand (command);
}

void OpenFile (char* charBuffer, int size)
{
	if (gAppForBrowser == nullptr) {
		return;
	}
	std::vector<char> buffer;
	buffer.assign (charBuffer, charBuffer + size);
	gAppForBrowser->OpenFile (buffer);
}

void CreateNode (int groupId, int nodeId, int xPosition, int yPosition)
{
	if (gAppForBrowser == nullptr) {
		return;
	}
	gAppForBrowser->CreateNode (groupId, nodeId, xPosition, yPosition);
}

bool NeedToSave ()
{
	if (gAppForBrowser == nullptr) {
		return false;
	}
	return gAppForBrowser->NeedToSave ();
}

void ContextMenuResponse (int commandId)
{
	if (gAppForBrowser == nullptr) {
		return;
	}
	gAppForBrowser->ContextMenuResponse (commandId);
}

void ParameterSettingsResponse (char* changedParametersJson)
{
	if (gAppForBrowser == nullptr) {
		return;
	}
	gAppForBrowser->ParameterSettingsResponse (changedParametersJson);
}

};

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

BrowserInterface::ContextMenuData::ContextMenuData () :
	commandStructure (),
	selectedCommandId (InvalidCommandId)
{

}

BrowserInterface::ParameterSettingsData::ParameterSettingsData () :
	paramInterface (nullptr)
{

}

BrowserInterface::BrowserInterface (NUIE::NodeEditor& nodeEditor, ParameterJsonInterface& paramJsonInterface) :
	nodeEditor (nodeEditor),
	paramJsonInterface (paramJsonInterface),
	state (State::Normal),
	contextMenuData (),
	paramSettingsData ()
{

}

bool BrowserInterface::AreEventsSuspended () const
{
	return state != State::Normal;
}

void BrowserInterface::OnAppInitialized (const AppNodeTree& appNodeTree) const
{
	std::string aaa = ConvertNodeTreeToJson (appNodeTree);
#ifdef EMSCRIPTEN
	EM_ASM ({
		OnAppInitialized ($0);
	}, aaa.c_str ());
#endif
}

void BrowserInterface::SaveFile (const std::vector<char>& buffer) const
{
#ifdef EMSCRIPTEN
	EM_ASM ({
		SaveFile ($0, $1);
	}, buffer.data (), buffer.size ());
#else
	(void) buffer;
#endif
}

NUIE::MenuCommandPtr BrowserInterface::ContextMenuRequest (const NUIE::Point& position, const NUIE::MenuCommandStructure& commands)
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

void BrowserInterface::ContextMenuResponse (int commandId)
{
#ifdef EMSCRIPTEN
	state = State::Normal;
	if (commandId != InvalidCommandId) {
		NUIE::MenuCommandPtr command = GetCommandById (contextMenuData.commandStructure, commandId);
		nodeEditor.ExecuteMenuCommand (command);
	}
	contextMenuData.commandStructure = NUIE::MenuCommandStructure ();
#else
	(void) commandId;
#endif
}

void BrowserInterface::DoubleClickRequest (const NUIE::Point& position)
{
#ifdef EMSCRIPTEN
	EM_ASM ({
		OnDoubleClick ($0, $1);
	}, position.GetX (), position.GetY ());
#else
	(void) position;
#endif
}

bool BrowserInterface::ParameterSettingsRequest (NUIE::ParameterInterfacePtr parameters)
{
#ifdef EMSCRIPTEN
	if (state == State::Normal) {
		state = State::WaitingForParametersResponse;
		paramSettingsData.paramInterface = parameters;
		std::string parametersJson = ConvertParametersToJson (parameters, paramJsonInterface);
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

void BrowserInterface::ParameterSettingsResponse (const std::string& changedParametersJsonStr)
{
#ifdef EMSCRIPTEN
	state = State::Normal;
	if (ProcessChangedParametersJson (changedParametersJsonStr, paramJsonInterface, paramSettingsData.paramInterface)) {
		nodeEditor.ApplyParameterChanges (paramSettingsData.paramInterface);
	}
	paramSettingsData.paramInterface = nullptr;
#else
	(void) changedParametersJsonStr;
#endif
}
