#include <deeRTOS.h>
#include <stdio.h>
#include <stdint.h>

#define NUM_OF_THREADS 2
#define STACK_SIZE 100

struct tcb{
	int32_t* stackPtr;
	struct tcb* nextTask;
};

struct tcb tasks[NUM_OF_THREADS];
int32_t TCB_STACK[NUM_OF_THREADS][STACK_SIZE];
struct tcb* pCurntTcb;

void portable_delay_cycles(uint32_t n) {
  while(n--) {
    __asm__ volatile("NOP");
  }
}

volatile void Task0() {
  while(1) {
    printf("Task1\n\r");
    portable_delay_cycles(1000);
  }
}

volatile void Task1() {
  while(1) {
    printf("Task2\n\r");
    portable_delay_cycles(2000);
  }
}

void OsInitThreadStack() {
  /// Enter critical section
  /// Disable interrupts
  __asm__("CPSID   I");
  /// Make the TCB linked list circular
  tasks[0].nextTask = &tasks[1];
  tasks[1].nextTask = &tasks[0];
  
  /// Setup stack for task0
  
  /// Setup the stack such that it is holding one task context.
  /// Remember it is a descending stack and a context consists of 16 registers.
  tasks[0].stackPtr = &TCB_STACK[0][STACK_SIZE-16];
  /// Set the 'T' bit in stacked xPSR to '1' to notify processor
  /// on exception return about the thumb state. V6-m and V7-m cores
  /// can only support thumb state hence this should be always set
  /// to '1'.
  TCB_STACK[0][STACK_SIZE-1] = 0x01000000;
  /// Set the stacked PC to point to the task
  TCB_STACK[0][STACK_SIZE-2] = (int32_t)(Task0);
  
  
  /// Setup stack for task1
  
  /// Setup the stack such that it is holding one task context.
  /// Remember it is a descending stack and a context consists of 16 registers.
  tasks[1].stackPtr = &TCB_STACK[1][STACK_SIZE-16];
  /// Set the 'T' bit in stacked xPSR to '1' to notify processor
  /// on exception return about the thumb state. V6-m and V7-m cores
  /// can only support thumb state hence this should be always set
  /// to '1'.
  TCB_STACK[1][STACK_SIZE-1] = 0x01000000;
  /// Set the stacked PC to point to the task
  TCB_STACK[1][STACK_SIZE-2] = (int32_t)(Task1);
  /// Make current tcb pointer point to task0
  pCurntTcb = &tasks[0];
  /// Enable interrupts
  __asm__("CPSIE   I ");
}

__attribute__((naked)) void LaunchScheduler(){
 /// R0 contains the address of currentPt
    __asm__("LDR     R0, =pCurntTcb");
    /// R2 contains the address in currentPt(value of currentPt)
    __asm__("LDR     R2, [R0]");
    /// Load the SP reg with the stacked SP value
    __asm__("LDR     R4, [R2]");
    __asm__("MOV     SP, R4");
    /// Pop registers R8-R11(user saved context)
    __asm__("POP     {R4-R7}");
    __asm__("MOV     R8, R4");
    __asm__("MOV     R9, R5");
    __asm__("MOV     R10, R6");
    __asm__("MOV     R11, R7");
    /// Pop registers R4-R7(user saved context)
    __asm__("POP     {R4-R7}");
    ///  Start poping the stacked exception frame.
    __asm__("POP     {R0-R3}");
    __asm__("POP     {R4}");
    __asm__("MOV     R12, R4");
    /// Skip the saved LR
    __asm__("ADD     SP,SP,#4");
    /// POP the saved PC into LR via R4, We do this to jump into the
    /// first task when we execute the branch instruction to exit this routine.
    __asm__("POP     {R4}");
    __asm__("MOV     LR, R4");
    __asm__("ADD     SP,SP,#4");
    /// Enable interrupts
    __asm__("CPSIE   I ");
    __asm__("BX      LR");
}
