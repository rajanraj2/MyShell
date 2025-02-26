#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

void executable_files(std::stringstream path, std::string command) {
  std::string directory;
  while (std::getline(path, directory, ':')) {
    // search at this directory for the required command, if it is found then print the path and return
    for (auto file : fs::directory_iterator(directory)) {
      if (file.path().filename() == command) {
        std::string value = file.path();
        std::cout << command << " is " << value  << "\n";
        return;
      }
    }
  }
  std::cout << command << ": not found\n";
}

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
 
  std::cout << "$ ";

  std::string input;
  std::unordered_set<std::string> builtin_commands = {"echo", "type", "exit"};

  std::string path = std::getenv("PATH");
  
  while (true) {
    std::getline(std::cin, input);
    if (input == "exit 0") break;

    std::istringstream iss(input);
    std::string command, statement;
    iss >> command;
    std::getline(iss >> std::ws, statement);


    if (command == "echo") std::cout << statement << "\n";
    else if (command == "type") {
      if (builtin_commands.find(statement) != builtin_commands.end()) std::cout << statement << " is a shell builtin\n";
      else executable_files(std::stringstream(path), statement);
    }
    else std::cout << input << ": command not found\n";
    
    std::cout << "$ ";
  }
}
