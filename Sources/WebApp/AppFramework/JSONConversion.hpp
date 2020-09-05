#ifndef JSON_CONVERSION_HPP
#define JSON_CONVERSION_HPP

#include "NUIE_MenuCommands.hpp"
#include "NUIE_ParameterInterface.hpp"
#include "AppNodeTree.hpp"

#include "rapidjson/document.h"

extern const int InvalidCommandId;
extern const int FirstCommandId;

class ParameterJsonInterface
{
public:
	ParameterJsonInterface ();
	virtual ~ParameterJsonInterface ();

	virtual bool			ConvertParameterValueToJson (const NUIE::ParameterInterfacePtr& parameters, size_t paramIndex, rapidjson::Value& valueObj, rapidjson::Document::AllocatorType& allocator) const = 0;
	virtual NE::ValuePtr	ConvertParameterJsonToValue (const rapidjson::Value& valueObj, NUIE::ParameterInterfacePtr& parameters, size_t paramIndex) const = 0;
};

void			AddString (rapidjson::Value& obj, rapidjson::Document::AllocatorType& allocator, const rapidjson::GenericStringRef<char>& key, const std::string& value);
void			AddString (rapidjson::Value& obj, rapidjson::Document::AllocatorType& allocator, const rapidjson::GenericStringRef<char>& key, const std::wstring& value);
void			AddBoolean (rapidjson::Value& obj, rapidjson::Document::AllocatorType& allocator, const rapidjson::GenericStringRef<char>& key, bool value);
void			AddInteger (rapidjson::Value& obj, rapidjson::Document::AllocatorType& allocator, const rapidjson::GenericStringRef<char>& key, int value);
void			AddDouble (rapidjson::Value& obj, rapidjson::Document::AllocatorType& allocator, const rapidjson::GenericStringRef<char>& key, double value);

std::string		ConvertNodeTreeToJson (const AppNodeTree& appNodeTree);
std::string		ConvertMenuCommandsToJson (const NUIE::MenuCommandStructure& commands);

bool			ConvertParameterValueToJson (const NUIE::ParameterInterfacePtr& parameters, size_t paramIndex, rapidjson::Value& valueObj, rapidjson::Document::AllocatorType& allocator);
std::string		ConvertParametersToJson (const NUIE::ParameterInterfacePtr& parameters, const ParameterJsonInterface& paramJsonInterface);

NE::ValuePtr	ConvertParameterJsonToValue (const rapidjson::Value& valueObj, NUIE::ParameterInterfacePtr& parameters, size_t paramIndex);
bool			ProcessChangedParametersJson (const std::string& changeParametersJsonStr, const ParameterJsonInterface& paramJsonInterface, NUIE::ParameterInterfacePtr& parameters);

#endif
