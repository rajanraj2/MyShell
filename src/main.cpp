#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
 
  // Uncomment this block to pass the first stage
  std::cout << "$ ";

  std::string input;
  std::unordered_set<std::string> builtin_commands = {"echo", "type", "exit"};
  
  while (true) {
    std::getline(std::cin, input);
    if (input == "exit 0") break;

    std::istringstream iss(input);
    std::string command, statement;
    iss >> command;
    std::getline(iss >> std::ws, statement);

    if (command == "echo") {
      std::cout << statement << "\n";
    }
    else if (command == "type") {
      if (builtin_commands.find(statement) != builtin_commands.end()) {
        std::cout << statement << " is a shell builtin\n";
      }
      else {
        std::cout << statement << ": not found\n";
      }
    }
    else std::cout << input << ": command not found\n";
    
    std::cout << "$ ";
  }
}
