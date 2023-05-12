#include "Commit.h"
#include "Repository.h"
#include "Utils.h"

/* Constructors */

Commit::Commit() 
        :   m_timestamp(),
            m_message(),
            m_parent_ref(),
            m_second_parent_ref(),
            m_commit_ref(),
            m_map()
        { }

Commit::Commit(const std::string& message, std::time_t time)
        :   m_timestamp(time),
            m_message(message),
            m_parent_ref(),
            m_second_parent_ref(),
            m_commit_ref(),
            m_map()     
        {
            m_commit_ref = sha1({m_message, 
                                m_timestamp.toString(), 
                                toString(m_map)});
        }


Commit Commit::getRoot() const {
    Commit root = *this;
    while (root.m_parent_ref != "") {
        root = getFirstParent();
    }
    return root;
}

Commit Commit::getFirstParent() const {
    if (m_parent_ref == "") return Commit();
    Commit first_parent;
    first_parent.setCommitRef(m_parent_ref);
    first_parent.readFromFile();
    return first_parent;
}

Commit Commit::getSecondParent() const {
    if (m_second_parent_ref == "") return Commit();
    Commit second_parent;
    second_parent.setCommitRef(m_second_parent_ref);
    second_parent.readFromFile();
    return second_parent;
}

bool Commit::equalTo(const Commit& commit) const {
    return m_commit_ref == commit.m_commit_ref;
}

void Commit::setLogMessage(const std::string& log_message) {
    m_message = log_message;
}

void Commit::setSecondParentRef(const std::string& second_parent_ref) {
    m_second_parent_ref = second_parent_ref;
}

void Commit::setBlobRef(const std::string& filename, const std::string& commit_ref) {
    m_map.emplace(filename, commit_ref);
}

const std::unordered_map<std::string, std::string>& Commit::getFilesMap() const {
    return m_map;
}

bool Commit::isMergedCommit() const {
    return m_second_parent_ref != "";
}

void Commit::updateRef() {
    m_commit_ref = sha1({ m_message, m_parent_ref, m_second_parent_ref, toString(m_map) });
}

const std::string& Commit::getMessage() const {
    return m_message;
}

std::string Commit::getDate() const {
    return m_timestamp.toString();
}

bool Commit::containFile(const std::string& filename) const {
    return m_map.find(filename) != m_map.end();
}

bool Commit::containFile(const std::string& filename, const std::string& blob_ref) const {
    return m_map.find(filename) != m_map.end() && m_map.at(filename) == blob_ref;
}

void Commit::setCommitRef(const std::string& commit_ref) {
    m_commit_ref = commit_ref;
}

std::string Commit::getBlobRef(const std::string& filename) const {
    if (m_map.find(filename) == m_map.end()) return "";
    return m_map.at(filename);
}

void Commit::updateDate() {
    m_timestamp = Date();
}

void Commit::updateMessage(const std::string& message) {
    m_message = message;
}

void Commit::addFileReference(const std::string& filename, const std::string& commit_ref) {
    m_map.emplace(filename, commit_ref);
}

void Commit::removeFileReference(const std::string& filename, const std::string& commit_ref) {
    if (m_map.find(filename) != m_map.end() && m_map[filename] == commit_ref) {
        m_map.erase(filename);
    }
}

const std::string& Commit::getSecondParentRef() const {
    return m_second_parent_ref;
}

const std::string& Commit::getParentRef() const {
    return m_parent_ref;
}

void Commit::setParentRef(const std::string& parent_ref) {
    m_parent_ref = parent_ref;
}

const std::string& Commit::getCommitRef() const {
    return m_commit_ref;
}

bool Commit::containsBlob(const Blob& blob) const {
    const std::string& filename = blob.getFilename();
    const std::string& blob_ref = blob.getFileRef();
    return m_map.find(filename) != m_map.end() && m_map.at(filename) == blob_ref;
}



/*
- CWD
    - .gitlet/            -- local repo
        - commits/        -- directory of all commit files
            - [subdir]/     -- directory of commit files, named with first 2 characters of commit reference
                [commit1]   -- commit file, named with last 38 characters of commit reference
                ....
                [commitN]
PS: commit reference is a SHA-1 string of size 40
*/

bool Commit::readFromFile() {
    // check the existence of parent directory of commit file
    fs::path commit_parent_dir = Repository::COMMIT_DIR / m_commit_ref.substr(0,2);
    if (fs::exists(commit_parent_dir) == false) {
        return false;
    }

    // in case the commit reference is a short ref
    std::string commit_file_short = m_commit_ref.substr(2);
    std::list<fs::path> regular_files = plainFilenamesIn(commit_parent_dir);
    for (const auto& it : regular_files) {
        // if get the target file, quit the loop
        if (commit_file_short == it.string().substr(0, commit_file_short.size())) {
            commit_file_short = it.string();
            break;
        }
    }
    
    // check the eixistence of commit file
    fs::path commit_file = commit_parent_dir / commit_file_short;
    if (fs::exists(commit_file) == false) {
        return false;
    }

    readObject(commit_file, *this);
    return true;
}

void Commit::writeToFile() {
    fs::path commit_parent_dir = Repository::COMMIT_DIR / m_commit_ref.substr(0, 2);
    fs::create_directory(commit_parent_dir);
    fs::path commit_file = commit_parent_dir / m_commit_ref.substr(2);
    writeObject(commit_file, *this);    
}


// todo
void Commit::readFromRemoteRepo(const fs::path& remote_repo) {

}

// todo
void Commit::writeToRemoteRepo(const fs::path& remote_repo) {

}