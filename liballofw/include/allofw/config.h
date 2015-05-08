#ifndef ALLOFW_CONFIG_H
#define ALLOFW_CONFIG_H

#include <memory>
#include <string>

#include "common.h"

namespace ALLOFW_NS {
    class Configuration;
    typedef shared_ptr<Configuration> PConfiguration;

    class Configuration {
    public:

        virtual char* getString(const char* key, const char* fallback = "") = 0;
        virtual void getStringFree(char* s) = 0;

        virtual int getInt32(const char* key, int fallback = 0) = 0;
        virtual unsigned int getUInt32(const char* key, unsigned int fallback = 0) = 0;
        virtual float getFloat(const char* key, float fallback = 0.0f) = 0;
        virtual double getDouble(const char* key, double fallback = 0.0) = 0;

        virtual void parseFile(const char* path) = 0;
        virtual void parseFile(const char* path, const char* key) = 0;

        virtual ~Configuration() { }

        // C++ convenient functions.
        template <typename T> T get(const std::string& key, T fallback = T());
        void parseFile(const std::string& path) { parseFile(path.c_str()); }
        void parseFile(const std::string& path, const std::string& key) { parseFile(path.c_str(), key.c_str()); }

        // Constructor.
        static Configuration* Create_();
        static PConfiguration Create() { return PConfiguration(Create_()); }
    };

    template<> std::string Configuration::get<std::string>(const std::string& key, std::string fallback);
    template<> int Configuration::get<int>(const std::string& key, int fallback);
    template<> unsigned int Configuration::get<unsigned int>(const std::string& key, unsigned int fallback);
    template<> float Configuration::get<float>(const std::string& key, float fallback);
    template<> double Configuration::get<double>(const std::string& key, double fallback);
}

#endif
