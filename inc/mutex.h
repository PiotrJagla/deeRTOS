#ifndef MUTEX_H
#define MUTEX_H

#include <stdbool.h>

typedef struct {
  volatile bool is_locked;
} OS_mutex_handle;

OS_mutex_handle OS_create_mutex();
void OS_lock(OS_mutex_handle* mh);
void OS_unlock(OS_mutex_handle* mh);

#endif
