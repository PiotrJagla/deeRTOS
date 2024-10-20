#include <stm32f302x8.h>
#include <stm32f3xx.h>
#include <usart.h>
#include <cmsis_util.h>
#include <gpio.h>
#include <timers.h>
#include <stdint.h>
#include <deeRTOS.h>
#include <printf.h>
#include <adc.h>
#include <dma.h>


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
#define SEG_3_BASE GPIOA
#define SEG_3_PIN 12
#define SEG_4_BASE GPIOA
#define SEG_4_PIN 11
#define SWITCH_BASE GPIOC
#define SWITCH_PIN 13
#define BUZZ_SWITCH_BASE GPIOA
#define BUZZ_SWITCH_PIN 4
#define BUZZ_LED_BASE GPIOB
#define BUZZ_LED_PIN 0

#define MATRIX_SIZE 8
// ---------- END SNAKE MACROS --------


void writeC1(bool value) {
  GpioWritePin(C1_BASE, C1_PIN, value);
}
void writeC2(bool value) {
  GpioWritePin(C2_BASE, C2_PIN, value);
}
void writeC3(bool value) {
  GpioWritePin(C3_BASE, C3_PIN, value);
}
void writeC4(bool value) {
  GpioWritePin(C4_BASE, C4_PIN, value);
}
void writeC5(bool value) {
  GpioWritePin(C5_BASE, C5_PIN, value);
}
void writeC6(bool value) {
  GpioWritePin(C6_BASE, C6_PIN, value);
}
void writeC7(bool value) {
  GpioWritePin(C7_BASE, C7_PIN, value);
}
void writeC8(bool value) {
  GpioWritePin(C8_BASE, C8_PIN, value);
}
void writeR1(bool value) {
  GpioWritePin(R1_BASE, R1_PIN, value);
}
void writeR2(bool value) {
  GpioWritePin(R2_BASE, R2_PIN, value);
}
void writeR3(bool value) {
  GpioWritePin(R3_BASE, R3_PIN, value);
}
void writeR4(bool value) {
  GpioWritePin(R4_BASE, R4_PIN, value);
}
void writeR5(bool value) {
  GpioWritePin(R5_BASE, R5_PIN, value);
}
void writeR6(bool value) {
  GpioWritePin(R6_BASE, R6_PIN, value);
}
void writeR7(bool value) {
  GpioWritePin(R7_BASE, R7_PIN, value);
}
void writeR8(bool value) {
  GpioWritePin(R8_BASE, R8_PIN, value);
}

void (*writeRows[])(bool) = {
  writeR1,
  writeR2,
  writeR3,
  writeR4,
  writeR5,
  writeR6,
  writeR7,
  writeR8,
};
void (*writeCols[])(bool) = {
  writeC1,
  writeC2,
  writeC3,
  writeC4,
  writeC5,
  writeC6,
  writeC7,
  writeC8,
};

void writeCol(uint8_t col, bool value) {
  for(int8_t i = 0 ; i < 8 ; ++i) {
    writeCols[i](false);
  }
  writeCols[col](value);
}

void clear_whole_row() {
  for(uint8_t i = 0 ; i < 8 ; ++i) {
    writeRows[i](true);
  }
}
void writeColDiodes(uint8_t col, uint8_t diodes) {
  clear_whole_row();

  writeCol(col, true);
  for(int8_t i = 0 ; i < 8 ; ++i) {
    writeRows[i](true);
  }

  for(uint8_t i = 0 ; i < 8 ; ++i) {
    if(diodes & (1<<i)) {
      writeRows[i](false);
    }
  }
}


void CustomGpioInit();

