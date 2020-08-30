#include "JSONConversion.hpp"
#include "NE_SingleValues.hpp"
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
	jsonValue.SetString (value.c_str (), allocator);
	obj.AddMember (key, jsonValue, allocator);
}

static void AddString (Value& obj, Document::AllocatorType& allocator, const GenericStringRef<char>& key, const std::wstring& value)
{
	std::string str = NE::WStringToString (value);
	AddString (obj, allocator, key, str);
}

static void AddBoolean (Value& obj, Document::AllocatorType& allocator, const GenericStringRef<char>& key, bool value)
{
	Value jsonValue;
	jsonValue.SetBool (value);
	obj.AddMember (key, jsonValue, allocator);
}

static void AddInteger (Value& obj, Document::AllocatorType& allocator, const GenericStringRef<char>& key, int value)
{
	Value jsonValue;
	jsonValue.SetInt (value);
	obj.AddMember (key, jsonValue, allocator);
}

static void AddDouble (Value& obj, Document::AllocatorType& allocator, const GenericStringRef<char>& key, double value)
{
	Value jsonValue;
	jsonValue.SetDouble (value);
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
		AddInteger (commandObj, allocator, "id", currentId);
		bool isChecked = false;
		if (command->HasChildCommands ()) {
			Value childCommandArr;
			childCommandArr.SetArray ();
			std::vector<NUIE::MenuCommandPtr> childCommandList = command->GetChildCommands ();
			AddCommandsToJson (childCommandList, currentId, childCommandArr, allocator);
			commandObj.AddMember ("commands", childCommandArr, allocator);
		} else {
			isChecked = command->IsChecked ();
		}
		AddBoolean (commandObj, allocator, "isChecked", isChecked);
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

		Value valueObj;
		valueObj.SetObject ();
		NE::ValueConstPtr value = parameters->GetParameterValue (i);
		NUIE::ParameterType type = parameters->GetParameterType (i);
		if (type == NUIE::ParameterType::Boolean) {
			if (NE::Value::IsType<NE::BooleanValue> (value)) {
				AddBoolean (valueObj, allocator, "boolVal", NE::BooleanValue::Get (value));
			}
		} else if (type == NUIE::ParameterType::Integer) {
			if (NE::Value::IsType<NE::IntValue> (value)) {
				AddInteger (valueObj, allocator, "intVal", NE::IntValue::Get (value));
			}
		} else if (type == NUIE::ParameterType::Float) {
			if (NE::Value::IsType<NE::FloatValue> (value)) {
				AddDouble (valueObj, allocator, "numVal", NE::FloatValue::Get (value));
			}
		} else if (type == NUIE::ParameterType::Double) {
			if (NE::Value::IsType<NE::DoubleValue> (value)) {
				AddDouble (valueObj, allocator, "numVal", NE::DoubleValue::Get (value));
			}
		} else if (type == NUIE::ParameterType::String) {
			if (NE::Value::IsType<NE::StringValue> (value)) {
				AddString (valueObj, allocator, "strVal", NE::StringValue::Get (value));
			}
		} else if (type == NUIE::ParameterType::Enumeration) {
			if (NE::Value::IsType<NE::IntValue> (value)) {
				AddInteger (valueObj, allocator, "intVal", NE::IntValue::Get (value));
				std::vector<std::wstring> choices = parameters->GetParameterValueChoices (i);
				Value choicesArr;
				choicesArr.SetArray ();
				for (const std::wstring& choice : choices) {
					Value choiceStrVal;
					std::string choiceStr = NE::WStringToString (choice);
					choiceStrVal.SetString (choiceStr.c_str (), (SizeType) choiceStr.length (), allocator);
					choicesArr.PushBack (choiceStrVal, allocator);
				}
				valueObj.AddMember ("choices", choicesArr, allocator);
			}
		}

		paramObj.AddMember ("value", valueObj, allocator);
		paramArr.PushBack (paramObj, allocator);
	}

	doc.AddMember ("parameters", paramArr, allocator);
	return DocumentToString (doc);
}

