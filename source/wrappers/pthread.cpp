#include "pthread.h"

#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "../util.h"

namespace pthread_wrapper {

struct ThreadStart
{
    void* (*entry)(void*);
    void* arg;
};

static void* entry(void* p) {
    ThreadStart ts = *(ThreadStart*)p;
    free(p);
    tls_setup_guard();
    return ts.entry(ts.arg);
}

int create(pthread_t* thread, const void* unused, void* entry, void* arg) {
    (void)unused;
    ThreadStart* ts = (ThreadStart*)malloc(sizeof(ThreadStart));
    if (!ts)
        return -1;
    ts->entry = (void* (*)(void*))entry;
    ts->arg   = arg;
    return pthread_create(thread, NULL, pthread_wrapper::entry, ts);
}

namespace mutex {
    int init(pthread_mutex_t** uid, const int* mutexattr) {
        if (!uid)
            return -1;
        pthread_mutex_t* m = (pthread_mutex_t*)calloc(1, sizeof(pthread_mutex_t));
        if (!m)
            return -1;

        const int recursive = (mutexattr && *mutexattr == 1);
        int ret;
        if (recursive) {
            pthread_mutexattr_t attr;
            pthread_mutexattr_init(&attr);
            pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
            ret = pthread_mutex_init(m, &attr);
            pthread_mutexattr_destroy(&attr);
        } else {
            ret = pthread_mutex_init(m, NULL);
        }
        if (ret != 0) {
            free(m);
            return -1;
        }
        *uid = m;
        return 0;
    }

    int destroy(pthread_mutex_t** uid) {
        if (uid && *uid && (uintptr_t)*uid > 0x8000) {
            pthread_mutex_destroy(*uid);
            free(*uid);
            *uid = NULL;
        }
        return 0;
    }

    int lock(pthread_mutex_t** uid) {
        int ret = 0;
        if (!*uid)
            ret = init(uid, NULL);
        else if ((uintptr_t)*uid == 0x4000) {
            int attr = 1;
            ret      = init(uid, &attr);
        }
        if (ret < 0)
            return ret;
        return pthread_mutex_lock(*uid);
    }

    int trylock(pthread_mutex_t** uid) {
        int ret = 0;
        if (!*uid)
            ret = init(uid, NULL);
        else if ((uintptr_t)*uid == 0x4000) {
            int attr = 1;
            ret      = init(uid, &attr);
        }
        if (ret < 0)
            return ret;
        return pthread_mutex_trylock(*uid);
    }

    int unlock(pthread_mutex_t** uid) {
        int ret = 0;
        if (!*uid)
            ret = init(uid, NULL);
        else if ((uintptr_t)*uid == 0x4000) {
            int attr = 1;
            ret      = init(uid, &attr);
        }
        if (ret < 0)
            return ret;
        return pthread_mutex_unlock(*uid);
    }
}

namespace mutexattr {
    int init(int* attr) {
        if (!attr)
            return -1;
        *attr = 0;
        return 0;
    }

    int settype(int* attr, int type) {
        if (!attr)
            return -1;
        *attr = type;
        return 0;
    }

    int destroy(int* attr) {
        (void)attr;
        return 0;
    }
}

namespace cond {
    int init(pthread_cond_t** cnd, const int* condattr) {
        (void)condattr;
        if (!cnd)
            return -1;
        pthread_cond_t* c = (pthread_cond_t*)calloc(1, sizeof(pthread_cond_t));
        if (!c)
            return -1;
        *c = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
        if (pthread_cond_init(c, NULL) < 0) {
            free(c);
            return -1;
        }
        *cnd = c;
        return 0;
    }

    int destroy(pthread_cond_t** cnd) {
        if (cnd && *cnd) {
            pthread_cond_destroy(*cnd);
            free(*cnd);
            *cnd = NULL;
        }
        return 0;
    }

    int broadcast(pthread_cond_t** cnd) {
        if (!*cnd && init(cnd, NULL) < 0)
            return -1;
        return pthread_cond_broadcast(*cnd);
    }

    int signal(pthread_cond_t** cnd) {
        if (!*cnd && init(cnd, NULL) < 0)
            return -1;
        return pthread_cond_signal(*cnd);
    }

    int wait(pthread_cond_t** cnd, pthread_mutex_t** mtx) {
        if (!*cnd && init(cnd, NULL) < 0)
            return -1;
        if (!*mtx)
            mutex::init(mtx, NULL);
        else if ((uintptr_t)*mtx == 0x4000) {
            int attr = 1;
            mutex::init(mtx, &attr);
        }
        return pthread_cond_wait(*cnd, *mtx);
    }

    int timedwait(pthread_cond_t** cnd, pthread_mutex_t** mtx, const struct timespec* t) {
        if (!*cnd && init(cnd, NULL) < 0)
            return -1;
        if (!*mtx)
            mutex::init(mtx, NULL);
        else if ((uintptr_t)*mtx == 0x4000) {
            int attr = 1;
            mutex::init(mtx, &attr);
        }
        return pthread_cond_timedwait(*cnd, *mtx, t);
    }
}

int once(volatile int* once_control, void (*init_routine)(void)) {
    if (!once_control || !init_routine)
        return -1;
    if (__sync_lock_test_and_set(once_control, 1) == 0)
        (*init_routine)();
    return 0;
}

}