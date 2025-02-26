#include <iostream>
#include <sstream>

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
 
  // Uncomment this block to pass the first stage
  std::cout << "$ ";

  std::string input;
  
  while (true) {
    std::getline(std::cin, input);
    if (input == "exit 0") break;

    std::istringstream iss(input);
    std::string command, statement;
    iss >> command;

    if (command == "echo") {
      std::getline(iss >> std::ws, statement);
      std::cout << statement << "\n";
    }
    else std::cout << input << ": command not found\n";
    
    std::cout << "$ ";
  }
}
