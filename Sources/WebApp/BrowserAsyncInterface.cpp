#include "BrowserAsyncInterface.hpp"

#include "NE_StringUtils.hpp"

#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif

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
			std::vector<NUIE::MenuCommandPtr> childCommandList;
			command->EnumerateChildCommands ([&] (const NUIE::MenuCommandPtr& childCommand) {
				childCommandList.push_back (childCommand);
			});
			AddCommandsToJson (childCommandList, json);
		}
		json += L"}";
		if (i < commandList.size () - 1) {
			json += L",";
		}
	}
	json += L"]";
}

static std::string ConvertMenuCommandsToJson (const NUIE::MenuCommandStructure& commands)
{
	std::wstring json = L"";
	json += L"{";

	std::vector<NUIE::MenuCommandPtr> commandList;
	commands.EnumerateCommands ([&] (const NUIE::MenuCommandPtr& command) {
		commandList.push_back (command);
	});
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

void BrowserAsyncInterface::ContextMenuRequest (const NUIE::Point& position, const NUIE::MenuCommandStructure& commands)
{
	state = State::WaitingForContextMenu;
	contextMenuData.position = position;

	std::string commandsJson = ConvertMenuCommandsToJson (commands);
#ifdef EMSCRIPTEN
	EM_ASM ({
		ContextMenuRequest ($0);
	}, commandsJson.c_str ());
#else
	(void) commands;
#endif
}

void BrowserAsyncInterface::ContextMenuResponse (int /*commandIndex*/)
{
	state = State::Normal;
}

const BrowserAsyncInterface::ContextMenuData& BrowserAsyncInterface::GetContextMenuData () const
{
	return contextMenuData;
}
