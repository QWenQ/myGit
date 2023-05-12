#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>

#include "Utils.h"
#include "Repository.h"

void checkAndRun(int argc, char* argv[]) {
    if (argc == 1) {
        message("Please enter a command!");
        std::exit(0);
    }

    if (std::strcmp(argv[1], "init") == 0) {
        // Debug
        if (argc != 2) {
            message("Incorrect operands!");
            std::exit(0);
        }
        Repository::init();
    }
    else if (std::strcmp(argv[1], "add") == 0) {
        if (argc != 3) {
            message("Incorrect operands.");
            std::exit(0);
        }
        Repository::add(argv[2]);
    }
    else if (std::strcmp(argv[1], "commit") == 0) {
        if (argc != 3) {
            message("Incorrect operands.");
            std::exit(0);
        }
        Repository::commit(argv[2]);
    }
    else if (std::strcmp(argv[1], "checkout") == 0) {
        /** checkout [branch name] */
        if (argc == 3) Repository::checkout(argv[2]);
        /** checkout -- [file name] */
        else if (argc == 4 && std::strcmp(argv[2], "--") == 0) Repository::checkout(argv[2], argv[3]);
        /** checkout [commit id] -- [file name] */
        else if (argc == 5 && std::strcmp(argv[3], "--") == 0) Repository::checkout(argv[2], argv[3], argv[4]);
        else message("Incorrect operands.");
    }
    else if (std::strcmp(argv[1], "rm") == 0) {
        if (argc != 2) {
            message("Incorrect operands.");
            std::exit(0);
        }
        Repository::rm(argv[1]);
    }
    else if (std::strcmp(argv[1], "log") == 0) {
        if (argc != 2) {
            message("Incorrect operands.");
            std::exit(0);
        }
        Repository::log();
    }
    else if (std::strcmp(argv[1], "global-log") == 0) {
        if (argc != 2) {
            message("Incorrect operands.");
            std::exit(0);
        }
        Repository::globalLog();
    }
    else if (std::strcmp(argv[1], "find") == 0) {
        if (argc != 3) {
            message("Incorrect operands.");
            std::exit(0);
        }
        Repository::find(argv[2]);
    }
    else if (std::strcmp(argv[1], "status") == 0) {
        if (argc != 2) {
            message("Incorrect operands.");
            std::exit(0);
        }
        Repository::status();

    }
    else if (std::strcmp(argv[1], "branch") == 0) {
        if (argc != 3) {
            message("Incorrect operands.");
            std::exit(0);
        }
        Repository::branch(argv[2]);

    }
    else if (std::strcmp(argv[1], "rm-branch") == 0) {
        if (argc != 3) {
            message("Incorrect operands.");
            std::exit(0);
        }
        Repository::rmBranch(argv[2]);
    }
    else if (std::strcmp(argv[1], "reset") == 0) {
        if (argc != 3) {
            message("Incorrect operands.");
            std::exit(0);
        }
        Repository::reset(argv[2]);
    }
    else if (std::strcmp(argv[1], "merge") == 0) {
        if (argc != 3) {
            message("Incorrect operands.");
            std::exit(0);
        }
        Repository::merge(argv[2]);

    }
    else if (std::strcmp(argv[1], "add-remote") == 0) {
        if (argc != 4) {
            message("Incorrect operands.");
            std::exit(0);
        }
        Repository::addRemote(argv[2], argv[3]);

    }
    else if (std::strcmp(argv[1], "rm-remote") == 0) {
        if (argc != 3) {
            message("Incorrect operands.");
            std::exit(0);
        }
        Repository::rmRemote(argv[2]);

    }
    else if (std::strcmp(argv[1], "push") == 0) {
        if (argc != 4) {
            message("Incorrect operands.");
            std::exit(0);
        }
        Repository::push(argv[2], argv[3]);

    }
    else if (std::strcmp(argv[1], "fetch") == 0) {
        if (argc != 4) {
            message("Incorrect operands.");
            std::exit(0);
        }
        Repository::fetch(argv[2], argv[3]);

    }
    else if (std::strcmp(argv[1], "pull") == 0) {
        if (argc != 4) {
            message("Incorrect operands.");
            std::exit(0);
        }
        Repository::pull(argv[2], argv[3]);
    }
    else {
        message("No command with that name exists.");
    }
    

}

int main(int argc, char* argv[]) {
    checkAndRun(argc, argv);
    return 0;
}