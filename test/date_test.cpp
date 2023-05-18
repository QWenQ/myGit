#include <iostream>
#include <string>
#include "../Date.h"

int main(int argc, char* arg[]) {
	Date date = Date(0);
	std::string str = date.toString();
	std::cout << str << std::endl;
	return 0;
}
