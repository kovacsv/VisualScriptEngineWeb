#include "JSONConversion.hpp"
#include "NE_StringUtils.hpp"

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

using namespace rapidjson;

const int InvalidCommandId = -1;
const int FirstCommandId = 1; 

static void AddString (Value& obj, Document::AllocatorType& allocator, const GenericStringRef<char>& key, const std::string& value)
{
	Value jsonValue;
	jsonValue.SetString (value.c_str (), (SizeType) value.length (), allocator);
	obj.AddMember (key, jsonValue, allocator);
}

static void AddString (Value& obj, Document::AllocatorType& allocator, const GenericStringRef<char>& key, const std::wstring& value)
{
	std::string str = NE::WStringToString (value);
	AddString (obj, allocator, key, str);
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

static std::string DocumentToString (const Document& doc)
{
	StringBuffer buffer;
	Writer<StringBuffer> writer (buffer);
	doc.Accept (writer);

	std::string docString (buffer.GetString ());
	return docString;
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

	return DocumentToString (doc);
}

std::string ConvertParametersToJson (const NUIE::ParameterInterfacePtr& parameters)
{
	Document doc;
	Document::AllocatorType& allocator = doc.GetAllocator ();
	doc.SetObject ();

	Value paramArr;
	paramArr.SetArray ();

	for (size_t i = 0; i < parameters->GetParameterCount (); i++) {
		Value paramObj;
		paramObj.SetObject ();
		AddString (paramObj, allocator, "name", parameters->GetParameterName (i));
		AddString (paramObj, allocator, "type", parameters->GetParameterType (i).GetId ());
		paramArr.PushBack (paramObj, allocator);
	}

	doc.AddMember ("parameters", paramArr, allocator);
	return DocumentToString (doc);
}
