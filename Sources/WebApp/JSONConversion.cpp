#include "JSONConversion.hpp"
#include "NE_StringUtils.hpp"

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

using namespace rapidjson;

const int InvalidCommandId = -1;
const int FirstCommandId = 1; 

static void AddString (Value& obj, Document::AllocatorType& allocator, const GenericStringRef<char>& key, const std::wstring& value)
{
	std::string str = NE::WStringToString (value);
	Value jsonValue;
	jsonValue.SetString (str.c_str (), (SizeType) str.length (), allocator);
	obj.AddMember (key, jsonValue, allocator);
}

static void AddInt (Value& obj, Document::AllocatorType& allocator, const GenericStringRef<char>& key, int value)
{
	Value jsonValue;
	jsonValue.SetInt (value);
	obj.AddMember (key, jsonValue, allocator);
}

static void AddBool (Value& obj, Document::AllocatorType& allocator, const GenericStringRef<char>& key, bool value)
{
	Value jsonValue;
	jsonValue.SetBool (value);
	obj.AddMember (key, jsonValue, allocator);
}

static void AddCommandsToJson (std::vector<NUIE::MenuCommandPtr> commandList, int& currentId, Value& commands, Document::AllocatorType& allocator)
{
	for (size_t i = 0; i < commandList.size (); i++) {
		const NUIE::MenuCommandPtr& command = commandList[i];
		Value commandObj (kObjectType);
		
		AddString (commandObj, allocator, "name", command->GetName ());
		AddInt (commandObj, allocator, "id", currentId);
		AddBool (commandObj, allocator, "isChecked", command->IsChecked ());
		if (command->HasChildCommands ()) {
			Value childCommandArr;
			childCommandArr.SetArray ();
			std::vector<NUIE::MenuCommandPtr> childCommandList = command->GetChildCommands ();
			AddCommandsToJson (childCommandList, currentId, childCommandArr, allocator);
			commandObj.AddMember ("commands", childCommandArr, allocator);
		}
		commands.PushBack (commandObj, allocator);
		currentId++;
	}
}

std::string ConvertMenuCommandsToJson (const NUIE::MenuCommandStructure& commands)
{
	Document doc;
	Document::AllocatorType& allocator = doc.GetAllocator ();
	doc.SetObject ();

	Value commandArr;
	commandArr.SetArray ();

	std::vector<NUIE::MenuCommandPtr> commandList = commands.GetCommands ();
	int currentId = FirstCommandId;
	AddCommandsToJson (commandList, currentId, commandArr, allocator);
	doc.AddMember ("commands", commandArr, allocator);

	StringBuffer buffer;
	Writer<StringBuffer> writer (buffer);
	doc.Accept (writer);

	std::string docString (buffer.GetString ());
	return docString;
}
