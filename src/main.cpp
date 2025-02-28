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

const std::unordered_set<std::string> builtin_commands = {"echo", "type", "exit", "pwd"};

std::string find_executable(const std::string &command) {
    std::string path = std::getenv("PATH");
    std::istringstream path_stream(path);
    std::string directory;
    
    while (std::getline(path_stream, directory, ':')) {
        for (const auto &file : fs::directory_iterator(directory)) {
            if (file.path().filename() == command) {
                return file.path();
            }
        }
    }
    return "";
}

void run_executables(const std::vector<std::string> &args) {
    if (args.empty()) return;

    char *cmd = const_cast<char*>(args[0].c_str());
    std::vector<char*> argv;
    for (const auto &arg : args) argv.push_back(const_cast<char*>(arg.c_str()));
    argv.push_back(nullptr);

    if (fork() == 0) { 
        execvp(cmd, argv.data());
        std::cerr << cmd << ": command not found\n";
        exit(1);
    } 
    wait(nullptr);
}

void handle_builtin_commands(const std::vector<std::string> &args) {
    if (args[0] == "pwd") {
      std::cout << fs::current_path() << "\n";
    }
    else if (args[0] == "echo") {
        for (size_t i = 1; i < args.size(); ++i) {
            if (i > 1) std::cout << " ";
            std::cout << args[i];
        }
        std::cout << "\n";
    } else if (args[0] == "type") {
        if (args.size() < 2) {
            std::cerr << "type: missing operand\n";
            return;
        }
        if (builtin_commands.find(args[1]) != builtin_commands.end()) {
            std::cout << args[1] << " is a shell builtin\n";
        } else {
            std::string exe_path = find_executable(args[1]); 
            if(!exe_path.empty()) std::cout << args[1] << " is " << exe_path << "\n";
            else std::cout << args[1] << ": not found\n";
        }
    }
}

void process_input(const std::string &input) {
    std::istringstream iss(input);
    std::vector<std::string> args;
    std::string command;

    while (iss >> command) {
        args.push_back(command);
    }
    
    if (args.empty()) return;
    if (args[0] == "exit" && args.size() > 1 && args[1] == "0") exit(0);
    
    if (builtin_commands.find(args[0]) != builtin_commands.end()) {
        handle_builtin_commands(args);
    } else {
        run_executables(args);
    }
}

int main() {
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;
    std::string input;
    
    while (true) {
        std::cout << "$ ";
        std::getline(std::cin, input);
        process_input(input);
    }
    return 0;
}
