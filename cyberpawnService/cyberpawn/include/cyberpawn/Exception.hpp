#ifndef CYBERPAWN_CYBERPAWN_EXCEPTION_H_INCLUDED
#define CYBERPAWN_CYBERPAWN_EXCEPTION_H_INCLUDED

#include <exception>
#include <string>


namespace cyberpawn {


    class Exception : public std::exception {
    private:
        std::string message_;
    public:
        Exception(const std::string & message) : message_(message) {}

        const char * what() const {
            return message_.c_str();
        }
        const std::string & to_string() const {
            return message_;
        }
    };


}




#endif // CYBERPAWN_CYBERPAWN_EXCEPTION_H_INCLUDED
