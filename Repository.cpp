#include <iostream>
#include <list>
#include <vector>
#include <algorithm>

#include "Repository.h"

/* static variable initialization */
fs::path Repository::CWD = std::filesystem::current_path();
fs::path Repository::GITLET_DIR = CWD / ".gitlet";
fs::path Repository::HEAD = GITLET_DIR / "HEAD"; 
fs::path Repository::INDEX = GITLET_DIR / "INDEX";
fs::path Repository::COMMIT_DIR = GITLET_DIR / "commits";
fs::path Repository::BLOB_DIR = GITLET_DIR / "blobs";
fs::path Repository::TREE = GITLET_DIR / "TREE";
fs::path Repository::CONFIG = GITLET_DIR / "CONFIG";

Tree Repository::tree = Tree();
StagingArea Repository::staging_area = StagingArea();
RemoteRepo Repository::repo = RemoteRepo();

// initialize a local repo
void Repository::init() {
    fs::create_directory(GITLET_DIR);
    create_file(HEAD);
    create_file(INDEX);
    fs::create_directory(COMMIT_DIR);
    fs::create_directory(BLOB_DIR);
    create_file(TREE);
    create_file(CONFIG);

    // repo = RemoteRepo();
    // repo.writeToConfigFile();


    Commit initial_commit("initial commit", 0);
    initial_commit.writeToFile();

    tree.add(initial_commit);
    tree.writeToFile();

    staging_area = StagingArea();
    staging_area.writeToFile();
}

void Repository::add(const std::string& filename) {
    fs::path file(filename);
    if (fs::exists(file) == false) {
        message("File does not exist.");
        return;
    }
    tree.readFromFile();
    staging_area.readFromFile();

    Blob blob = Blob();
    blob.readFromOriginalFile(file);
    staging_area.addToAddition(blob);
    staging_area.removeFromRemoval(filename);
    Commit head = tree.getHead();
    if (head.containFile(filename, blob.getFileRef())) {
        staging_area.removeFromAddition(blob);
    }
    staging_area.writeToFile();
    blob.writeToBlobFile();
}

void Repository::commit(const std::string& msg) {
    if (msg == "") {
        message("Please enter a commit message.");
        return;
    }
    staging_area.readFromFile();
    if (staging_area.isEmpty()) {
        message("No changes added to the commit.");
        return;
    }
    tree.readFromFile();
    Commit commit = tree.getHead();
    std::unordered_map<std::string, std::string> addition 
        = staging_area.getAddition();
    for (auto& it : addition) {
        commit.addFileReference(it.first, it.second);
    }

    std::unordered_map<std::string, std::string> removal
        = staging_area.getRemoval();
    for (auto& it : removal) {
        commit.removeFileReference(it.first, it.second);
    }

    staging_area = StagingArea();
    commit.updateDate();
    commit.updateMessage(msg);
    commit.updateRef();
    tree.add(commit);

    // persistence
    commit.writeToFile();
    tree.writeToFile();
    staging_area.writeToFile();
}

void Repository::checkout(const std::string& double_dash,const std::string& filename) {
    tree.readFromFile();
    Commit head = tree.getHead();
    Blob blob = Blob();
    std::string blob_ref = head.getBlobRef(filename);
    if (blob_ref == "") {
        message("File does not exist in that commit.");
        return;
    }
    blob.setFileRef(blob_ref);
    blob.readFromBlobFile();
    blob.writeToOriginalFile();

}

void Repository::checkout(const std::string& commit_ref, const std::string& double_dash, const std::string& filename) {
    Commit commit = Commit();
    commit.setCommitRef(commit_ref);
    if (commit.readFromFile() == false) {
        message("No commit with that id exists.");
        return;
    }

    std::string blob_ref = commit.getBlobRef(filename);
    if (blob_ref == "") {
        message("File does not exist in that commit.");
        return;
    }
    Blob blob = Blob();
    blob.setFileRef(blob_ref);
    blob.readFromBlobFile();
    blob.writeToOriginalFile();
}

