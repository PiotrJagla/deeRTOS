#include <sys/times.h>
#include <sys/stat.h>
#include "usart.h"
#include <errno.h>
#undef errno
extern int errno;

register char* stack_ptr __asm__("sp");

caddr_t _sbrk(int incr) {
  extern char __bss_end__;
  static char* heap_end;
  char* prev_heap_end;

  if(heap_end == 0) {
    heap_end = &__bss_end__;
  }
  prev_heap_end = heap_end;
  if(heap_end + incr > stack_ptr) {
    while(1) {
      // stack and heap collision
    }
  }

  heap_end += incr;
  return (caddr_t) prev_heap_end;
}

int _close(int file) {
  return -1;
}

int _isatty(int file) {
  return 1;
}

int _lseek(int file, int ptr, int dir) {
  return 0;
}

int _read(int file, char* ptr, int len) {
  return 0;
}

int _write(int file, char* ptr, int len) {
  (void) file;

  for(uint32_t i= 0; i<len ; i++) {
    usart_transmit(USART2, *ptr++);
  }

  return len;
}

int _fstat(int file, struct stat* st) {
  st->st_mode = S_IFCHR;
  return 0;
}

void _exit(int exit_code) {
  while(1) {

  }
}





