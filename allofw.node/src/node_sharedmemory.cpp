#include <node.h>
#include <node_buffer.h>
#include <v8.h>

#include "node_sharedmemory.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include <stdexcept>

using namespace v8;

void NODE_SharedMemory::Init(Handle<Object> exports) {
    Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
    tpl->SetClassName(Nan::New<String>("SharedMemory").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    Nan::SetPrototypeMethod(tpl, "size", NODE_size);
    Nan::SetPrototypeMethod(tpl, "shmid", NODE_shmid);
    Nan::SetPrototypeMethod(tpl, "semid", NODE_semid);
    Nan::SetPrototypeMethod(tpl, "buffer", NODE_buffer);
    Nan::SetPrototypeMethod(tpl, "delete", NODE_delete);
    Nan::SetPrototypeMethod(tpl, "close", NODE_close);
    Nan::SetPrototypeMethod(tpl, "writeLock", NODE_writeLock);
    Nan::SetPrototypeMethod(tpl, "writeUnlock", NODE_writeUnlock);
    Nan::SetPrototypeMethod(tpl, "readLock", NODE_readLock);
    Nan::SetPrototypeMethod(tpl, "readUnlock", NODE_readUnlock);

    constructor.Reset(tpl->GetFunction());

    // Export constructor.
    Nan::Set(exports, Nan::New<String>("SharedMemory").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

int shmrm(key_t key) {
    if(key) {
        int id = shmget(key, 0, 0);
        if(id == -1)
            return -1;
        return shmctl(id, IPC_RMID, NULL);
    }
    return -1;
}

int semrm(key_t key) {
    if(key) {
        int id = semget(key, 0, 0);
        if(id == -1)
            return -1;
        return semctl(id, 0, IPC_RMID, NULL);
    }
    return -1;
}

// open(key: int, size: int) -> [ shm_id, sem_id ]
NODE_SharedMemory::NODE_SharedMemory(int key, int size_, bool is_create_) {
    is_create = is_create_;
    shm_data = NULL;
    size = size_;
    if(is_create) {
        // Try to remove first.
        shmrm(key);
        semrm(key);

        shm_id = shmget(key, size, IPC_CREAT | IPC_EXCL | 0666);
        if(shm_id < 0) { perror("shmget"); return; }
        sem_id = semget(key, 2, IPC_CREAT | IPC_EXCL | 0666);
        if(sem_id < 0) { perror("semget"); return; }
        short sarray[2] = { 0, 0 };
        semctl(sem_id, 0, SETALL, sarray);

        shm_data = (unsigned char*)shmat(shm_id, NULL, 0);
    } else {
        shm_id = shmget(key, size, 0666);
        if(shm_id < 0) { perror("shmget"); return; }
        sem_id = semget(key, 2, 0666);
        if(sem_id < 0) { perror("semget"); return; }

        shm_data = (unsigned char*)shmat(shm_id, NULL, 0);
    }
}

NODE_SharedMemory::NODE_SharedMemory(int shmid, int semid, int size_, bool is_create_) {
    is_create = is_create_;
    shm_data = NULL;
    size = size_;
    if(is_create) {
        shm_id = shmget(IPC_PRIVATE, size, IPC_CREAT | 0666);
        if(shm_id < 0) { perror("shmget2"); return; }
        sem_id = semget(IPC_PRIVATE, 2, IPC_CREAT | 0666);
        if(sem_id < 0) { perror("semget2"); return; }
        short sarray[2] = { 0, 0 };
        semctl(sem_id, 0, SETALL, sarray);

        shm_data = (unsigned char*)shmat(shm_id, NULL, 0);
    } else {
        shm_id = shmid;
        sem_id = semid;
        shm_data = (unsigned char*)shmat(shm_id, NULL, 0);
    }
}

NODE_SharedMemory::~NODE_SharedMemory() {
    if(is_create) {
        if(sem_id >= 0) semctl(sem_id, 0, IPC_RMID, 0);
        if(shm_id >= 0) shmctl(shm_id, IPC_RMID, 0);
    }
}

NAN_METHOD(NODE_SharedMemory::New) {
    Nan::HandleScope scope;
    if(info.IsConstructCall()) {
        if(info.Length() == 3) {
            int key = info[0]->IntegerValue();
            int size = info[1]->IntegerValue();
            bool is_create = info[2]->BooleanValue();
            NODE_SharedMemory* obj = new NODE_SharedMemory(key, size, is_create);
            if(obj->shm_id < 0 || obj->sem_id < 0) {
                Nan::ThrowError("SharedMemory: shmget()/semget() failed.");
            }
            obj->Wrap(info.This());
            info.GetReturnValue().Set(info.This());
        } else if(info.Length() == 4) {
            int shmid = info[0]->IntegerValue();
            int semid = info[1]->IntegerValue();
            int size = info[2]->IntegerValue();
            bool is_create = info[3]->BooleanValue();
            NODE_SharedMemory* obj = new NODE_SharedMemory(shmid, semid, size, is_create);
            if(obj->shm_id < 0 || obj->sem_id < 0) {
                Nan::ThrowError("SharedMemory: shmget()/semget() failed.");
            }
            obj->Wrap(info.This());
            info.GetReturnValue().Set(info.This());
        } else {
            Nan::ThrowError("SharedMemory: invalid arguments.");
        }
    } else {
        // Invoked as plain function `MyObject(...)`, turn into construct call.
        const int argc = 3;
        Local<Value> argv[argc] = { info[0], info[1], info[2] };
        info.GetReturnValue().Set(Nan::New(constructor)->NewInstance(argc, argv));
    }
}

NAN_METHOD(NODE_SharedMemory::NODE_size) {
    NODE_SharedMemory* obj = node::ObjectWrap::Unwrap<NODE_SharedMemory>(info.This());
    info.GetReturnValue().Set(Nan::New<Integer>(obj->size));
}

NAN_METHOD(NODE_SharedMemory::NODE_shmid) {
    NODE_SharedMemory* obj = node::ObjectWrap::Unwrap<NODE_SharedMemory>(info.This());
    info.GetReturnValue().Set(Nan::New<Integer>(obj->shm_id));
}

NAN_METHOD(NODE_SharedMemory::NODE_semid) {
    NODE_SharedMemory* obj = node::ObjectWrap::Unwrap<NODE_SharedMemory>(info.This());
    info.GetReturnValue().Set(Nan::New<Integer>(obj->sem_id));
}

void do_nothing_free_callback(char* data, void* hint) { }

NAN_METHOD(NODE_SharedMemory::NODE_buffer) {
    NODE_SharedMemory* obj = node::ObjectWrap::Unwrap<NODE_SharedMemory>(info.This());
    if(obj->shm_data) {
        int start = info[0]->IsUndefined() ? 0 : info[0]->IntegerValue();
        int length = info[1]->IsUndefined() ? obj->size : info[1]->IntegerValue();
        info.GetReturnValue().Set(Nan::NewBuffer((char*)obj->shm_data + start, length, do_nothing_free_callback, NULL).ToLocalChecked());
    } else {
        Nan::ThrowError("buffer: shared memory deleted or not opened.");
    }
}

NAN_METHOD(NODE_SharedMemory::NODE_delete) {
    NODE_SharedMemory* obj = node::ObjectWrap::Unwrap<NODE_SharedMemory>(info.This());
    if(obj->sem_id >= 0) semctl(obj->sem_id, 0, IPC_RMID, 0);
    if(obj->shm_id >= 0) shmctl(obj->shm_id, IPC_RMID, 0);
    obj->shm_id = -1;
    obj->sem_id = -1;
    obj->shm_data = NULL;
    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(NODE_SharedMemory::NODE_close) {
    NODE_SharedMemory* obj = node::ObjectWrap::Unwrap<NODE_SharedMemory>(info.This());
    shmdt(obj->shm_data);
    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(NODE_SharedMemory::NODE_writeLock) {
    NODE_SharedMemory* obj = node::ObjectWrap::Unwrap<NODE_SharedMemory>(info.This());
    sembuf operations[2];
    operations[0].sem_num = 1;  // wait for reads to be zero.
    operations[0].sem_op = 0;
    operations[0].sem_flg = 0;
    operations[1].sem_num = 0;  // increment writes.
    operations[1].sem_op = 1;
    operations[1].sem_flg = 0;
    if(semop(obj->sem_id, operations, 2) == 0) {
        info.GetReturnValue().Set(info.This());
    } else {
        Nan::ThrowError("writeLock: semop() failed.");
    }
}

NAN_METHOD(NODE_SharedMemory::NODE_writeUnlock) {
    NODE_SharedMemory* obj = node::ObjectWrap::Unwrap<NODE_SharedMemory>(info.This());
    sembuf operations[1];
    operations[0].sem_num = 0;  // decrement writes.
    operations[0].sem_op = -1;
    operations[0].sem_flg = 0;
    if(semop(obj->sem_id, operations, 1) == 0) {
        info.GetReturnValue().Set(info.This());
    } else {
        Nan::ThrowError("writeUnlock: semop() failed.");
    }
}

NAN_METHOD(NODE_SharedMemory::NODE_readLock) {
    NODE_SharedMemory* obj = node::ObjectWrap::Unwrap<NODE_SharedMemory>(info.This());
    sembuf operations[2];
    operations[0].sem_num = 0;  // wait for writes to be zero.
    operations[0].sem_op = 0;
    operations[0].sem_flg = 0;
    operations[1].sem_num = 1;  // increment reads.
    operations[1].sem_op = 1;
    operations[1].sem_flg = 0;
    if(semop(obj->sem_id, operations, 2) == 0) {
        info.GetReturnValue().Set(info.This());
    } else {
        Nan::ThrowError("readUnlock: semop() failed.");
    }
}

NAN_METHOD(NODE_SharedMemory::NODE_readUnlock) {
    NODE_SharedMemory* obj = node::ObjectWrap::Unwrap<NODE_SharedMemory>(info.This());
    sembuf operations[1];
    operations[0].sem_num = 1;  // decrement reads.
    operations[0].sem_op = -1;
    operations[0].sem_flg = 0;
    if(semop(obj->sem_id, operations, 1) == 0) {
        info.GetReturnValue().Set(info.This());
    } else {
        Nan::ThrowError("readUnlock: semop() failed.");
    }
}

Nan::Persistent<v8::Function> NODE_SharedMemory::constructor;
