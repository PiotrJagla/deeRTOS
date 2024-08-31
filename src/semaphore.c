#include <semaphore.h>


OS_semaphore_handle OS_create_semaphore(int32_t init_val) {
  OS_semaphore_handle sh = {
    .val = init_val,
  };
  return sh;
}

void OS_sem_wait(OS_semaphore_handle* sh) {
  while(!__sync_val_compare_and_swap(&sh->val, 0, 0));
  __sync_fetch_and_sub(&sh->val, 1);
}
void OS_sem_signal(OS_semaphore_handle* sh) {
  __sync_fetch_and_add(&sh->val, 1);
}
