#include <allofw/stream.h>
#include <cstdio>
#include <cstring>

using namespace std;

ALLOFW_NS_BEGIN

size_t ByteStream::read(void* buffer, size_t length) { return 0; }
size_t ByteStream::write(const void* buffer, size_t length) { return 0; }
void ByteStream::seek(SeekOrigin origin, std::ptrdiff_t pos) { }
std::ptrdiff_t ByteStream::position() { return 0; }
void ByteStream::flush() { }
bool ByteStream::canRead() { return false; }
bool ByteStream::canWrite() { return false; }
bool ByteStream::canSeek() { return false; }

namespace {
    class FileStream : public ByteStream {
    public:
        FileStream(const std::string& path, const std::string& mode) {
            fp = fopen(path.c_str(), mode.c_str());
            //if(!fp) throw invalid_argument("couldn't open file");
            if(mode == "a" || mode == "w") writable = true;
            else writable = false;
        }

        virtual size_t read(void* buffer, size_t length) {
            return fread(buffer, 1, length, fp);
        }
        virtual size_t write(const void* buffer, size_t length) {
            return fwrite(buffer, 1, length, fp);
        }
        virtual void seek(SeekOrigin origin, std::ptrdiff_t pos) {
            switch(origin) {
                case BEGIN: fseek(fp, pos, SEEK_SET); break;
                case CURRENT: fseek(fp, pos, SEEK_CUR); break;
                case END: fseek(fp, pos, SEEK_END); break;
            }
        }
        virtual std::ptrdiff_t position() {
            return ftell(fp);
        }
        virtual void flush() {
            fflush(fp);
        }
        virtual bool canRead() { return true; }
        virtual bool canWrite() { return true; }
        virtual bool canSeek() { return true; }

        virtual ~FileStream() {
            if(fp) fclose(fp);
        }

        FILE* fp;
        bool writable;
    };
}

ByteStream* ByteStream::OpenFile(const char* path, const char* mode) {
    FileStream* f = new FileStream(path, mode);
    if(!f->fp) {
        delete f;
        return NULL;
    }
    return f;
}

exception::exception(const char* what_) {
    if(!what_) what_ = "unknown exception";
    size_t len = strlen(what_);
    description = new char[len + 1];
    strcpy(description, what_);
}
const char* exception::what() const {
    return description;
}
exception::exception(const exception& e) {
    size_t len = strlen(e.description);
    description = new char[len + 1];
    strcpy(description, e.description);
}
exception& exception::operator = (const exception& e) {
    delete [] description;
    size_t len = strlen(e.description);
    description = new char[len + 1];
    strcpy(description, e.description);
    return *this;
}
exception::~exception() {
    delete [] description;
}

// std::string get_file_contents(const std::string& filename) {
//     std::unique_ptr<ByteStream> file(ByteStream::OpenFile(filename, "r"));
//     std::string contents;
//     file->seek(ByteStream::END, 0);
//     contents.resize(file->position());
//     file->seek(ByteStream::BEGIN, 0);
//     file->read(&contents[0], contents.size());
//     return contents;
// }

ALLOFW_NS_END
