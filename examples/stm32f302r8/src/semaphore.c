#include <semaphore.h>
#include <deeRTOS.h>


extern uint16_t sync_val_compare_and_swap(uint16_t * ptr, uint16_t old_val, uint16_t new_val);
extern void sync_fetch_and_sub(uint16_t* ptr, uint16_t val);
extern void sync_fetch_and_add(uint16_t* ptr, uint16_t val);

OS_semaphore_handle OS_create_semaphore(uint16_t init_val) {
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
