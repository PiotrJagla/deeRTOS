#include <stm32f302x8.h>
#include <stm32f3xx.h>
#include <usart.h>
#include <cmsis_util.h>
#include <gpio.h>
#include <timers.h>
#include <stdint.h>
#include <deeRTOS.h>
#include <printf.h>

#include <queue.h>

  //GpioSetPinMode(LED3_BASE, LED3_PIN, GPIO_OUTPUT);
// ---------- SNAKE MACROS --------
#define JOYSTICK_X_BASE GPIOC
#define JOYSTICK_X_PIN 0
#define JOYSTICK_Y_BASE GPIOC
#define JOYSTICK_Y_PIN 1
#define POTENTIOMETR_BASE GPIOA
#define POTENTIOMETR_PIN 0
#define BUZZ_BASE GPIOA
#define BUZZ_PIN 1
#define LED_R_BASE GPIOC
#define LED_R_PIN 10
#define LED_G_BASE GPIOC
#define LED_G_PIN 11
#define LED_W_BASE GPIOC
#define LED_W_PIN 12
#define C8_BASE GPIOC
#define C8_PIN 4
#define C7_BASE GPIOA
#define C7_PIN 5
#define R2_BASE GPIOA
#define R2_PIN 6
#define C1_BASE GPIOA
#define C1_PIN 7
#define R4_BASE GPIOB
#define R4_PIN 1
#define C6_BASE GPIOB
#define C6_PIN 2
#define C4_BASE GPIOB
#define C4_PIN 11
#define R1_BASE GPIOB
#define R1_PIN 12
#define R5_BASE GPIOA
#define R5_PIN 10
#define R7_BASE GPIOB
#define R7_PIN 5
#define C2_BASE GPIOB
#define C2_PIN 10
#define C3_BASE GPIOA
#define C3_PIN 8
#define R8_BASE GPIOA
#define R8_PIN 9
#define C5_BASE GPIOC
#define C5_PIN 7
#define R6_BASE GPIOB
#define R6_PIN 6
#define R3_BASE GPIOB
#define R3_PIN 15
#define SEG_A_BASE GPIOB
#define SEG_A_PIN 8
#define SEG_B_BASE GPIOB
#define SEG_B_PIN 9
#define SEG_C_BASE GPIOB
#define SEG_C_PIN 13
#define SEG_D_BASE GPIOB
#define SEG_D_PIN 14
#define SEG_E_BASE GPIOC
#define SEG_E_PIN 8
#define SEG_F_BASE GPIOC
#define SEG_F_PIN 6
#define SEG_G_BASE GPIOC
#define SEG_G_PIN 9
#define SEG_1_BASE GPIOC
#define SEG_1_PIN 5
#define SEG_2_BASE GPIOA
#define SEG_2_PIN 12
#define SEG_3_BASE GPIOA
#define SEG_3_PIN 11
// ---------- END SNAKE MACROS --------

void CustomGpioInit();

bool matrix[][8] = {
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0}
};

#define STACK_SIZE_TASK1 128
uint32_t stack_task1[STACK_SIZE_TASK1];
OSThread tcb_task1;
void task1() {
  uint8_t something = 10;
  while(1) {
    //GpioTogglePin(LED1_BASE, LED1_PIN);
    OS_delay(1000);
  }
}

#define STACK_SIZE_TASK2 128
uint32_t stack_task2[STACK_SIZE_TASK2];
OSThread tcb_task2;
void task2() {
  while(1) {
    //GpioTogglePin(LED2_BASE, LED2_PIN);
    OS_delay(2000);

  }
}

void main(void) {
  system_init();
  usart_init(USART2);
  GpioInit();
  CustomGpioInit();
  OS_init();


  OS_create_thread(&tcb_task1, 1, &task1, stack_task1, sizeof(stack_task1));
  OS_create_thread(&tcb_task2, 2, &task2, stack_task2, sizeof(stack_task2));

  __enable_irq();

  OS_start();

  while(1) {
    delay_ms(1000);
  }
}


void CustomGpioInit() {
  GpioSetPinMode(SEG_3_BASE,SEG_3_PIN, GPIO_OUTPUT);
  GpioSetPinMode(SEG_2_BASE,SEG_2_PIN, GPIO_OUTPUT);
  GpioSetPinMode(SEG_1_BASE,SEG_1_PIN, GPIO_OUTPUT);
  GpioSetPinMode(SEG_G_BASE,SEG_G_PIN, GPIO_OUTPUT);
  GpioSetPinMode(SEG_F_BASE,SEG_F_PIN, GPIO_OUTPUT);
  GpioSetPinMode(SEG_E_BASE,SEG_E_PIN, GPIO_OUTPUT);
  GpioSetPinMode(SEG_D_BASE,SEG_D_PIN, GPIO_OUTPUT);
  GpioSetPinMode(SEG_C_BASE,SEG_C_PIN, GPIO_OUTPUT);
  GpioSetPinMode(SEG_B_BASE,SEG_B_PIN, GPIO_OUTPUT);
  GpioSetPinMode(SEG_A_BASE,SEG_A_PIN, GPIO_OUTPUT);
  GpioSetPinMode(R3_BASE,R3_PIN, GPIO_OUTPUT);
  GpioSetPinMode(R6_BASE,R6_PIN, GPIO_OUTPUT);
  GpioSetPinMode(C5_BASE,C5_PIN, GPIO_OUTPUT);
  GpioSetPinMode(R8_BASE,R8_PIN, GPIO_OUTPUT);
  GpioSetPinMode(C3_BASE,C3_PIN, GPIO_OUTPUT);
  GpioSetPinMode(C2_BASE,C2_PIN, GPIO_OUTPUT);
  GpioSetPinMode(R7_BASE,R7_PIN, GPIO_OUTPUT);
  GpioSetPinMode(R5_BASE,R5_PIN, GPIO_OUTPUT);
  GpioSetPinMode(R1_BASE,R1_PIN, GPIO_OUTPUT);
  GpioSetPinMode(C4_BASE,C4_PIN, GPIO_OUTPUT);
  GpioSetPinMode(C6_BASE,C6_PIN, GPIO_OUTPUT);
  GpioSetPinMode(R4_BASE,R4_PIN, GPIO_OUTPUT);
  GpioSetPinMode(C1_BASE,C1_PIN, GPIO_OUTPUT);
  GpioSetPinMode(R2_BASE,R2_PIN, GPIO_OUTPUT);
  GpioSetPinMode(C7_BASE,C7_PIN, GPIO_OUTPUT);
  GpioSetPinMode(C8_BASE,C8_PIN, GPIO_OUTPUT);
  GpioSetPinMode(LED_W_BASE,LED_W_PIN, GPIO_OUTPUT);
  GpioSetPinMode(LED_G_BASE,LED_G_PIN, GPIO_OUTPUT);
  GpioSetPinMode(LED_R_BASE,LED_R_PIN, GPIO_OUTPUT);
  GpioSetPinMode(BUZZ_BASE,BUZZ_PIN, GPIO_OUTPUT);
  GpioSetPinMode(POTENTIOMETR_BASE,POTENTIOMETR_PIN, GPIO_OUTPUT);
  GpioSetPinMode(JOYSTICK_Y_BASE,JOYSTICK_Y_PIN, GPIO_OUTPUT);
  GpioSetPinMode(JOYSTICK_X_BASE,JOYSTICK_X_PIN, GPIO_OUTPUT);

  GpioLD2EnableOutput();
}
