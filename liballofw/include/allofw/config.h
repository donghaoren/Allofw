#ifndef ALLOFW_CONFIG_H
#define ALLOFW_CONFIG_H

#include "common.h"

#include <memory>
#include <string>

namespace allofw {

    //! YAML-based configuration reader.
    class Configuration {
    public:

        //! Config node.
        class Node {
        public:

            virtual bool isArray() const = 0;       //! Is this an array?
            virtual bool isDictionary() const = 0;  //! Is this a dictionary?
            virtual bool isValue() const = 0;       //! Is this a value?

            //! Get string.
            virtual void getString(char* output, size_t output_capacity, const char* fallback = "") const = 0;
            //! Get the length of the string.
            virtual size_t getStringLength(const char* fallback = "") const = 0;
            //! Get int32 value.
            virtual int getInt32(int fallback = 0) const = 0;
            //! Get uint32 value.
            virtual unsigned int getUInt32(unsigned int fallback = 0) const = 0;
            //! Get float value.
            virtual float getFloat(float fallback = 0.0f) const = 0;
            //! Get double value.
            virtual double getDouble(double fallback = 0.0) const = 0;
            //! Get boolean value.
            virtual bool getBoolean(bool fallback = 0.0) const = 0;

            //! Get STL string value.
            std::string getString(const char* fallback = "") {
                size_t len = getStringLength(fallback);
                char* buffer = new char[len + 1];
                getString(buffer, len + 1, fallback);
                std::string result(buffer);
                delete [] buffer;
                return result;
            }

            //! Get array item by index.
            virtual Node* get(int index) const = 0;
            //! Get dictionary item by key.
            virtual Node* get(const char* key) const = 0;
            //! Get size of the node (for array and dictionary).
            virtual size_t size() const = 0;

        protected:
            virtual ~Node() { }
        };

        //! Get a node with a key (pair with destroyNode).
        virtual Node* getNode(const char* key) = 0;
        //! Destroy the node.
        virtual void destroyNode(Node* node) = 0;

        //! Get the string at a given path.
        virtual void getString(const char* path, char* output, size_t output_capacity, const char* fallback = "") = 0;
        //! Get the length of the string at a give path.
        virtual size_t getStringLength(const char* path, const char* fallback = "") = 0;

        //! Get int32 at a given path.
        virtual int getInt32(const char* path, int fallback = 0) = 0;
        //! Get uint32 at a given path.
        virtual unsigned int getUInt32(const char* path, unsigned int fallback = 0) = 0;
        //! Get float at a given path.
        virtual float getFloat(const char* path, float fallback = 0.0f) = 0;
        //! Get double at a given path.
        virtual double getDouble(const char* path, double fallback = 0.0) = 0;
        //! Get boolean at a given path.
        virtual bool getBoolean(const char* path, bool fallback = 0.0) = 0;

        //! Parse a YAML file.
        virtual void parseFile(const char* path) = 0;
        //! Parse a YAML file to a give key.
        virtual void parseFile(const char* path, const char* key) = 0;

        //! Get STL string at a given path.
        std::string getString(const char* path, const char* fallback = "") {
            size_t len = getStringLength(path, fallback);
            char* buffer = new char[len + 1];
            getString(path, buffer, len + 1, fallback);
            std::string result(buffer);
            delete [] buffer;
            return result;
        }

        virtual ~Configuration() { }

        //! Create Configuration object.
        static Configuration* Create();
        //! Create Configuration object by C++ main() arguments.
        static Configuration* ParseMainArgs(int argc, char* argv[]);
        //! Create Configuration object from file.
        static Configuration* CreateFromFile(const char* args = nullptr);
    };

}

#endif
