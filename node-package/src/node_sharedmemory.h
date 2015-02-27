#include <node.h>
#include <v8.h>

class NODE_SharedMemory : public node::ObjectWrap {
public:
    static void Init(v8::Handle<v8::Object> exports);

    int key, size;
    int shm_id, sem_id;
    unsigned char* shm_data;
    bool is_create;

private:
    explicit NODE_SharedMemory(int key, int size, bool is_create);
    explicit NODE_SharedMemory(int shmid, int semid, int size, bool is_create);
    ~NODE_SharedMemory();

    static v8::Handle<v8::Value> New(const v8::Arguments& args);

    static v8::Handle<v8::Value> NODE_size(const v8::Arguments& args);
    static v8::Handle<v8::Value> NODE_shmid(const v8::Arguments& args);
    static v8::Handle<v8::Value> NODE_semid(const v8::Arguments& args);
    static v8::Handle<v8::Value> NODE_buffer(const v8::Arguments& args);
    static v8::Handle<v8::Value> NODE_delete(const v8::Arguments& args);
    static v8::Handle<v8::Value> NODE_close(const v8::Arguments& args);

    static v8::Handle<v8::Value> NODE_writeLock(const v8::Arguments& args);
    static v8::Handle<v8::Value> NODE_writeUnlock(const v8::Arguments& args);
    static v8::Handle<v8::Value> NODE_readLock(const v8::Arguments& args);
    static v8::Handle<v8::Value> NODE_readUnlock(const v8::Arguments& args);

    static v8::Persistent<v8::Function> constructor;
};
