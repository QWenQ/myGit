#include <iostream>
#include <string>
#include <fstream>
#include "../Utils.h"

int main(int argc, char* argv[]) {
    const std::string dir = "dir1";
    std::fstream file = join(dir, {"a.txt"});
    std::string msg;
    std::getline(file, msg);
    message(msg, {"hello", "world"});
    return 0;
}