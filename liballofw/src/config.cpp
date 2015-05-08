#include "allofw/config.h"

#include <vector>
#include <yaml-cpp/yaml.h>
#include <iostream>

ALLOFW_NS_BEGIN

class ConfigurationImpl : public Configuration {
public:

    std::string getStringEx(const char* key, const char* fallback = "") {
        return get<std::string>(key, fallback);
    }

    virtual void getString(const char* key, char* output, size_t output_capacity, const char* fallback) {
        std::string r = getStringEx(key, fallback);
        strcpy(output, r.c_str());
    }
    virtual size_t getStringLength(const char* key, const char* fallback = "") {
        std::string r = getStringEx(key, fallback);
        return r.size();
    }

    virtual int getInt32(const char* key, int fallback = 0) {
        return get<int>(key, fallback);
    }
    virtual unsigned int getUInt32(const char* key, unsigned int fallback = 0) {
        return get<unsigned int>(key, fallback);
    }
    virtual float getFloat(const char* key, float fallback = 0.0f) {
        return get<float>(key, fallback);
    }
    virtual double getDouble(const char* key, double fallback = 0.0) {
        return get<double>(key, fallback);
    }

    virtual void parseFile(const char* path) {
        entries.push_back(YAML::LoadFile(path));
    }
    virtual void parseFile(const char* path, const char* key) {
        YAML::Node part = YAML::LoadFile(path)[key];
        if(!part.IsNull() && part.IsDefined())
            entries.push_back(part);
    }

    template<typename T>
    T get(const std::string& path, const T& fallback = T()) {
        for(size_t i = 0; i < entries.size(); i++) {
            YAML::Node node = entries[i];
            size_t pos = 0;
            while(pos < path.size()) {
                std::string subpath;
                size_t end = path.find('.', pos);
                if(end == std::string::npos) {
                    subpath = path.substr(pos);
                    pos = path.size();
                } else {
                    subpath = path.substr(pos, end - pos);
                    pos = end + 1;
                }
                if(!node.IsNull()) {
                    node.reset(node[subpath]);
                }
            }
            if(!node.IsNull() && node.IsDefined()) {
                return node.as<T>();
            }
        }
        return fallback;
    }

    std::vector<YAML::Node> entries;
};

Configuration* Configuration::Create() {
    return new ConfigurationImpl();
}

ALLOFW_NS_END
