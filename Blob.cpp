#include "Blob.h"
#include "Repository.h"

Blob::Blob() 
:   m_filename(),
    m_content(),
    m_blob_ref()
    { }

void Blob::setFilename(const std::string& filename) {
    m_filename = filename;
}

void Blob::update() {
    m_blob_ref = sha1({m_content});
}

void Blob::setContent(const std::string& content) {
    m_content = content;
}

const std::string& Blob::getContent() const {
    return m_content;
}

void Blob::setFileRef(const std::string& blob_ref) {
    m_blob_ref = blob_ref;
}

const std::string& Blob::getFileRef() const {
    return m_blob_ref;
}

const std::string& Blob::getFilename() const {
    return m_filename;
}


void Blob::readFromOriginalFile(const fs::path& filename) {
    if (fs::exists(filename) == false) {
        return;
    }
    m_filename = filename;
    m_content = readContent(filename);
    m_blob_ref = sha1({m_content});
}

void Blob::writeToOriginalFile() {
    writeContents(m_filename, {m_content});
}

void Blob::readFromBlobFile() {
    fs::path filename = Repository::BLOB_DIR / m_blob_ref;
    if (fs::exists(filename) == false) {
        return;
    }
    readObject(filename, *this);
}

void Blob::writeToBlobFile() {
    fs::path filename = Repository::BLOB_DIR / m_blob_ref;
    writeObject(filename, *this);
}


// todo
void Blob::readFromRemoteBlobFile(const fs::path& remote_repo) {

} 

// todo
void Blob::writeToRemoteBlobFile(const fs::path& remote_repo) {

}
