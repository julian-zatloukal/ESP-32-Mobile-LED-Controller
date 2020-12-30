#include "commandParser.h"
#include "ledModule.h"


std::vector<std::string> commands = {"SET_LED_BUNDLE", "GET_LED_BUNDLE"};
uint8_t ledArrayState[8] = {0};

bool parseCommand(std::string *buffer, std::string *type, std::vector<uint8_t> *params)
{
  (*type) = "";
  for (std::string command : commands)
  {
    if ((*buffer).find(command) != -1)
    {
      (*type) = command;
    }
  }
  if ((*type) == "")
    return false;

  bool readingParam = false;
  std::string currentParam;

  for (int i = 0; i < (*buffer).length(); i++)
  {
    if ((*buffer)[i] == ']')
    {
      readingParam = false;
      (*params).push_back(atoi(currentParam.c_str()));
    }
    if (readingParam)
      currentParam.push_back((*buffer)[i]);

    if ((*buffer)[i] == '[')
    {
      readingParam = true;
      currentParam = "";
    }
  }

  return true;
}



void handleCommand(std::string *type, std::vector<uint8_t> *params)
{
  if ((*type) == "SET_LED_BUNDLE")
  {
    setLedBundleHandler(params);
  }
  else if ((*type) == "GET_LED_BUNDLE")
  {
    getLedBundleHandler();
  }
}

void setLedBundleHandler(std::vector<uint8_t> *params)
{
  for (int i = 0; i < 8; i++)
  {
    if ((*params)[i] == 0)
    {
      ledArrayState[i] = 0;
      ledModule::setLED(i, 0); 
    }
    else
    {
      ledArrayState[i] = 1;
      ledModule::setLED(i, 1); 
    }
  }
}

void getLedBundleHandler()
{
  std::string buffer;
  buffer += "SET_LED_BUNDLE";
  for (int i = 0; i < 8; i++)
  {
    buffer += "[";
    if (ledArrayState[i])
    {
      buffer += "1";
    }
    else
    {
      buffer += "0";
    }
    buffer += "]";
  }

  /* send buffer through WebSocket */
}