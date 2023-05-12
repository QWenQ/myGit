#ifndef UTILS_H
#define UTILS_H

#include <string.h>
#include <string>
#include <fstream>
#include <filesystem>
#include <list>
#include <set>
#include <sstream>
#include <initializer_list>

#include "sha1.h"

// include headers that implement a archive in simple text format
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/string.hpp>



namespace fs = std::filesystem;

std::string sha1(std::initializer_list<std::string> lst);

std::string readContent(const std::string& filename);

std::string readContent(const fs::path& filename);


void writeContents(const std::string& file_name, std::initializer_list<std::string> lst);

void writeContents(const fs::path& filename, std::initializer_list<std::string> lst);

void message(const std::string& msg, std::initializer_list<std::string> lst = {});

void create_file(const fs::path &p);

// read/write an object is something wrong
template <typename T>
void writeObject(const std::string& file_name, const T& object) {
    try {
        std::ofstream ofs(file_name);
        boost::archive::text_oarchive oa(ofs);
        oa << object;
    }
    catch (std::ofstream::failure const& ex) {
        std::cout << ex.what() << std::endl;
    }
}

template <typename T>
void writeObject(const std::filesystem::path& p, const T& object) {
	try {
		std::ofstream ofs(p);
		boost::archive::text_oarchive oa(ofs);
		oa << object;
	}
	catch (std::ofstream::failure const& ex) {
		std::cout << ex.what() << std::endl;
	}
}

template <typename T>
void readObject(const std::string& file_name, T& object) {
    try {
        std::ifstream ifs(file_name);
        boost::archive::text_iarchive ia(ifs);
        ia >> object;
    }
    catch (std::ifstream::failure const& ex) {
        std::cout << ex.what() << std::endl;
    }
}

template <typename T>
void readObject(const std::filesystem::path& p, T& object) {
    try{
        std::ifstream ifs(p);
        boost::archive::text_iarchive ia(ifs);
        ia >> object;
    }
    catch (std::ifstream::failure const& ex) {
        std::cout << ex.what() << std::endl;
    }
}


std::set<std::string> plainFilenamesIn(const fs::path& dir_name);


std::fstream join(const std::string& first, std::initializer_list<std::string> lst);

// output unordered_map as string format
std::string toString(std::unordered_map<std::string, std::string>& map);

#endif // UTILS_H
