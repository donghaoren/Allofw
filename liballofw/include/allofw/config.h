#ifndef ALLOFW_CONFIG_H
#define ALLOFW_CONFIG_H

#include <memory>
#include <string>

#include "common.h"

namespace ALLOFW_NS {

    class Configuration {
    public:

        virtual void getString(const char* key, char* output, size_t output_capacity, const char* fallback = "") = 0;
        virtual size_t getStringLength(const char* key, const char* fallback = "") = 0;

        virtual int getInt32(const char* key, int fallback = 0) = 0;
        virtual unsigned int getUInt32(const char* key, unsigned int fallback = 0) = 0;
        virtual float getFloat(const char* key, float fallback = 0.0f) = 0;
        virtual double getDouble(const char* key, double fallback = 0.0) = 0;

        virtual void parseFile(const char* path) = 0;
        virtual void parseFile(const char* path, const char* key) = 0;

        virtual ~Configuration() { }

        // Constructor.
        static Configuration* Create();
    };

}

#endif
