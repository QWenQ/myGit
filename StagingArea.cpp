#include "StagingArea.h"
#include "Repository.h"


StagingArea::StagingArea() 
    :   m_addition(),
        m_removal()
{ }

bool StagingArea::containsBlob(const Blob& blob) const {
    return containsInAddition(blob) || containsInRemoval(blob);
}

bool StagingArea::containsInAddition(const Blob& blob) const {
    return m_addition.find(blob.getFilename()) != m_addition.end()
            && m_addition.at(blob.getFilename()) == blob.getFileRef();
}

bool StagingArea::containsInRemoval(const Blob& blob) const {
    return m_removal.find(blob.getFilename()) != m_removal.end()
            && m_removal.at(blob.getFilename()) == blob.getFileRef();
}

bool StagingArea::containsInRemoval(const std::string& filename) const {
    return m_removal.find(filename) != m_removal.end();
}

std::string StagingArea::getFileRefInAddition(const std::string& filename) const {
    if (containsInAddition(filename) == false) return "";
    return m_addition.at(filename);
}

bool StagingArea::containsInAddition(const std::string& filename) const {
    return m_addition.find(filename) != m_addition.end();
}

std::unordered_map<std::string, std::string>& StagingArea::getAddition() {
    return m_addition;
}

std::unordered_map<std::string, std::string>& StagingArea::getRemoval() {
    return m_removal;
}

void StagingArea::addToAddition(const Blob& blob) {
    if (m_addition.find(blob.getFilename()) != m_addition.end()) {
        m_addition.erase(blob.getFilename());
    }
    m_addition.emplace(blob.getFilename(), blob.getFileRef());
}

void StagingArea::removeFromAddition(const Blob& blob) {
    if (containsInAddition(blob) == false) return;
    m_addition.erase(blob.getFilename());
}

void StagingArea::removeFromAddition(const std::string& filename) {
    m_addition.erase(filename);
}

void StagingArea::addToRemoval(const Blob& blob) {
    if (m_removal.find(blob.getFilename()) != m_removal.end()) {
        m_removal.erase(blob.getFilename());
    }
    m_removal.emplace(blob.getFilename(), blob.getFileRef());
}

void StagingArea::removeFromRemoval(const std::string& filename) {
    m_removal.erase(filename);
}

bool StagingArea::isEmpty() const {
    return m_addition.empty() && m_removal.empty();
}


// persistence

void StagingArea::readFromFile() {
    if (fs::exists(Repository::INDEX) == false) {
        return;
    }
    readObject(Repository::INDEX, *this); 
}

void StagingArea::writeToFile() {
    writeObject(Repository::INDEX, *this);	
}
