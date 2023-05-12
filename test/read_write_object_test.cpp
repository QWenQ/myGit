#include <iostream>
#include "../Utils.h"

int main(int argc, char* argv[]) {
    std::map<int, int> m1;
    m1.emplace(1, 2);
    const std::string file_name("filename");
    writeObject(file_name, m1);
    std::map<int, int> m2;
    readObject(file_name, m2);

    // filesystem
    std::filesystem::path p{file_name};
    std::map<int, int> m3;
    readObject(p, m3);
    return 0;
}