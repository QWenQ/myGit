#ifndef DATE_H
#define DATE_H

#include <ctime>
#include "Utils.h"

/*
// include headers that implement a archive in simple text format
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/string.hpp>
*/

class Date {
    public:
        Date(std::time_t time) 
        :   m_t(time)
        { }

        Date() { 
            m_t = std::time(NULL);
        }

        ~Date() { }

        std::string toString() const {
            char ret[50];
            std::strftime(ret, sizeof ret, "%c %z", std::localtime(&m_t));
            return ret;
        }
    private:
        friend class boost::serialization::access;
        template <typename Archive>
        void serialize(Archive& ar, const unsigned int version) {
            ar & m_t;
        }
        std::time_t m_t;
}; // class Date

#endif // DATE_H