void writeA(bool value) {
  GpioWritePin(SEG_A_BASE, SEG_A_PIN, value);
}
void writeB(bool value) {
  GpioWritePin(SEG_B_BASE, SEG_B_PIN, value);
}
void writeC(bool value) {
  GpioWritePin(SEG_C_BASE, SEG_C_PIN, value);
}
void writeD(bool value) {
  GpioWritePin(SEG_D_BASE, SEG_D_PIN, value);
}
void writeE(bool value) {
  GpioWritePin(SEG_E_BASE, SEG_E_PIN, value);
}
void writeF(bool value) {
  GpioWritePin(SEG_F_BASE, SEG_F_PIN, value);
}
void writeG(bool value) {
  GpioWritePin(SEG_G_BASE, SEG_G_PIN, value);
}
void write_SEG1(bool value) {
  GpioWritePin(SEG_1_BASE, SEG_1_PIN, value);
}
void write_SEG2(bool value) {
  // NONE
}
void write_SEG3(bool value) {
  GpioWritePin(SEG_3_BASE, SEG_3_PIN, value);
}
void write_SEG4(bool value) {
  GpioWritePin(SEG_4_BASE, SEG_4_PIN, value);
}

void (*write_seg_element[])(bool) = {
  writeA,
  writeB,
  writeC,
  writeD,
  writeE,
  writeF,
  writeG,
};

void (*write_seg[])(bool) = {
  write_SEG1,
  write_SEG2,
  write_SEG3,
  write_SEG4,
};

uint8_t digits[10] = {
  0b0111111,
  0b0000110,
  0b1011011,
  0b1001111,
  0b1100110,
  0b1101101,
  0b1111101,
  0b0000111,
  0b1111111,
  0b1101111
  // G, F, E, D, D, B , A
};

void clear_all_segments() {
  for(uint8_t i = 0 ; i < 7 ; ++i) {
    write_seg_element[i](false);
  }
}
void light_digit_on_display(uint8_t digit, uint8_t display) {
  clear_all_segments();

  for(uint8_t i = 0 ; i < 4 ; ++i) {
    if((display - 1) == i) {
      write_seg[i](false);
    } else {
      write_seg[i](true);
    }
  }

  for(uint8_t i = 0 ; i < 7 ; ++i) {
    if(digits[digit] & (1<<i)) {
      write_seg_element[i](true);
    } else {
      write_seg_element[i](false);
    }
  }
}

//ADC
#define ADC_BUF_SIZE 3
#define ADC_X 0
#define ADC_Y 1
#define ADC_POTENTIOMETER 2
uint8_t adc_buf[ADC_BUF_SIZE] = {};

#define SNAKE_MAX_SIZE 30

