#include <iostream>
#include <string>
#include "../Utils.h"

int main() {
    std::string str = "hello, world";
    std::initializer_list<std::string> lst = {"!"};
    message(str);
    std::cout << sha1(lst) << std::endl;
    return 0;
}