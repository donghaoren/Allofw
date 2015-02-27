#ifndef ALLOFW_COMMON_H
#define ALLOFW_COMMON_H

#include <string>
#include <memory>

#define ALLOFW_NS allofw

#define ALLOFW_NS_BEGIN namespace ALLOFW_NS {
#define ALLOFW_NS_END }

ALLOFW_NS_BEGIN

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

class not_implemented_yet : public exception {
public:
    not_implemented_yet(const char* what_ = nullptr) : exception(what_) { }
};

class not_supported : public exception {
public:
    not_supported(const char* what_ = nullptr) : exception(what_) { }
};

class invalid_argument : public exception {
public:
    invalid_argument(const char* what_ = nullptr) : exception(what_) { }
};

ALLOFW_NS_END

#endif
