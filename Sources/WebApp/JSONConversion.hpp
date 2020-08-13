#ifndef JSON_CONVERSION_HPP
#define JSON_CONVERSION_HPP

#include "NUIE_MenuCommands.hpp"

extern const int InvalidCommandId;
extern const int FirstCommandId;

std::string ConvertMenuCommandsToJson (const NUIE::MenuCommandStructure& commands);

#endif
