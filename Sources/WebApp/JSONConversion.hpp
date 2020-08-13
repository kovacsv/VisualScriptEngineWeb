#ifndef JSON_CONVERSION_HPP
#define JSON_CONVERSION_HPP

#include "NUIE_MenuCommands.hpp"
#include "NUIE_ParameterInterface.hpp"

extern const int InvalidCommandId;
extern const int FirstCommandId;

std::string ConvertMenuCommandsToJson (const NUIE::MenuCommandStructure& commands);
std::string ConvertParametersToJson (const NUIE::ParameterInterfacePtr& parameters);

#endif
