#ifndef ALLOFW_CONFIG_H
#define ALLOFW_CONFIG_H

#include "common.h"

#include <memory>
#include <string>

namespace ALLOFW_NS {

    class Configuration {
    public:

        class Node {
        public:
            virtual bool isArray() const = 0;
            virtual bool isDictionary() const = 0;
            virtual bool isValue() const = 0;

            virtual void getString(char* output, size_t output_capacity, const char* fallback = "") const = 0;
            virtual size_t getStringLength(const char* fallback = "") const = 0;
            virtual int getInt32(int fallback = 0) const = 0;
            virtual unsigned int getUInt32(unsigned int fallback = 0) const = 0;
            virtual float getFloat(float fallback = 0.0f) const = 0;
            virtual double getDouble(double fallback = 0.0) const = 0;
            virtual bool getBoolean(bool fallback = 0.0) const = 0;

            std::string getString(const char* fallback = "") {
                size_t len = getStringLength(fallback);
                char* buffer = new char[len + 1];
                getString(buffer, len + 1, fallback);
                std::string result(buffer);
                delete [] buffer;
                return result;
            }

            // Get for array and dict.
            virtual Node* get(int index) const = 0;
            virtual Node* get(const char* key) const = 0;
            virtual size_t size() const = 0;

        protected:
            virtual ~Node() { }
        };

        virtual Node* getNode(const char* key) = 0;
        virtual void destroyNode(Node* node) = 0;

        virtual void getString(const char* key, char* output, size_t output_capacity, const char* fallback = "") = 0;
        virtual size_t getStringLength(const char* key, const char* fallback = "") = 0;

        virtual int getInt32(const char* key, int fallback = 0) = 0;
        virtual unsigned int getUInt32(const char* key, unsigned int fallback = 0) = 0;
        virtual float getFloat(const char* key, float fallback = 0.0f) = 0;
        virtual double getDouble(const char* key, double fallback = 0.0) = 0;
        virtual bool getBoolean(const char* key, bool fallback = 0.0) = 0;

        virtual void parseFile(const char* path) = 0;
        virtual void parseFile(const char* path, const char* key) = 0;

        std::string getString(const char* key, const char* fallback = "") {
            size_t len = getStringLength(key, fallback);
            char* buffer = new char[len + 1];
            getString(key, buffer, len + 1, fallback);
            std::string result(buffer);
            delete [] buffer;
            return result;
        }

        virtual ~Configuration() { }

        // Constructor.
        static Configuration* Create();
        static Configuration* ParseMainArgs(int argc, char* argv[]);
        static Configuration* ParseArgs(const char* args = nullptr);
    };

}

#endif
