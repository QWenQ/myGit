#include <iostream>
#include <string>
#include "../Date.h"

int main(int argc, char* arg[]) {
	Date date;
	std::string str = date.toString();
	std::cout << str << std::endl;
	return 0;
}
