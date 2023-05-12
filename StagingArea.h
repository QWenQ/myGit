#ifndef STAGINGAREA_H
#define STAGINGAREA_H

#include "Utils.h"
#include "Blob.h"


class StagingArea {
    public:

        StagingArea();

        bool containsBlob(const Blob& blob) const;
        bool containsInAddition(const Blob& blob) const;
        bool containsInRemoval(const Blob& blob) const;
        bool containsInRemoval(const std::string& filename) const;
        std::string getFileRefInAddition(const std::string& filename) const;
        bool containsInAddition(const std::string& filename) const;
        std::unordered_map<std::string, std::string>& getAddition() const;
        std::unordered_map<std::string, std::string>& getRemoval() const;
        void addToAddition(const Blob& blob);
        void removeFromAddition(const Blob& blob);
        void removeFromAddition(const std::string& filename);
        void addToRemoval(const Blob& blob);
        void removeFromRemoval(const std::string& filename);
        bool isEmpty() const;



        // I/O
        void readFromFile();
        void writeToFile();
    private:
        friend class boost::serialization::access;
        // When the class Archive corresponds to an output archive, the
        // & operator is defined similar to <<.  Likewise, when the class Archive
        // is a type of input archive the & operator is defined similar to >>.
        template <typename Archive> 
        void serialize(Archive& ar, const unsigned int version) {
            ar & m_addition;
            ar & m_removal;
        }

        std::unordered_map<std::string, std::string> m_addition;
        std::unordered_map<std::string, std::string> m_removal;

}; // class StagingArea

#endif // STAGINGAREA_H