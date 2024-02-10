#include <cmsis_util.h>
#include <stm32f3xx.h>


static uint32_t ticks;

void systick_handler() {
  ticks++;
}


//__attribute__((naked)) void systick_handler(){
//	/// STEP 1 - SAVE THE CURRENT TASK CONTEXT
//
//	/// At this point the processor has already pushed PSR, PC, LR, R12, R3, R2, R1 and R0
//	/// onto the stack. We need to push the rest(i.e R4, R5, R6, R7, R8, R9, R10 & R11) to save the
//	/// context of the current task.
//	/// Disable interrupts
//  __asm__("CPSID   I");
//  /// Push registers R4 to R7
//  __asm__("PUSH    {R4-R7}");
//  /// Push registers R8-R11
//  __asm__("MOV     R4, R8");
//  __asm__("MOV     R5, R9");
//  __asm__("MOV     R6, R10");
//  __asm__("MOV     R7, R11");
//  __asm__("PUSH    {R4-R7}");
//  /// Load R0 with the address of pCurntTcb
//  __asm__("LDR     R0, =pCurntTcb");
//  /// Load R1 with the content of pCurntTcb(i.e post this, R1 will contain the address of current TCB).
//  __asm__("LDR     R1, [R0]");
//  /// Move the SP value to R4
//  __asm__("MOV     R4, SP");
//  /// Store the value of the stack pointer(copied in R4) to the current tasks "stackPt" element in its TCB.
//  /// This marks an end to saving the context of the current task.
//  __asm__("STR     R4, [R1]");
//
//
//  /// STEP 2: LOAD THE NEW TASK CONTEXT FROM ITS STACK TO THE CPU REGISTERS, UPDATE pCurntTcb.
//
//  /// Load the address of the next task TCB onto the R1.
//  __asm__("LDR     R1, [R1,#4]");
//  /// Load the contents of the next tasks stack pointer to pCurntTcb, equivalent to pointing pCurntTcb to
//  /// the newer tasks TCB. Remember R1 contains the address of pCurntTcb.
//  __asm__("STR     R1, [R0]");
//  /// Load the newer tasks TCB to the SP using R4.
//  __asm__("LDR     R4, [R1]");
//  __asm__("MOV     SP, R4");
//  /// Pop registers R8-R11
//  __asm__("POP     {R4-R7}");
//  __asm__("MOV     R8, R4");
//  __asm__("MOV     R9, R5");
//  __asm__("MOV     R10, R6");
//  __asm__("MOV     R11, R7");
//  /// Pop registers R4-R7
//  __asm__("POP     {R4-R7}");
//  __asm__("CPSIE   I ");
//  __asm__("BX      LR");
//}

void delay_ms(uint32_t miliseconds){
  uint32_t start = ticks;
  uint32_t end = start + miliseconds;
  if(end < start) {
    while(ticks > start);
  }

  while(ticks < end);
}
