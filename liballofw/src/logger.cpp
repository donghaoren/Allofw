#include "allofw/logger.h"
#include <pthread.h>
#include <string>
#include <deque>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <stdio.h>

ALLOFW_NS_BEGIN

struct ScopedLogger::Impl {
    std::deque<std::string> prefix_stack;
};

ScopedLogger::ScopedLogger() {
    impl = new Impl();

    uint64_t tid;
    pthread_threadid_np(NULL, &tid);
    char buf[64];
    snprintf(buf, 64, "[T.%llu] ", tid);
    impl->prefix_stack.push_back(buf);
}
void ScopedLogger::pushScope(const char* prefix) {
    std::string previous_prefix;
    if(!impl->prefix_stack.empty()) previous_prefix = impl->prefix_stack.back();
    impl->prefix_stack.push_back(previous_prefix + prefix);
}
void ScopedLogger::popScope() {
    if(!impl->prefix_stack.empty()) {
        impl->prefix_stack.pop_back();
    }
}
void ScopedLogger::print(int level, const char* string) {
    std::string s(string);
    std::string prefix;
    if(!impl->prefix_stack.empty()) prefix = impl->prefix_stack.back();
    std::string indent = std::string("\n") + s;
    boost::algorithm::trim_right(indent);
    boost::algorithm::replace_all(indent, "\n", std::string("\n") + prefix);
    loggerOutput(level, indent.substr(1).c_str());
}
void ScopedLogger::printf(int level, const char* fmt, ...) {
    char buf[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, 1024, fmt, args);
    print(level, buf);
}
void ScopedLogger::vprintf(int level, const char* fmt, va_list args) {
    char buf[1024];
    vsnprintf(buf, 1024, fmt, args);
    print(level, buf);
}

ScopedLogger::~ScopedLogger() {
    delete impl;
}

class LoggerImplStderr : public ScopedLogger {
protected:
    virtual void loggerOutput(int level, const char* s) override {
        if(isatty(fileno(stderr))) {
            switch(level) {
                case kInfo: {
                    fprintf(stderr, "\x1b[0m%s\x1b[0m\n", s);
                } break;
                case kWarning: {
                    fprintf(stderr, "\x1b[1;33m%s\x1b[0m\n", s);
                } break;
                case kError: {
                    fprintf(stderr, "\x1b[1;31m%s\x1b[0m\n", s);
                } break;
                case kFatal: {
                    fprintf(stderr, "\x1b[1;37;41m%s\x1b[0m\n", s);
                } break;
            }
        } else {
            ::fputs(s, stderr);
            ::fputc('\n', stderr);
        }
    }
};

class LoggerFactoryStderr : public LoggerFactory {
public:

    LoggerFactoryStderr() {
        pthread_once(&thread_logger_key_once, logger_key_create);
    }

    static void logger_key_create() {
        pthread_key_create(&thread_logger_key, NULL);
    }

    struct LoggerInfo {
        Logger* logger;
        bool is_owner;
    };

    static void logger_destructor(void*) {
        LoggerInfo* info = (LoggerInfo*)pthread_getspecific(thread_logger_key);
        if(info->is_owner) {
            delete info->logger;
        }
        delete info;
    }

    virtual Logger* getThreadLogger() override {
        LoggerInfo* info = (LoggerInfo*)pthread_getspecific(thread_logger_key);
        if(!info) {
            info = new LoggerInfo();
            info->logger = new LoggerImplStderr();
            info->is_owner = true;
            pthread_setspecific(thread_logger_key, info);
        }
        return info->logger;
    }

    virtual void setThreadLogger(Logger* logger) override {
        LoggerInfo* info = (LoggerInfo*)pthread_getspecific(thread_logger_key);
        if(info->is_owner) {
            delete info->logger;
            info->is_owner = false;
        }
        info->logger = logger;
    }

    static pthread_key_t thread_logger_key;
    static pthread_once_t thread_logger_key_once;
};

pthread_key_t LoggerFactoryStderr::thread_logger_key;
pthread_once_t LoggerFactoryStderr::thread_logger_key_once = PTHREAD_ONCE_INIT;

LoggerFactory* default_logger_factory = new LoggerFactoryStderr();

LoggerFactory* LoggerFactory::Default() {
    return default_logger_factory;
}
void LoggerFactory::SetDefault(LoggerFactory* factory) {
    default_logger_factory = factory;
}

LoggerScope::LoggerScope(int level, const char* header, const char* prefix, Logger* logger)
: logger_(logger), level_(level) {
    if(header) logger_->print(level, header);
    logger_->pushScope(prefix);
}

void LoggerScope::print(const char* string) {
    logger_->print(level_, string);
}
void LoggerScope::printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    logger_->vprintf(level_, fmt, args);
}

LoggerScope::~LoggerScope() {
    logger_->popScope();
}

ALLOFW_NS_END
