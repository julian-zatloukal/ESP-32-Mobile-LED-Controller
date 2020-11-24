#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <map>

void handleCommand(std::string *type, std::vector<uint8_t> *params);
bool parseCommand(std::string *buffer, std::string *type, std::vector<uint8_t> *params);
void setLedBundleHandler (std::vector<uint8_t> *params);
void getLedBundleHandler ();

std::vector<std::string> commands = {"SET_LED_BUNDLE", "GET_LED_BUNDLE"};
uint8_t ledArrayState[8] = {0};

class Commands {

  enum commandTypes {
    setLEDBundle,
    getLEDBundle,
  };

  std::map <commandTypes, std::string> list;

  // std::vector<std::string> list = {"SET_LED_BUNDLE", "GET_LED_BUNDLE"};
  
};

int main()
{

  std::string buffer_str("SET_LED_BUNDLE[1][1][1][1][1][0][1][0]");


  std::vector<uint8_t> parameters;
  std::string command_type("");

  bool parsingResult = parseCommand(&buffer_str, &command_type, &parameters);
  if (parsingResult) handleCommand(&command_type, &parameters);


  std::string buffer_str2("GET_LED_BUNDLE");
  bool parsingResult2 = parseCommand(&buffer_str2, &command_type, &parameters);
  if (parsingResult2) handleCommand(&command_type, &parameters);
  

  return 0;
}

bool parseCommand(std::string *buffer, std::string *type, std::vector<uint8_t> *params)
{
  (*type) = "";
  for (std::string command : commands) {
    if ((*buffer).find(command)!=-1) {
      (*type) = command;
    }
  }
  if ((*type)=="") return false;


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
  // std::cout << "Handling command " << (*type) << std::endl;
  // std::cout << "Printing params\n";
  // for (uint8_t param : *params)
  // {
  //   std::string s = std::to_string(param);
  //   std::cout << s << std::endl;
  // }

  if ((*type) == "SET_LED_BUNDLE") {
    setLedBundleHandler(params);
  } else if ((*type) == "GET_LED_BUNDLE") { 
    getLedBundleHandler();
  }

}

void setLedBundleHandler (std::vector<uint8_t> *params) {
  for (int i = 0; i < 8; i++) {
    if ((*params)[i]==0) {
      std::cout << "turn off led number " << std::to_string(i) << std::endl;
      ledArrayState[i] = 0;
    } else {
      std::cout << "turn on led number " << std::to_string(i) << std::endl;
      ledArrayState[i] = 1;
    }
  }
}

void getLedBundleHandler () {
  std::string buffer;
  buffer+="SET_LED_BUNDLE";
  for (int i = 0; i < 8; i++) {
    buffer+="[";
    if (ledArrayState[i]) {
      buffer+="1";
    } else {
      buffer+="0";
    }
    buffer+="]";
  }

  std::cout << "Sending command: " << buffer << std::endl;

  /* send through WebSocket */
}