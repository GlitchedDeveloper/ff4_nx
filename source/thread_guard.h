#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void* thread_guard_run_ptr(void* (*func)(void*), void* arg);

#ifdef __cplusplus
}
#endif