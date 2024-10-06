#include <mutex.h>
#include <deeRTOS.h>



OS_mutex_handle OS_create_mutex() {
  OS_mutex_handle mh = {
    .is_locked = false,
  };
  return mh;
}


void OS_lock(OS_mutex_handle* mh) {
  extern bool sync_bool_compare_and_swap(bool * ptr, bool old_val, bool new_val);

  while(!sync_bool_compare_and_swap(&mh->is_locked, false, true)) {
    OS_delay(1);
  }
}
void OS_unlock(OS_mutex_handle* mh) {
  mh->is_locked = false;
}


