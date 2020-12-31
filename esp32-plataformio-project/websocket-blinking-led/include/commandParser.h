#pragma once
#ifndef COMMAND_PARSER_H_
#define COMMAND_PARSER_H_



/* Standard libraries */
#include <stdio.h>
#include <string>
#include <vector>
#include <cstdint>
#include <cstdlib>
#include <map>


void handleCommand(std::string *type, std::vector<uint8_t> *params);
bool parseCommand(std::string *buffer, std::string *type, std::vector<uint8_t> *params);
bool composeCommand(std::string *buffer, std::string *type, std::vector<uint8_t> *parameters);
void setLedBundleHandler(std::vector<uint8_t> *params);
void getLedBundleHandler();



#endif 