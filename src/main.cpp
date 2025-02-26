#include <iostream>

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
    std::cout << input << ": command not found\n";
    std::cout << "$ ";
  }
}
