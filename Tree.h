#ifndef TREE_H
#define TREE_H

#include "Commit.h"
#include "Utils.h"
class Tree {
    public:
        Tree() 
        :   m_head(),
            m_active_branch(),
            m_branches()
        { }

        std::string getHeadRef() const;
        void addNewBranch(const std::string& branch_name, const Commit& branch_head);
        void setActiveBranchToNULL();
        void updateBranchHead(const std::string& branch_name, const Commit& branch_head);
        Commit getGivenBranchHead(std::string& branch_name) const;
        void updateHead(const std::string& commit_ref);
        void removeBranch(const std::string& branch_name);
        void addNewBranch(const std::string& branch_name);
        const std::unordered_map<std::string, std::string>& getBranches() const;
        bool isBranchExist(const std::string& branch_name) const;
        const std::string& getActiveBranch() const;
        void setActiveBranch(const std::string& branch_name);
        void add(Commit& commit);
        Commit getHead() const;

        // I/O
        void readFromFile();
        void writeToFile();
        // remote repo I/O
        void readFromRemoteFile(const fs::path& remote_repo);
        void writeToRemoteFile(const fs::path& remote_repo);
    private:
        friend class boost::serialization::access;
        template <typename Archive> 
        void serialize(Archive& ar, const unsigned int version) {
            ar & m_head;
            ar & m_active_branch;
            ar & m_branches;
        }

        std::string m_head;
        std::string m_active_branch;
        std::unordered_map<std::string, std::string> m_branches;
}; // class Tree

#endif // TREE_H