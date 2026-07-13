#pragma once

#include <pthread.h>

namespace pthread_wrapper {
namespace cond {
    int broadcast(pthread_cond_t** cnd);
    int destroy(pthread_cond_t** cnd);
    int init(pthread_cond_t** cnd, const int* condattr);
    int wait(pthread_cond_t** cnd, pthread_mutex_t** mtx);
}
namespace mutexattr {
    int init(int* attr);
    int settype(int* attr, int type);
    int destroy(int* attr);
}
namespace mutex {
    int destroy(pthread_mutex_t** uid);
    int init(pthread_mutex_t** uid, const int* mutexattr);
    int lock(pthread_mutex_t** uid);
    int unlock(pthread_mutex_t** uid);
}
int create(pthread_t* thread, const void* unused, void* entry, void* arg);
}