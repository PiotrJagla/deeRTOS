#include <semaphore.h>
#include <deeRTOS.h>


extern bool sync_val_compare_and_swap(int8_t * ptr, int8_t old_val, int8_t new_val);
extern void sync_fetch_and_sub(int8_t* ptr, int8_t val);
extern void sync_fetch_and_add(int8_t* ptr, int8_t val);

OS_semaphore_handle OS_create_semaphore(int32_t init_val) {
  OS_semaphore_handle sh = {
    .val = init_val,
  };
  return sh;
}

void OS_sem_wait(OS_semaphore_handle* sh) {

  while(!sync_val_compare_and_swap(&sh->val, 0, 0)) {
    OS_delay(1);
  }
  sync_fetch_and_sub(&sh->val, 1);
}
void OS_sem_signal(OS_semaphore_handle* sh) {
  sync_fetch_and_add(&sh->val, 1);
}
