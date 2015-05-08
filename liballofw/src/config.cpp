#include "allofw/config.h"

#include <vector>
#include <yaml-cpp/yaml.h>
#include <iostream>

ALLOFW_NS_BEGIN

class ConfigurationImpl : public Configuration {
public:

    virtual char* getString(const char* key, const char* fallback = "") {
        std::string result = get<std::string>(key, fallback);
        char* p = new char[result.size() + 1];
        strcpy(p, result.c_str());
        return p;
    }
    virtual void getStringFree(char* s) {
        delete [] s;
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

Configuration* Configuration::Create_() {
    return new ConfigurationImpl();
}

template<> std::string Configuration::get<std::string>(const std::string& key, std::string fallback) {
    char* p = getString(key.c_str(), fallback.c_str());
    std::string r(p);
    getStringFree(p);
    return r;
}

template<> int Configuration::get<int>(const std::string& key, int fallback) {
    return getInt32(key.c_str(), fallback);
}

template<> unsigned int Configuration::get<unsigned int>(const std::string& key, unsigned int fallback) {
    return getUInt32(key.c_str(), fallback);
}

template<> float Configuration::get<float>(const std::string& key, float fallback) {
    return getFloat(key.c_str(), fallback);
}

template<> double Configuration::get<double>(const std::string& key, double fallback) {
    return getDouble(key.c_str(), fallback);
}

ALLOFW_NS_END