void Repository::checkout(const std::string& branch_name) {
    tree.readFromFile();
    if (tree.isBranchExist(branch_name) == false) {
        message("No such branch exists.");
        return;
    }

    if (branch_name == tree.getActiveBranch()) {
        message("No need to checkout the current branch.");
        return;
    }

    Commit head = tree.getHead();
    tree.setActiveBranch(branch_name);
    Commit checked_head = tree.getHead();

    staging_area.readFromFile();
    std::set<std::string> files = plainFilenamesIn(CWD);
    for (const auto& file : files) {
        if (checked_head.containFile(file)) {
            Blob blob = Blob();
            blob.readFromOriginalFile(file);
            if (staging_area.containsInAddition(file) == false && head.containFile(file) == false) {
                message("There is an untracked file in the way; delete it, or add and commit it first.");
                return;
            }
        }
    }

    for (const auto& file : files) {
        /** All files in the commit at the head of the given branch will be put into CWD */
        if (head.containFile(file) && checked_head.containFile(file)) {
            Blob blob = Blob();
            blob.setFileRef(checked_head.getBlobRef(file));
            blob.readFromBlobFile();
            blob.writeToOriginalFile();
        }
        /** Any files that are tracked in the current branch but are not present
         * in the checked-out branch are deleted. */
        else if (head.containFile(file) && !checked_head.containFile(file)) {
            fs::remove(file);
        }
    }

    /** recover files tracked in check head but not exist in CWD */
    files = plainFilenamesIn(CWD);
    std::unordered_map<std::string, std::string> file_map = checked_head.getFilesMap();
    for (auto it : file_map) {
        if (files.find(it.first) == files.end()) {
            Blob blob = Blob();
            blob.setFileRef(it.second);
            blob.readFromBlobFile();
            blob.setFilename(it.first);
            blob.writeToOriginalFile();
        }
    }

    // persistence
    staging_area = StagingArea();
    staging_area.writeToFile();
    tree.writeToFile();
}

void Repository::rm(const std::string& file_name) {
    tree.readFromFile();
    staging_area.readFromFile();

    Commit head = tree.getHead();
    if (staging_area.containsInAddition(file_name) == false && head.containFile(file_name) == false) {
        message("No reason to remove the file.");
        return;
    }

    if (staging_area.containsInAddition(file_name)) {
        staging_area.removeFromAddition(file_name);
    }

    if (head.containFile(file_name)) {
        Blob blob = Blob();
        blob.setFileRef(head.getBlobRef(file_name));
        blob.readFromBlobFile();
        staging_area.removeFromAddition(file_name);
        staging_area.addToRemoval(blob);
        fs::path pa(file_name);
        fs::remove(pa);
    }

    staging_area.writeToFile();
}

void Repository::log() {
    tree.readFromFile();
    Commit head = tree.getHead();
    bool to_end = false;
    while (to_end == false) {
        message("===");
        message("commit " + head.getCommitRef());
        if (head.isMergedCommit()) {
            message("Merge: " 
                    + head.getParentRef().substr(0, 7)
                    + " " 
                    + head.getSecondParentRef().substr(0, 7));
        }
        message("Date: " + head.getDate());
        message(head.getMessage());
        message("");

        std::string parent_ID = head.getParentRef();
        if (parent_ID == "") {
            to_end = true;
        }
        else {
            head.setCommitRef(parent_ID);
            head.readFromFile();
        }
    }

}

void Repository::globalLog() {
    std::set<std::string> commit_dirs = plainFilenamesIn(COMMIT_DIR);
    for (const auto& commit_dir : commit_dirs) {
        std::set<std::string> commit_files = plainFilenamesIn(commit_dir);
        for (const auto& commit_file : commit_files) {
            Commit commit = Commit();
            commit.setCommitRef(commit_dir + commit_file);
            commit.readFromFile();
            message("===");
            message("commit " + commit.getCommitRef());
            if (commit.isMergedCommit()) {
                message("Merge: " 
                        + commit.getParentRef().substr(0, 7)
                        + " " 
                        + commit.getSecondParentRef().substr(0, 7));
            }
            message("Date: " + commit.getDate());
            message(commit.getMessage());
            message("");
        }
    }
}

void Repository::find(const std::string& commit_message) {
    bool find_commit = false;
    std::set<std::string> commit_dirs = plainFilenamesIn(COMMIT_DIR);
    for (const auto& commit_dir : commit_dirs) {
        std::set<std::string> commit_files = plainFilenamesIn(commit_dir);
        for (const auto& commit_file : commit_files) {
            Commit commit = Commit();
            commit.setCommitRef(commit_dir + commit_file);
            commit.readFromFile();
            if (commit_message == commit.getMessage()) {
                find_commit = true;
                message(commit.getCommitRef());
            }
        }
    }
    if (find_commit == false) {
        message("Found no commit with that message.");
    }
}

