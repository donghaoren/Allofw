#ifndef ALLOFW_LOGGER_H
#define ALLOFW_LOGGER_H

#include "common.h"

namespace ALLOFW_NS {

    class Logger;

    class LoggerFactory {
    public:
        // The factory function should be thread-safe, but the returned logger might not.
        virtual Logger* getThreadLogger() = 0;
        // Set the logger for the current thread, so the next time getThreadLogger will return this logger.
        // The caller is responsible for releasing the logger.
        virtual void setThreadLogger(Logger* logger) = 0;

        virtual ~LoggerFactory() { }

        // Default logger writes to stderr.
        static LoggerFactory* Default();
        static void SetDefault(LoggerFactory* factory);
    };

    class Logger {
    public:
        // Log levels.
        static const int kInfo     = 0;
        static const int kWarning  = 1;
        static const int kError    = 2;
        static const int kFatal    = 3;

        // Scope functionality.
        virtual void pushScope(const char* prefix = "    ") = 0;
        virtual void popScope() = 0;
        // Output.
        virtual void print(int level, const char* string) = 0;
        virtual void printf(int level, const char* fmt, ...) = 0;
        virtual void vprintf(int level, const char* fmt, va_list args) = 0;

        virtual ~Logger() { }

        // Get logger from the default logger factory.
        // The function is thread-safe, but the returned logger is not.
        inline static Logger* Default() { return LoggerFactory::Default()->getThreadLogger(); }
    };

    // This class implements basic scope functions, and also keeps thread ID upon creation.
    class ScopedLogger : public Logger, non_copyable {
    public:
        ScopedLogger();
        virtual void pushScope(const char* prefix = "    ") override;
        virtual void popScope() override;
        virtual void print(int level, const char* string) override;
        virtual void printf(int level, const char* fmt, ...) override;
        virtual void vprintf(int level, const char* fmt, va_list args) override;
        ~ScopedLogger();
    protected:
        // Implement this to suit your needs.
        virtual void loggerOutput(int level, const char* string) = 0;
    private:
        struct Impl;
        Impl* impl;
    };

    // Handy class to be used with C++ scopes.
    class LoggerScope {
    public:
        LoggerScope(int level, const char* header = nullptr, const char* prefix = "  ", Logger* logger = Logger::Default());
        inline void operator()(const char* str) { print(str); }
        void print(const char* string);
        void printf(const char* fmt, ...);
        ~LoggerScope();
    private:
        LoggerScope(const LoggerScope&);
        LoggerScope& operator = (const LoggerScope&);
        Logger* logger_;
        int level_;
    };

}

#endif
