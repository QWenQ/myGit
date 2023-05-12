#include <iostream>
#include <list>
#include "../Utils.h"

int main(int argc, char* argv[]) {
    const std::string path = std::filesystem::current_path().c_str();
    std::list<std::string> ls = plainFilenamesIn(path);
    for (const auto& it : ls) {
        std::cout << it << std::endl;
    }

    return 0;
}