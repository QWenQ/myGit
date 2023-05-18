#include "Tree.h"
#include "Repository.h"


std::string Tree::getHeadRef() const {
    return m_head;
}

void Tree::addNewBranch(const std::string& branch_name, const Commit& branch_head) {
    if (m_branches.find(branch_name) != m_branches.end()) {
        m_branches.erase(branch_name);
    }
    m_branches.emplace(branch_name, branch_head.getCommitRef());
}

void Tree::setActiveBranchToNULL() {
    m_active_branch = "";
}

void Tree::updateBranchHead(const std::string& branch_name, const Commit& branch_head) {
    if (branch_name == m_active_branch) {
        updateHead(branch_head.getCommitRef());
    }
    else {
        if (m_branches.find(branch_name) != m_branches.end()) {
            m_branches.erase(branch_name);
        }
        m_branches.emplace(branch_name, branch_head.getCommitRef());
    }
}

Commit Tree::getGivenBranchHead(const std::string& branch_name) const {
    std::string commit_ref = m_branches.at(branch_name);
    Commit commit = Commit();
    commit.setCommitRef(commit_ref);
    commit.readFromFile();
    return commit;
}

void Tree::updateHead(const std::string& commit_ref) {
    if (m_branches.find(m_active_branch) != m_branches.end()) {
        m_branches.erase(m_active_branch);
    }
    m_branches.emplace(m_active_branch, commit_ref);
    m_head = commit_ref;
}

void Tree::removeBranch(const std::string& branch_name) {
    if (m_active_branch == branch_name) return;
    m_branches.erase(branch_name);
}

void Tree::addNewBranch(const std::string& branch_name) {
    if (m_branches.find(branch_name) != m_branches.end()) {
        m_branches.erase(branch_name);
    }
    m_branches.emplace(branch_name, m_head);
}

const std::unordered_map<std::string, std::string>& Tree::getBranches() const {
    return m_branches;
}

bool Tree::isBranchExist(const std::string& branch_name) const {
    return m_branches.find(branch_name) != m_branches.end();
}

const std::string& Tree::getActiveBranch() const {
    return m_active_branch;
}

void Tree::setActiveBranch(const std::string& branch_name) {
    if (m_branches.find(branch_name) != m_branches.end()) {
        m_active_branch = branch_name;
        m_head = m_branches[branch_name];
    }
}

void Tree::add(Commit& commit) {
    commit.setParentRef(m_head);
    if (m_branches.find(m_active_branch) != m_branches.end()) {
        m_branches.erase(m_active_branch);
    }
    if (m_branches.size() == 0) {
        m_active_branch = "master";
    }
    m_branches.emplace(m_active_branch, commit.getCommitRef());
    m_head = commit.getCommitRef();
}

Commit Tree::getHead() const {
    Commit commit = Commit();
    commit.setCommitRef(m_head);
    commit.readFromFile();
    return commit;
}


// persistence

void Tree::readFromFile() {
    if (fs::exists(Repository::TREE) == false) {
        return;
    }
    readObject(Repository::TREE, *this);
}

void Tree::writeToFile() {
    writeObject(Repository::TREE, *this);
}


// todo
void Tree::readFromRemoteFile(const fs::path& remote_repo) {

}

// todo
void Tree::writeToRemoteFile(const fs::path& remote_repo) {
    
}