std::string ConvertNodeTreeToJson (const AppNodeTree& appNodeTree)
{
	Document doc;
	Document::AllocatorType& allocator = doc.GetAllocator ();
	doc.SetArray ();

	const NUIE::NodeTree& nodeTree = appNodeTree.GetNodeTree ();
	const std::vector<NUIE::NodeTree::Group>& groups = nodeTree.GetGroups ();

	for (size_t groupIndex = 0; groupIndex < groups.size (); groupIndex++) {
		const NUIE::NodeTree::Group& group = groups[groupIndex];

		Value groupObj;
		groupObj.SetObject ();
		AddString (groupObj, allocator, "name", group.GetName ());

		Value itemArr;
		itemArr.SetArray ();
		const std::vector<NUIE::NodeTree::Item>& items = group.GetItems ();
		for (size_t nodeIndex = 0; nodeIndex < items.size (); nodeIndex++) {
			const NUIE::NodeTree::Item& item = items[nodeIndex];
			Value itemObj;
			itemObj.SetObject ();
			AddString (itemObj, allocator, "name", item.GetName ());
			AddString (itemObj, allocator, "icon", appNodeTree.GetIcon (groupIndex, nodeIndex));
			AddInteger (itemObj, allocator, "groupId", (int) groupIndex);
			AddInteger (itemObj, allocator, "nodeId", (int) nodeIndex);
			itemArr.PushBack (itemObj, allocator);
		}
		groupObj.AddMember ("nodes", itemArr, allocator);
		doc.PushBack (groupObj, allocator);
	}

	return DocumentToString (doc);
}

bool ProcessChangedParametersJson (const std::string& changeParametersJsonStr, NUIE::ParameterInterfacePtr& parameters)
{
	if (changeParametersJsonStr.empty ()) {
		return false;
	}

	Document doc;
	doc.Parse (changeParametersJsonStr.c_str ());
	if (doc.HasParseError ()) {
		return false;
	}
	if (!doc.HasMember ("params")) {
		return false;
	}

	bool wasChange = false;
	Value::Array paramsArr = doc["params"].GetArray ();
	for (SizeType i = 0; i < paramsArr.Size (); i++) {
		const Value& paramVal = paramsArr[i];
		if (paramVal.IsNull ()) {
			continue;
		}
		const Value& valueObj = paramVal["value"];
		NUIE::ParameterType type = parameters->GetParameterType (i);
		NE::ValuePtr resultVal = nullptr;
		if (type == NUIE::ParameterType::Boolean) {
			bool boolVal = valueObj["boolVal"].GetBool ();
			resultVal = NE::ValuePtr (new NE::BooleanValue (boolVal));
		} else if (type == NUIE::ParameterType::Integer) {
			int intVal = valueObj["intVal"].GetInt ();
			resultVal = NE::ValuePtr (new NE::IntValue (intVal));
		} else if (type == NUIE::ParameterType::Float) {
			float numVal = valueObj["numVal"].GetFloat ();
			resultVal = NE::ValuePtr (new NE::FloatValue (numVal));
		} else if (type == NUIE::ParameterType::Double) {
			double numVal = valueObj["numVal"].GetDouble ();
			resultVal = NE::ValuePtr (new NE::DoubleValue (numVal));
		} else if (type == NUIE::ParameterType::String) {
			std::string strVal = valueObj["strVal"].GetString ();
			resultVal = NE::ValuePtr (new NE::StringValue (NE::StringToWString (strVal)));
		} else if (type == NUIE::ParameterType::Enumeration) {
			int intVal = valueObj["intVal"].GetInt ();
			resultVal = NE::ValuePtr (new NE::IntValue (intVal));
		}
		if (resultVal != nullptr) {
			if (parameters->IsValidParameterValue (i, resultVal)) {
				parameters->SetParameterValue (i, resultVal);
				wasChange = true;
			}
		}
	}
	
	return wasChange;
}
