#ifndef ALLOFW_STREAM_H
#define ALLOFW_STREAM_H

#include "common.h"

namespace ALLOFW_NS {

    class ByteStream {
    public:
        enum SeekOrigin {
            BEGIN, END, CURRENT
        };
        virtual size_t read(void* buffer, size_t length);
        virtual size_t write(const void* buffer, size_t length);
        virtual void seek(SeekOrigin origin, std::ptrdiff_t pos);
        virtual std::ptrdiff_t position();
        virtual void flush();
        virtual bool canRead();
        virtual bool canWrite();
        virtual bool canSeek();

        // mode = "r" / "w" / "a"
        static ByteStream* OpenFile(const char* path, const char* mode);
        static void Destroy(ByteStream* stream);

    protected:
        virtual ~ByteStream();
    };

    class io_error : public std::exception { };

}

#endif
