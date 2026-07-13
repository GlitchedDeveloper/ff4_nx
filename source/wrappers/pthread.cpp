#include "pthread.h"

#include <stdlib.h>

#include "../thread_guard.h"
#include "../util.h"

namespace pthread_wrapper {
typedef struct
{
    void* (*func)(void*);
    void* arg;
} PthreadWrapperData;

static void* pthread_entry_wrapper(void* data) {
    PthreadWrapperData* pd = (PthreadWrapperData*)data;
    void* (*func)(void*)   = pd->func;
    void* arg              = pd->arg;
    free(pd);

    uint8_t* tls = reinterpret_cast<uint8_t*>(calloc(1, 0x100));
    armSetTlsRw(tls);

    return thread_guard_run_ptr(func, arg);
}

// pthread_t is an unsigned int, so it should be fine
// TODO: probably shouldn't assume default attributes
int create(pthread_t* thread, const void* unused, void* entry, void* arg) {
    PthreadWrapperData* pd = reinterpret_cast<PthreadWrapperData*>(malloc(sizeof(PthreadWrapperData)));
    pd->func               = (void* (*)(void*))entry;
    pd->arg                = arg;
    return pthread_create(thread, NULL, pthread_entry_wrapper, pd);
}

static int pthread_mutex_attr_is_recursive(const int* mutexattr) {
    int attr_val  = mutexattr ? *mutexattr : 0;
    int recursive = (attr_val == 1 || attr_val == 0x4000);
#ifdef PTHREAD_MUTEX_RECURSIVE
    if (attr_val == PTHREAD_MUTEX_RECURSIVE)
        recursive = 1;
#endif
    return recursive;
}

namespace mutex {
    static pthread_mutex_t* create(const int* mutexattr) {
        pthread_mutex_t* m = reinterpret_cast<pthread_mutex_t*>(calloc(1, sizeof(pthread_mutex_t)));
        if (!m)
            return NULL;

        int recursive = pthread_mutex_attr_is_recursive(mutexattr);

        pthread_mutexattr_t host_attr;
        pthread_mutexattr_init(&host_attr);
        if (recursive)
            pthread_mutexattr_settype(&host_attr, PTHREAD_MUTEX_RECURSIVE);

        int ret = pthread_mutex_init(m, &host_attr);
        pthread_mutexattr_destroy(&host_attr);
        if (ret < 0) {
            free(m);
            return NULL;
        }

        return m;
    }

    static pthread_mutex_t* ensure(pthread_mutex_t** uid, const int* mutexattr) {
        int recursive = pthread_mutex_attr_is_recursive(mutexattr);

        if (!uid)
            return NULL;

        while (1) {
            pthread_mutex_t* cur = __atomic_load_n(uid, __ATOMIC_ACQUIRE);
            if (cur && (uintptr_t)cur != 0x4000)
                return cur;

            int init_attr            = (recursive || (uintptr_t)cur == 0x4000) ? 1 : 0;
            pthread_mutex_t* created = create(init_attr ? &init_attr : NULL);
            if (!created)
                return NULL;

            pthread_mutex_t* expected = cur;
            if (__atomic_compare_exchange_n(uid, &expected, created, 0,
                    __ATOMIC_RELEASE, __ATOMIC_ACQUIRE))
                return created;

            pthread_mutex_destroy(created);
            free(created);

            if (expected && (uintptr_t)expected != 0x4000)
                return expected;
        }
    }

    int init(pthread_mutex_t** uid, const int* mutexattr) {
        if (!ensure(uid, mutexattr))
            return -1;
        return 0;
    }

    int destroy(pthread_mutex_t** uid) {
        if (uid) {
            pthread_mutex_t* m = __atomic_exchange_n(uid, NULL, __ATOMIC_ACQ_REL);
            if (m && (uintptr_t)m > 0x8000) {
                pthread_mutex_destroy(m);
                free(m);
            }
        }
        return 0;
    }

    int lock(pthread_mutex_t** uid) {
        pthread_mutex_t* m = ensure(uid, NULL);
        if (!m)
            return -1;
        return pthread_mutex_lock(m);
    }

    int unlock(pthread_mutex_t** uid) {
        pthread_mutex_t* m = ensure(uid, NULL);
        if (!m)
            return -1;
        return pthread_mutex_unlock(m);
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

        // Android bionic encodes recursive mutex type as 1.
        if (type == 1)
            *attr = 1;
#ifdef PTHREAD_MUTEX_RECURSIVE
        else if (type == PTHREAD_MUTEX_RECURSIVE)
            *attr = 1;
#endif
        else
            *attr = 0;

        return 0;
    }

    int destroy(int* attr) {
        (void)attr;
        return 0;
    }
}

namespace cond {
    static pthread_cond_t* create(void) {
        pthread_cond_t* c = reinterpret_cast<pthread_cond_t*>(calloc(1, sizeof(pthread_cond_t)));
        if (!c)
            return NULL;

        *c = PTHREAD_COND_INITIALIZER;

        int ret = pthread_cond_init(c, NULL);
        if (ret < 0) {
            free(c);
            return NULL;
        }

        return c;
    }

    static pthread_cond_t* ensure(pthread_cond_t** cnd) {
        if (!cnd)
            return NULL;

        while (1) {
            pthread_cond_t* cur = __atomic_load_n(cnd, __ATOMIC_ACQUIRE);
            if (cur)
                return cur;

            pthread_cond_t* created = create();
            if (!created)
                return NULL;

            pthread_cond_t* expected = NULL;
            if (__atomic_compare_exchange_n(cnd, &expected, created, 0,
                    __ATOMIC_RELEASE, __ATOMIC_ACQUIRE))
                return created;

            pthread_cond_destroy(created);
            free(created);

            if (expected)
                return expected;
        }
    }

    int init(pthread_cond_t** cnd, const int* condattr) {
        (void)condattr;
        if (!ensure(cnd))
            return -1;
        return 0;
    }

    int broadcast(pthread_cond_t** cnd) {
        pthread_cond_t* c = ensure(cnd);
        if (!c)
            return -1;
        return pthread_cond_broadcast(c);
    }

    int destroy(pthread_cond_t** cnd) {
        if (cnd) {
            pthread_cond_t* c = __atomic_exchange_n(cnd, NULL, __ATOMIC_ACQ_REL);
            if (c) {
                pthread_cond_destroy(c);
                free(c);
            }
        }
        return 0;
    }

    static volatile int g_cond_wait_count = 0;
    int wait(pthread_cond_t** cnd, pthread_mutex_t** mtx) {
        pthread_cond_t* c  = ensure(cnd);
        pthread_mutex_t* m = mutex::ensure(mtx, NULL);
        if (!c || !m)
            return -1;
        int n = __atomic_add_fetch(&g_cond_wait_count, 1, __ATOMIC_RELAXED);
        if (n <= 20 || (n % 200 == 0))
            debugPrintf("pthread_cond_wait: cnd=%p mtx=%p (call #%d)\n", (void*)c, (void*)m, n);
        return pthread_cond_wait(c, m);
    }
}
}