// ----------------- SNAKE GAME ------------
uint32_t tick = 0;
struct player{
  int8_t xPositions[30];
  int8_t yPositions[30];
  int8_t xDir;
  int8_t yDir;
  int8_t points;
};
uint8_t gameMatrix[8] = {0,32,0,0,0,0,0,0};
struct player snake = {
  {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
1,0,1};
uint8_t isInGame = 0;
uint8_t dotCollected = 0;
bool dotColledtedEvent = false;

int isDotOnSnake(uint8_t col, uint8_t row) {
  for(int8_t i = 0 ; i < SNAKE_MAX_SIZE ; ++i) {
    if(col == snake.yPositions[i] && row == snake.xPositions[i]) {
      return 1;
    }
  }
  return 0;
}
void pushRandomDot() {
  uint8_t randVal = tick%64;
  uint8_t randCol = randVal/MATRIX_SIZE;
  uint8_t randRow = randVal%MATRIX_SIZE;
  while(isDotOnSnake(randCol, randRow)) {
    randVal+= 3;
    randVal = randVal%64;
    randCol = randVal/MATRIX_SIZE;
    randRow = randVal%MATRIX_SIZE;
  }
  gameMatrix[randCol] |= (1<<randRow);
}

void initSnakeGame(){
  //reset game matrix
  for(uint8_t i = 0 ; i < MATRIX_SIZE ; ++i) {
    gameMatrix[i] = 0;
  }

  //reset snake positions
  for(uint8_t i = 1; i < SNAKE_MAX_SIZE; ++i) {
    snake.xPositions[i] = 0;
    snake.yPositions[i] = 0;
  }
  snake.xPositions[0] = 2;
  snake.yPositions[0] = 4;
  snake.xDir = 1;
  snake.yDir = 0;
  snake.points = 1;


  isInGame = 1;
  pushRandomDot();
  gameMatrix[snake.yPositions[0]] |= (1<<snake.xPositions[0]);
}

int8_t isOutOfBounds() {
  if(snake.xPositions[0] >= MATRIX_SIZE) {
    return 1;
  } else if(snake.xPositions[0] < 0) {
    return 1;
  }
  if(snake.yPositions[0] >= MATRIX_SIZE) {
    return 1;
  } else if(snake.yPositions[0] < 0) {
    return 1;
  }
  return 0;
}
int8_t didLose() {
  if(isOutOfBounds()) {
    return 1;
  }
  for(int8_t i = 1; i <snake.points ; ++i) {
    if(snake.xPositions[0] == snake.xPositions[i] && snake.yPositions[0] == snake.yPositions[i]) {
      return 1;
    }
  }
  return 0;

}

void updateSnakeGame(){

  if(isInGame == 1) {
    if(dotCollected == 0) {
      gameMatrix[snake.yPositions[snake.points - 1]] &= ~(1<<snake.xPositions[snake.points-1]);
    } else {
      snake.points++;
    }
    for(int8_t i = snake.points - 2 ; i >= 0; --i) {
      snake.xPositions[i+1] = snake.xPositions[i];
      snake.yPositions[i+1] = snake.yPositions[i];
    }
    snake.xPositions[0] += snake.xDir;
    snake.yPositions[0] += snake.yDir;


    //Is out of bounds
    if(didLose() == 1) {
      GpioWritePin(LED_R_BASE, LED_R_PIN, true);
      GpioWritePin(LED_G_BASE, LED_G_PIN, false);
      GpioWritePin(LED_W_BASE, LED_W_PIN, false);
      isInGame = 0;
    }


    //dot collected
    if(gameMatrix[snake.yPositions[0]] & (1<<snake.xPositions[0])) {
      pushRandomDot();

      dotCollected = 1;
      dotColledtedEvent = true;
    } else {
      dotCollected = 0;
    }


    //Draw new position
    gameMatrix[snake.yPositions[0]] |= (1<<snake.xPositions[0]);
  }
}

void updateDirection(uint8_t yJoystickVal, uint8_t xJoystickVal) {
  if(snake.yDir == 0) {
    if(xJoystickVal < 10) {
      snake.yDir = 1;
      snake.xDir = 0;
    }
    else if(xJoystickVal > 240) {
      snake.yDir = -1;
      snake.xDir = 0;
    }
  }

  if(snake.xDir == 0) {
    if(yJoystickVal < 10) {
      snake.xDir = -1;
      snake.yDir = 0;
    }
    else if(yJoystickVal > 240) {
      snake.xDir = 1;
      snake.yDir = 0;
    }
  }
}

// ----------------- END SNAKE GAME --------------

#define STACK_SIZE_TASK1 128
uint32_t stack_task1[STACK_SIZE_TASK1];
OSThread tcb_task1;
void task1() {
  while(1) {
    for(uint8_t i = 0 ; i < MATRIX_SIZE ; ++i) {
      tick++;
      if(gameMatrix[i] == 0) continue;

      writeColDiodes(i, gameMatrix[i]);
      OS_delay(4);
    }
  }
}

#define STACK_SIZE_TASK2 128
uint32_t stack_task2[STACK_SIZE_TASK2];
OSThread tcb_task2;
void task2() {
  while(1) {
    light_digit_on_display(adc_buf[ADC_POTENTIOMETER]/26, 1);
    OS_delay(4);
    light_digit_on_display(snake.points/10, 3);
    OS_delay(4);
    light_digit_on_display(snake.points%10, 4);
    OS_delay(4);
  }
}

#define STACK_SIZE_TASK3 128
uint32_t stack_task3[STACK_SIZE_TASK3];
OSThread tcb_task3;
void task3() {

  uint8_t switchLock = 1;
  initSnakeGame();
  GpioWritePin(LED_R_BASE, LED_R_PIN, false);
  GpioWritePin(LED_G_BASE, LED_G_PIN, true);
  GpioWritePin(LED_W_BASE, LED_W_PIN, false);

  while(1) {
    if(switchLock == 1 && !GpioReadPin(SWITCH_BASE, SWITCH_PIN)) {
      switchLock = 0;
      initSnakeGame();
      GpioWritePin(LED_R_BASE, LED_R_PIN, false);
      GpioWritePin(LED_G_BASE, LED_G_PIN, true);
      GpioWritePin(LED_W_BASE, LED_W_PIN, false);
    }
    if(GpioReadPin(SWITCH_BASE, SWITCH_PIN)) {
      switchLock = 1;
    }

    updateDirection(adc_buf[ADC_X], adc_buf[ADC_Y]);
    updateSnakeGame();

    OS_delay(300 + adc_buf[ADC_POTENTIOMETER]/2);
  }
}

#define STACK_SIZE_TASK4 128
uint32_t stack_task4[STACK_SIZE_TASK4];
OSThread tcb_task4;
void task4() {

  bool isBuzzin = false;
  bool buzzSwitchLock = 1;
  while(1) {
    if(buzzSwitchLock && !GpioReadPin(BUZZ_SWITCH_BASE, BUZZ_SWITCH_PIN)) {
      buzzSwitchLock = 0;
      isBuzzin = !isBuzzin;
      GpioWritePin(BUZZ_LED_BASE, BUZZ_LED_PIN, isBuzzin);
    }
    if(GpioReadPin(BUZZ_SWITCH_BASE, BUZZ_SWITCH_PIN)) {
      buzzSwitchLock = 1;
    }
    if(dotColledtedEvent) {
      GpioWritePin(LED_W_BASE, LED_W_PIN, true);
      if(isBuzzin) {
	GpioWritePin(BUZZ_BASE, BUZZ_PIN, true);
      }
      OS_delay(100);
      GpioWritePin(BUZZ_BASE, BUZZ_PIN, false);
      GpioWritePin(LED_W_BASE, LED_W_PIN, false);
      dotColledtedEvent = false;
    }
    OS_delay(50);
  }
}

void main(void) {
  system_init();
  usart_init(USART2);
  GpioInit();
  CustomGpioInit();
  //ADC CONFIG

  ADC_DMAConfig();
  DMAEnableCH1Interrupt();
  DMA_ADCInit((uint32_t)adc_buf, ADC_BUF_SIZE);
  ADCStart();

  //--
  OS_init();


  OS_create_thread(&tcb_task1, 3, &task1, stack_task1, sizeof(stack_task1));
  OS_create_thread(&tcb_task2, 3, &task2, stack_task2, sizeof(stack_task2));
  OS_create_thread(&tcb_task3, 2, &task3, stack_task3, sizeof(stack_task3));
  OS_create_thread(&tcb_task4, 1, &task4, stack_task4, sizeof(stack_task4));

  __enable_irq();

  OS_start();

  while(1) {
    printf("BUTTON: %b \r\n", GpioReadPin(BUZZ_SWITCH_BASE, BUZZ_SWITCH_PIN));
    GpioTogglePin(LED_W_BASE, LED_W_PIN);
    delay_ms(400);
  }
}


void CustomGpioInit() {
  GpioSetPinMode(SEG_4_BASE,SEG_4_PIN, GPIO_OUTPUT);
  GpioSetPinMode(SEG_3_BASE,SEG_3_PIN, GPIO_OUTPUT);
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
  GpioSetPinMode(BUZZ_LED_BASE,BUZZ_LED_PIN, GPIO_OUTPUT);

  GpioSetPinMode(SWITCH_BASE, SWITCH_PIN, GPIO_INPUT);
  GpioSetPull(SWITCH_BASE, SWITCH_PIN, GPIO_PULLUP);

  GpioSetPinMode(BUZZ_SWITCH_BASE, BUZZ_SWITCH_PIN, GPIO_INPUT);
  GpioSetPull(BUZZ_SWITCH_BASE, BUZZ_SWITCH_PIN, GPIO_PULLUP);

  EnableADC_IN1_PA0();
  EnableADC_IN6_PC0();
  EnableADC_IN7_PC1();

  //GpioLD2EnableOutput();
}
