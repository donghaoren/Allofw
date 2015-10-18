#ifndef ALLOFW_COMMON_H
#define ALLOFW_COMMON_H

#include <memory>

// Don't use these in the header files, otherwise doxygen cannot show the right namespace.
#define ALLOFW_NS allofw
#define ALLOFW_NS_BEGIN namespace allofw {
#define ALLOFW_NS_END }

namespace allofw {

    //! Allofw exception class.
    class exception {
    public:
        exception(const char* what_ = nullptr);
        exception(const exception& e);
        exception& operator = (const exception& e);
        virtual const char* what() const;
        virtual ~exception();
    private:
        char* description;
    };

    //! Not implemented exception.
    class not_implemented_yet : public exception {
    public:
        not_implemented_yet(const char* what_ = nullptr) : exception(what_) { }
    };

    //! Not supported exception.
    class not_supported : public exception {
    public:
        not_supported(const char* what_ = nullptr) : exception(what_) { }
    };

    //! Invalid argument exception.
    class invalid_argument : public exception {
    public:
        invalid_argument(const char* what_ = nullptr) : exception(what_) { }
    };

    //! Inherit this to make a class non-copyable.
    class non_copyable {
    public:
        non_copyable() = default;
    private:
        non_copyable(const non_copyable&);
        non_copyable& operator = (const non_copyable&);
    };

}

#endif
