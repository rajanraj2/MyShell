#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>
#include <filesystem>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

namespace fs = std::filesystem;

std::string find_executable(std::stringstream path, const std::string &command) {
    std::string directory;
    while (std::getline(path, directory, ':')) {
        for (auto &file : fs::directory_iterator(directory)) {
            if (file.path().filename() == command) {
                return file.path();
            }
        }
    }
    return "";
}

void run_executables(std::vector<std::string> args) {
  if (args.empty()) return;

  char* cmd = args[0].data();
  std::vector<char*> argv;
  for (auto& arg : args) argv.push_back(arg.data());
  argv.push_back(nullptr); 

  if (fork() == 0) { 
      execvp(cmd, argv.data());
      std::cerr << cmd << ": command not found\n";
      exit(1);
  } 
  wait(nullptr);
}


int main() {
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
        std::string command;
        std::vector<std::string> args;

        while (iss >> command) {
            args.push_back(command);
        }

        if (args.empty()) {
            std::cout << "$ ";
            continue;
        }

        if (args[0] == "echo") {
            for (size_t i = 1; i < args.size(); ++i) {
                if (i > 1) std::cout << " ";
                std::cout << args[i];
            }
            std::cout << "\n";
        } else if (args[0] == "type") {
            if (builtin_commands.find(args[1]) != builtin_commands.end())
                std::cout << args[1] << " is a shell builtin\n";
            else {
                std::string exe_path = find_executable(std::stringstream(path), args[1]);
                if (!exe_path.empty()) std::cout << args[1] << " is " << exe_path << "\n";
                else std::cout << args[1] << ": not found\n";
            }
        } else {
            run_executables(args);
        }

        std::cout << "$ ";
    }

    return 0;
}
