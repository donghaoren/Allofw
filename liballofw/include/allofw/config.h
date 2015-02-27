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

        virtual ~Configuration();

        static Configuration* LoadFromFile_(const char* path);
        static PConfiguration LoadFromFile(const std::string& path) {
            return PConfiguration(LoadFromFile_(path.c_str()));
        }
        static Configuration* LoadFromString_(const char* str);
        static PConfiguration LoadFromString(const std::string& str) {
            return PConfiguration(LoadFromString_(str.c_str()));
        }
    };
}

#endif
