#ifndef COMMIT_H
#define COMMIT_H

#include <string>

#include "Date.h"
#include "Utils.h"
#include "Blob.h"


class Commit {
    public:
        Commit();
        Commit(const std::string& message, std::time_t time);

        Commit getRoot() const;
        Commit getFirstParent() const;
        Commit getSecondParent() const;
        bool equalTo(const Commit& commit) const;

        void setLogMessage(const std::string& log_message);
        void setSecondParentRef(const std::string& second_parent_ref);
        void setBlobRef(const std::string& filename, const std::string& commit_ref);
        const std::unordered_map<std::string, std::string>& getFilesMap() const;
        bool isMergedCommit() const;
        void updateRef();
        const std::string& getMessage() const;
        std::string getDate() const;
        bool containFile(const std::string& filename) const;
        bool containFile(const std::string& filename, const std::string& blob_ref) const;
        void setCommitRef(const std::string& commitRef);
        std::string getBlobRef(const std::string& fileName) const;
        void updateDate();
        void updateMessage(const std::string& message);
        void addFileReference(const std::string& filename, const std::string& commit_ref);
        void removeFileReference(const std::string& filename, const std::string& commit_ref);
        const std::string& getSecondParentRef() const;
        const std::string& getParentRef() const;
        void setParentRef(const std::string& parentRef);
        const std::string& getCommitRef() const;
        bool containsBlob(const Blob& blob) const;


        // I/O
        bool readFromFile();
        void writeToFile();
        // remote repo I/O
        void readFromRemoteRepo(const fs::path& remote_repo);
        void writeToRemoteRepo(const fs::path& remote_repo);

    private:
        friend class boost::serialization::access;
        template <typename Archive>
        void serialize(Archive& ar, const unsigned int version) {
            ar & m_timestamp;
            ar & m_message;
            ar & m_parent_ref;
            ar & m_second_parent_ref;
            ar & m_commit_ref;
            ar & m_map;
        }

        Date m_timestamp;
        std::string m_message;
        std::string m_parent_ref;
        std::string m_second_parent_ref;
        std::string m_commit_ref;
        std::unordered_map<std::string, std::string> m_map;
}; // class Commit

#endif // COMMIT_H