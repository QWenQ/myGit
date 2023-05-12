#ifndef BLOB_H
#define BLOB_H

/*
// include headers that implement a archive in simple text format
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/string.hpp>
*/

// user defined header
#include "Utils.h"


class Blob {
    public:
        Blob();

        void setFilename(const std::string& filename);
        void update();
        void setContent(const std::string& content);
        const std::string& getContent() const;
        void setFileRef(const std::string& blob_ref);
        const std::string& getFileRef() const;
        const std::string& getFilename() const;

        
        // I/O
        void readFromOriginalFile(const fs::path& filename);
        void writeToOriginalFile();
        void readFromBlobFile();
        void writeToBlobFile();
        // remote repo I/O
        void writeToRemoteBlobFile(const fs::path& remote_repo);
        void readFromRemoteBlobFile(const fs::path& remote_repo);
    private:
        friend class boost::serialization::access;
        template <typename Archive> 
        void serialize(Archive& ar, const unsigned int version) {
            ar & m_filename;
            ar & m_content;
            ar & m_blob_ref;
        }

        std::string m_filename;
        std::string m_content;
        std::string m_blob_ref;
}; // class Blbo

#endif // BLOB_H