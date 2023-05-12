#include "Utils.h"

namespace fs = std::filesystem;

std::string sha1(std::initializer_list<std::string> lst) {
    SHA1 sha;
    sha.update(lst);
    return sha.final();
}


std::string readContent(const std::string& filename) {
    try {
        std::ifstream in_file(filename, std::ios_base::binary);
        return std::string {std::istreambuf_iterator<char>(in_file), 
                                std::istreambuf_iterator<char>()};
    }
    catch (std::ifstream::failure const& ex) {
        std::cout << ex.what() << std::endl;
    }
    return std::string();
}

std::string readContent(const fs::path& filename) {
    try {
        std::ifstream in_file(filename, std::ios_base::binary);
        return std::string {std::istreambuf_iterator<char>(in_file), 
                                std::istreambuf_iterator<char>()};
    }
    catch (std::ifstream::failure const& ex) {
        std::cout << ex.what() << std::endl;
    }
    return std::string();
}


void writeContents(const std::string& file_name, std::initializer_list<std::string> lst) {
    try {
        std::ofstream out_file(file_name, std::ios_base::app);
        for (auto it = lst.begin(); it != lst.end(); ++it) {
            out_file.write((*it).c_str(), (*it).size());
        }
        out_file.close();
    }
    catch (std::ofstream::failure const& ex) {
        std::cout << ex.what() << std::endl;
    }
}

void writeContents(const fs::path& filename, std::initializer_list<std::string> lst) {
    try {
        std::ofstream out_file(filename, std::ios_base::app);
        for (auto it = lst.begin(); it != lst.end(); ++it) {
            out_file.write((*it).c_str(), (*it).size());
        }
        out_file.close();
    }
    catch (std::ofstream::failure const& ex) {
        std::cout << ex.what() << std::endl;
    }
}


std::set<std::string> plainFilenamesIn(const fs::path& dir_name) {
    try {
        std::set<std::string> myset;
        for (const auto& entry : std::filesystem::directory_iterator(dir_name)) {
            if (std::filesystem::is_regular_file(entry.path())) {
                myset.emplace(entry.path().filename().string());
            }
        }
        return myset;
    }
    catch (std::ifstream::failure const& ex) {
        std::cout << ex.what() << std::endl;
    }
    return std::set<std::string>();
}


std::fstream join(const std::string& first, std::initializer_list<std::string> lst) {
    std::string file_name = first;
    for (const auto& it : lst) {
        file_name += ( "/" + it);
    }
    return std::fstream(file_name, std::ios_base::in | std::ios_base::out);
}

void message(const std::string& msg, std::initializer_list<std::string> lst) {
    std::string new_msg = msg;
    for (const auto& it : lst) {
        new_msg += ( " " + it);
    }
    std::cout << new_msg << std::endl;
}

void create_file(const fs::path &p) { 
    std::ofstream out_file{p}; 
}

// output unordered_map as string format
std::string toString(std::unordered_map<std::string, std::string>& map) {
    std::stringstream ss;
    for (const auto& entry : map) {
		ss << "{" << entry.first << ", " << entry.second << "} ";
	}
    return ss.str();
}
