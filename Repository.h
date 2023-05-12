#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <stdio.h>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include "Tree.h"
#include "StagingArea.h"
#include "RemoteRepo.h"
#include "Commit.h"

namespace fs = std::filesystem;

class Repository {
    public:
        static fs::path CWD; 
        static fs::path GITLET_DIR; 
        static fs::path HEAD; 
        static fs::path INDEX; 
        static fs::path COMMIT_DIR;
        static fs::path BLOB_DIR;
        static fs::path TREE;
        static fs::path CONFIG;

        // initialize a local repo
        static void init();
        static void add(const std::string& filename);
        static void commit(const std::string& message);
        static void checkout(const std::string& double_dash,const std::string& filename);
        static void checkout(const std::string& commit_ref, const std::string& double_dash, const std::string& filename);
        static void checkout(const std::string& branch_name);
        static void rm(const std::string& filename);
        static void log();
        static void globalLog();
        static void find(const std::string& commit_message);
        static void status();
        static void branch(const std::string& branch_name);
        static void rmBranch(const std::string& branch_name);
        static void reset(const std::string& commit_ref);
        static void merge(const std::string& branch_name);
        static void addRemote(const std::string& remote_name, const std::string& remote_repo);
        static void rmRemote(const std::string& remote_name);
        static void push(const std::string& remote_name, const std::string& branch_name);
        static void fetch(const std::string& remote_name, const std::string& branch_name);
        static void pull(const std::string& remote_name, const std::string& branch_name);

    private:
        static Tree tree;
        static StagingArea staging_area;
        static RemoteRepo repo; 
}; // class Repository



#endif // REPOSITORY_H