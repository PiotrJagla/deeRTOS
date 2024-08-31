#include <mutex.h>


OS_mutex_handle OS_create_mutex() {
  OS_mutex_handle mh = {
    .is_locked = false,
  };
  return mh;
}

void OS_lock(OS_mutex_handle* mh) {
  while(!__sync_bool_compare_and_swap(&mh->is_locked, false, true));
}
void OS_unlock(OS_mutex_handle* mh) {
  mh->is_locked = false;
}