void Repository::status() {
    tree.readFromFile();
    staging_area.readFromFile();

    /* Entries should be listed in lexicographic order, * using the Java string-comparison order (the asterisk doesn’t count). */
    message("=== Branches ===");
    std::unordered_map<std::string, std::string> branches = tree.getBranches();
    std::set<std::string> branches_set;
    for (const auto& branch : branches) {
        branches_set.emplace(branch.first);
    }
    
    for (const auto& branch_name : branches_set) {
        if (branch_name == tree.getActiveBranch()) {
            message("*" + branch_name);
        }
        else {
            message(branch_name);
        }
    }
    message("");


    message("=== Staged Files ===");
    std::unordered_map<std::string, std::string> staged_files = staging_area.getAddition();
    std::set<std::string> staged_set;
    for (const auto& file : staged_files) {
        staged_set.emplace(file.first);
    }
    for (const auto& file_name : staged_set) {
        message(file_name);
    }
    message("");


    message("=== Removed Files ===");
    std::unordered_map<std::string, std::string> removed_files = staging_area.getRemoval();
    std::set<std::string> removed_set;
    for (const auto& file : removed_files) {
        removed_set.emplace(file.first);
    }
    for (const auto& file_name : removed_set) {
        message(file_name);
    }
    message("");


    /* A file is "modified but not staged" if it is:
        Tracked in the current commit, changed in the working directory, but not staged; or
        Staged for addition, but with different contents than in the working directory; or
        Staged for addition, but deleted in the working directory; or
        Not staged for removal, but tracked in the current commit and deleted from the working directory.
    */
    message("=== Modifications Not Staged For Commit ===");
    std::set<std::string> files_modified_but_not_staged;
    Commit head = tree.getHead();
    std::set<std::string> files_in_cwd = plainFilenamesIn(CWD);
    for (const auto& file_name : files_in_cwd) {
        Blob blob = Blob();
        blob.readFromOriginalFile(file_name);
        // Tracked in the current commit, changed in the working directory, but not staged; 
        // or
        // Staged for addition, but with different contents than in the working directory;
        if ((head.containFile(file_name) && head.getBlobRef(file_name) != blob.getFileRef())
            || ((staging_area.containsInAddition(file_name) && staging_area.getFileRefInAddition(file_name) != blob.getFileRef()))) {
            files_modified_but_not_staged.emplace(file_name + "(modified)");
        }
    }

    // Staged for addition, but deleted in the working directory;
    for (const auto& file_name : staged_set) {
        if (files_in_cwd.find(file_name) == files_in_cwd.end()) {
            files_modified_but_not_staged.emplace(file_name + "(deleted)");
        }
    }

    // Not staged for removal, but tracked in the current commit and deleted from the working directory.
    std::set<std::string> files_in_head;
    for (const auto& file : head.getFilesMap()) {
        files_in_head.emplace(file.first);
    }
    for (const auto& file_name : files_in_head) {
        if (files_in_cwd.find(file_name) == files_in_cwd.end() 
        && staging_area.containsInRemoval(file_name) == false) {
            files_modified_but_not_staged.emplace(file_name + "(deleted)");
        }
    }

    // stdcout
    for (const auto& file_name : files_modified_but_not_staged) {
        message(file_name);
    }
    message("");


    // files present in the working directory but neither staged for addition nor tracked;
    // or
    // files that have been staged for removal, but then re-created without Gitlet’s knowledge.
    message("=== Untracked Files ===");
    // todo
    for (const auto& file_name : files_in_cwd) {
        if (staging_area.containsInAddition(file_name) == false 
            && staging_area.containsInRemoval(file_name) == false 
            && head.containFile(file_name) == false) {
            message(file_name);
        }
    }
    message("");
}

void Repository::branch(const std::string& branch_name) {
    // todo
}

void Repository::rmBranch(const std::string& branch_name) {
    // todo
}

void Repository::reset(const std::string& commit_ref) {
    // todo
}

void Repository::merge(const std::string& branch_name) {
    // todo
}

void Repository::addRemote(const std::string& remote_name, const std::string& remote_repo) {
    // todo
}

void Repository::rmRemote(const std::string& remote_name) {
    // todo
}

void Repository::push(const std::string& remote_name, const std::string& branch_name) {
    // todo
}

void Repository::fetch(const std::string& remote_name, const std::string& branch_name) {
    // todo
}

void Repository::pull(const std::string& remote_name, const std::string& branch_name) {
    // todo
}

