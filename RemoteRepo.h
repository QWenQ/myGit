#ifndef REMOTEREPO_H
#define REMOTEREPO_H

#include "Tree.h"
#include "Utils.h"

/*
// include headers that implement a archive in simple text format
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/string.hpp>
*/

class RemoteRepo {
    public:
        RemoteRepo() 
        :   m_remote_name(), 
            m_branch_name(), 
            m_repos(),
            m_tree()
        { }

        // I/O
        void writeToConfigFile();
        void readFromConfigFile();

    private:
        friend class boost::serialization::access;
        template <typename Archive>
        void serialize(Archive& ar, const unsigned int version) {
            ar & m_remote_name;
            ar & m_branch_name;
            ar & m_repos;
            ar & m_tree;
        }

        std::string m_remote_name;
        std::string m_branch_name; 
        std::unordered_map<std::string, std::string> m_repos;
        Tree m_tree;
}; // class RemoteRepo

#endif // REMOTEREPO_H