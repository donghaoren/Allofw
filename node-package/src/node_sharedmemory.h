#include <node.h>
#include <v8.h>
#include <nan.h>

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

    static NAN_METHOD(New);

    static NAN_METHOD(NODE_size);
    static NAN_METHOD(NODE_shmid);
    static NAN_METHOD(NODE_semid);
    static NAN_METHOD(NODE_buffer);
    static NAN_METHOD(NODE_delete);
    static NAN_METHOD(NODE_close);

    static NAN_METHOD(NODE_writeLock);
    static NAN_METHOD(NODE_writeUnlock);
    static NAN_METHOD(NODE_readLock);
    static NAN_METHOD(NODE_readUnlock);

    static v8::Persistent<v8::Function> constructor;
};
