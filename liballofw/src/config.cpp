#include "allofw/config.h"
#include "allofw/logger.h"
#include <vector>
#include <yaml-cpp/yaml.h>
#include <iostream>
#include <string.h>

#ifdef __APPLE__
#include <unistd.h>
#endif

ALLOFW_NS_BEGIN

class ConfigurationImpl : public Configuration {
public:

    class NodeImpl : public Node {
    public:
        NodeImpl(const YAML::Node& node) : content(node) { }

        virtual bool isArray() const {
            return content.IsSequence();
        }
        virtual bool isDictionary() const {
            return content.IsMap();
        }
        virtual bool isValue() const {
            return content.IsScalar();
        }
        virtual void getString(char* output, size_t output_capacity, const char* fallback = "") const {
            std::string s = content.as<std::string>(fallback);
            strcpy(output, s.c_str());
        }
        virtual size_t getStringLength(const char* fallback = "") const {
            std::string s = content.as<std::string>(fallback);
            return s.size();
        }
        virtual int getInt32(int fallback = 0) const {
            return content.as<int>(fallback);
        }
        virtual unsigned int getUInt32(unsigned int fallback = 0) const {
            return content.as<unsigned int>(fallback);
        }
        virtual float getFloat(float fallback = 0.0f) const {
            return content.as<float>(fallback);
        }
        virtual double getDouble(double fallback = 0.0) const {
            return content.as<double>(fallback);
        }
        virtual bool getBoolean(bool fallback = 0.0) const {
            return content.as<bool>(fallback);
        }


        // Get for array and dict.
        virtual Node* get(int index) const {
            return new NodeImpl(content[index]);
        }
        virtual Node* get(const char* key) const {
            return new NodeImpl(content[key]);
        }
        virtual size_t size() const {
            return content.size();
        }

        virtual ~NodeImpl() { }

    private:
        YAML::Node content;
        std::string myString;
    };

    virtual Node* getNode(const char* key) {
        std::string path = key;
        YAML::Node result;
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
                return new NodeImpl(node);
            }
        }
        return NULL;
    }
    virtual void destroyNode(Node* node) {
        delete (NodeImpl*)node;
    }

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
    virtual bool getBoolean(const char* key, bool fallback = 0.0) {
        return get<bool>(key, fallback);
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
        try {
            entries.push_back(YAML::LoadFile(path));
        } catch(std::exception& e) {
            throw exception(e.what());
        }
    }
    virtual void parseFile(const char* path, const char* key) {
        try {
            YAML::Node part = YAML::LoadFile(path)[key];
            if(!part.IsNull() && part.IsDefined())
                entries.push_back(part);
        } catch(std::exception& e) {
            throw exception(e.what());
        }
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

private:
    std::vector<YAML::Node> entries;
};

Configuration* Configuration::Create() {
    return new ConfigurationImpl();
}

Configuration* Configuration::ParseMainArgs(int argc, char* argv[]) {
    if(argc == 2) return CreateFromFile(argv[1]);
    return CreateFromFile();
}

Configuration* Configuration::CreateFromFile(const char* args) {
    Configuration* config = Configuration::Create();
    const char* config_path = "allofw.yaml";
    if(args) config_path = args;
    try {
        char hostname[256];
        gethostname(hostname, 256);
        config->parseFile(config_path);
        config->parseFile(config_path, hostname);
    } catch(allofw::exception& e) {
        Logger::Default()->printf(Logger::kWarning, "OmniApp: failed to read config file '%s', using defaults.", config_path);
        Logger::Default()->pushScope("> ");
        Logger::Default()->printf(Logger::kWarning, e.what());
        Logger::Default()->popScope();
    }
    return config;
}

ALLOFW_NS_END
