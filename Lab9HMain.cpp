// Lab9HMain.cpp
// Runs on MSPM0G3507
// Lab 9 ECE319H
// Your name
// Last Modified: 1/1/2024

#include <stdint.h>
#include <stdio.h>
#include <ti/devices/msp/msp.h>
#include "../inc/Clock.h"
#include "../inc/DAC5.h"
#include "DAC.h"
#include "../inc/LaunchPad.h"
#include "../inc/ST7735.h"
#include "../inc/SlidePot.h"
#include "../inc/TExaS.h"
#include "../inc/Timer.h"
#include "FIFO.h"
#include "FIFO2.h"
#include "KeyboardUART.h"
#include "LED.h"
#include "SmallFont.h"
#include "Sound.h"
#include "Switch.h"
#include "images/images.h"
#include "Keyboard.h"
#include "KeyboardUART.h"
#include "Math.h"
#include "JoyStick.h"
#include "sprites.h"

extern "C" void __disable_irq(void);
extern "C" void __enable_irq(void);
extern "C" void TIMG12_IRQHandler(void);
extern "C" void TIMG0_IRQHandler(void);


uint32_t timer;

char* englishWords[20] = {
     "over ","lame ","hungry ","fail ","sadness ","depression ","home ","lament ","help ","monkey ","despair ","fear ","doom ","sick ","tired ","fatigue ","bevo ","milkshake ","repent ","polyphony "
};

char* spanishWords[20] = {"hola ", "adios ", "gracias ", "porfavor ", "si ", "no ", "bien ", "mal ", "hombre ", "mujer ",
                        "nino ", "nina ", "casa ", "coche ", "perro ", "gato ", "comida ", "agua ", "libro ", "amigo "};

const int testLength = 200;
int testTime = 30;
char* testWords[testLength];
bool isError[testLength * 15];
bool isTyped[testLength * 15];
int wordIndex = 0;
int rowWidth = 23;
int testgoop = 0;
int seconds = 30;
uint32_t XData,YData;

const short TIMER_TEXT_COLOR = ST7735_YELLOW, BACKGROUND_COLOR = ST7735_DARKGREY, TEST_TEXT_COLOR_UNTYPED = ST7735_LIGHTGREY, TEST_TEXT_COLOR_TYPED = ST7735_WHITE, TEST_TEXT_COLOR_ERROR = ST7735_RED;

// ****note to ECE319K students****
// the data sheet says the ADC does not work when clock is 80 MHz
// however, the ADC seems to work on my boards at 80 MHz
// I suggest you try 80MHz, but if it doesn't work, switch to 40MHz
void PLL_Init(void) { // set phase lock loop (PLL)
  // Clock_Init40MHz(); // run this line for 40MHz
  Clock_Init80MHz(0); // run this line for 80MHz
}

uint32_t M = 1;
uint32_t Random32(void) {
  M = 1664525 * M + 1013904223;
  return M;
}
uint32_t Random(uint32_t n) { return (Random32() >> 16) % n; }

SlidePot Sensor(1500, 0); // copy calibration from Lab 7

// games  engine runs at 30Hz
void TIMG12_IRQHandler(void) {
  uint32_t pos, msg;
  if ((TIMG12->CPU_INT.IIDX) == 1) { // this will acknowledge
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
                                // game engine goes here
    // 1) sample slide pot
    // 2) read input switches
    // 3) move sprites
    // 4) start sounds
    // 5) set semaphore
    // NO LCD OUTPUT IN INTERRUPT SERVICE ROUTINES
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
  }
}
void TIMG0_IRQHandler(void){
  if((TIMG0->CPU_INT.IIDX) == 1){ // this will acknowledge
    timer++;
  }
}
uint8_t TExaS_LaunchPadLogicPB27PB26(void) {
  return (0x80 | ((GPIOB->DOUT31_0 >> 26) & 0x03));
}


// Interrupt service routine
// Executed every 12.5ns*(period)


typedef enum {English, Spanish} Language_t;
Language_t myLanguage = English;

int length(char* string) {
    int returnLength = 0;
    while (*(string + returnLength))
        returnLength++;
    return returnLength;
}

char* randWord(Language_t lang) {
    if (lang == English)
        return englishWords[Random(20)];
    else
        return spanishWords[Random(20)];
}

void initTest(Language_t lang) {
    for (int i = 0; i < testLength; i++)
        testWords[i] = randWord(lang);
}

int getTestRow(int wordIndex) { // returns row of text to display the word at the given index on
    int row = 0;
    int i = 0;
    while (i < wordIndex) {
        int rowLength = 0;
        while (rowLength <= rowWidth) {
            rowLength += length(testWords[i]);
            i++;

        }
        i--;
        if (i <= wordIndex)
            row++;
    }
    return row; //Math::max(row, 0);
}

int firstWordInRow(int rowIndex) {
    int row = 0;
    int i = 0;
    while (row < rowIndex) {
        int rowLength = 0;
        while (rowLength <= rowWidth) {
            rowLength += length(testWords[i]);
            i++;
        }
        i--;
        if (i <= testLength)
            row++;
    }
    return i;
}

void displayTest(int rowIndex) {
    int index = firstWordInRow(rowIndex);
    int rowLength = 0;
    int row = 0;
    int lastRow = row;
    int indexRow = getTestRow(index);
    int numCharacters = 0;
    for (int i = index; i < testLength; i++) {
        row = getTestRow(i);
        if (lastRow < row)
            rowLength = 0;
        lastRow = row;
        if (row < indexRow - 1 || row > indexRow + 2)
            continue;
        int currentChar = 0;
        while (testWords[i][currentChar]) {
            if (isError[testgoop + numCharacters])
                ST7735_DrawChar(10 + rowLength, 30 + (8 * (row - indexRow)), testWords[i][currentChar], TEST_TEXT_COLOR_ERROR, BACKGROUND_COLOR, 1);
            else if (isTyped[testgoop +numCharacters])
                ST7735_DrawChar(10 + rowLength, 30 + (8 * (row - indexRow)), testWords[i][currentChar], TEST_TEXT_COLOR_TYPED, BACKGROUND_COLOR, 1);
            else
                ST7735_DrawChar(10 + rowLength, 30 + (8 * (row - indexRow)), testWords[i][currentChar], TEST_TEXT_COLOR_UNTYPED, BACKGROUND_COLOR, 1);
            rowLength += 6;
            currentChar++;
            numCharacters++;
            if (i >= testLength)
                break;
        }
    }
}

int indexOfChar(int charIndex) {
    int totalLength = 0;
    int i = 0;
    while (totalLength <= charIndex) {
        totalLength += length(testWords[i]);
        i++;
    }
    return i - 1;
}

void drawScore(int time, int mistakes, int characters) {
    ST7735_FillScreen(BACKGROUND_COLOR);
//    float calculationTime = 60 / seconds;
    int wpm = (characters * 2) / (5);
    ST7735_DrawString(1, 1, "Time's up!", ST7735_YELLOW);
    ST7735_DrawString(1, 2, "WPM: ", ST7735_YELLOW);
    ST7735_SetCursor(8, 2);
    ST7735_OutChar((wpm / 100)+0x30);
    ST7735_OutChar(((wpm/10)%10)+0x30);
    ST7735_OutChar((wpm % 10)+0x30);
    while(1){}
//    ST7735_DrawString(1, 3, "Accuracy: ", ST7735_YELLOW);
//    ST7735_DrawString(12, 3, accuracy, ST7735_YELLOW);
//    ST7735_DrawString(15, 3, "%", ST7735_YELLOW);
}

void drawTime(int time) {
    if ((timer >> 7) > 9)
        ST7735_DrawChar(147, 10, ((timer >> 7) / 10) + 0x30, TIMER_TEXT_COLOR, BACKGROUND_COLOR, 1);
    ST7735_DrawChar(154, 10, ((timer >> 7) % 10) + 0x30, TIMER_TEXT_COLOR, BACKGROUND_COLOR, 1);
}

bool failed(char ascii, int characterIndex) {
    int i = 0;
    int word = 0;
    int character = 0;
    while (i <= characterIndex) {
        while (testWords[word][character]) {
            if (i >= characterIndex)
                return testWords[word][character] != ascii;
            character++;
            i++;
        }
        word++;
        character = 0;
    }
    return false;
}

int main(void) { // main3
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
  ST7735_SetRotation(3);

  char* randomString;
  int i = 0;
  bool done = false;

  Switch_Init(); // initialize switches
  LED_Init();    // initialize LED
  UART2_Init();
  Keyboard_Init();
  Sound_Init(1, 0);
  JoyStick_Init();

  TimerG0_IntArm(10000,31,2); // 129 hz, intended to be close to 128, so that we can it shift it by 7 and convert to seconds.
  __enable_irq();
  uint32_t lastTimer = 0;

  ST7735_InvertDisplay(1);
  ST7735_FillScreen(BACKGROUND_COLOR); // set screen to black
  ST7735_SetCursor(0, 0);

  int timerState = 0;
  int monkeyState = 0;

  int mistakes = 0, characters = 0;

  int nowRow = 0;
  int lastRow = nowRow;
  uint8_t EnglishSelect, lastSelect = 0;
  bool languageSelected = false;
  JoyStick_In(&XData,&YData);
  Sound_Startup();

//  int logoPos = -50;
//  for (int i = 0; i < 50; i++) {
//      Clock_Delay1ms(20);
//      logoPos++;
//      ST7735_DrawBitmap(0, logoPos, LOGO_SPRITE, LOGO_SPRITE_WIDTH, LOGO_SPRITE_HEIGHT);
//  }

  while(languageSelected == false) {
      processScanCodetoASCII();
      JoyStick_In(&XData,&YData);
      if (XData > 3000) {
          EnglishSelect = 1;
      } else if (XData < 1000) {
          EnglishSelect = 2;
      }
      if(lastSelect != EnglishSelect) {
         ST7735_FillRect(10, 30, 150, 150, BACKGROUND_COLOR);
         if(EnglishSelect == 1) {
             ST7735_SetCursor(2,2);
                  ST7735_OutString("English Selected, left for Spanish");
                  ST7735_FillRect(70, 64, 20, 20, BACKGROUND_COLOR);
                  ST7735_DrawBitmap(80 - 10, 74 - 10, SPAIN_FLAG, 9, 7);
              } else if (EnglishSelect == 2) {
                  ST7735_SetCursor(2,2);
                  ST7735_OutString("Spanish Selected, right for Spanish");
                  ST7735_FillRect(70, 64, 20, 20, BACKGROUND_COLOR);
                  ST7735_DrawBitmap(80 - 10, 74 - 10, SPAIN_FLAG, 9, 7);
              } else {ST7735_SetCursor(2,2);
                  ST7735_OutString("Left for Spanish, Right for English");
              }
      }
      lastSelect = EnglishSelect;


      if(ASCII_Fifo_Size() > 0) {
      if(EnglishSelect == 1) {
               languageSelected = true;
               initTest(English);

           } else if (EnglishSelect == 2) {
               languageSelected = true;
               initTest(Spanish);
           } else { ST7735_OutString("No language selected");
           }
      }
  }

  timer = 0;
  ASCII_Fifo_Get();
  ST7735_FillScreen(BACKGROUND_COLOR);
  ST7735_DrawBitmap(144, 112, MONKEY_IMAGE_TWO, 16, 17);

  while(!done) {

      //int testNum = firstWordInRow(1);
      //ST7735_FillScreen(ST7735_BLACK);
      nowRow = getTestRow(indexOfChar(i));
      if (nowRow != lastRow) {
          testgoop = i;
          ST7735_FillRect(10, 30, 150, 98, BACKGROUND_COLOR);
      }
      lastRow = nowRow;
      displayTest(nowRow); // displayTest needs word index, we have character index
      //displayTest(3);
      processScanCodetoASCII();
      if (ASCII_Fifo_Size() > 0) {
          if (monkeyState == 0)
              ST7735_DrawBitmap(144, 112, MONKEY_IMAGE_ONE, 16, 17);
          else
              ST7735_DrawBitmap(144, 112, MONKEY_IMAGE_TWO, 16, 17);
          monkeyState = (monkeyState + 1) % 2;

          char ascii = ASCII_Fifo_Get();

          if (ascii == 0x00) {
              Sound_Failed();
          }
          else if (ascii == 0x08) { // Backspace
              Sound_Type();
              i--;
              isTyped[i] = false;
              characters--;
          }
          else if (failed(ascii, i)) {
              isError[i] = true;
              i++;
              Sound_Failed();
              mistakes++;
              characters++;
          }
          else{
              isTyped[i] = true;
              Sound_Type();
              i++;
              characters++;
          }
      }
      if ((timer >> 7) <= seconds) {
          drawTime(timer);
          if ((timer >> 7) < 8)
              ST7735_DrawBitmap(135, 17, STOPWATCH_ONE, 9, 11);
          else if ((timer >> 16) < 16)
              ST7735_DrawBitmap(135, 17, STOPWATCH_TWO, 9, 11);
          else if ((timer >> 7) < 24)
              ST7735_DrawBitmap(135, 17, STOPWATCH_THREE, 9, 11);
          else
              ST7735_DrawBitmap(135, 17, STOPWATCH_FOUR, 9, 11);
      }
      else {
          done = true;
          Sound_Timeout();
          drawScore(timer, mistakes, characters);
          ST7735_DrawBitmap(135, 17, STOPWATCH_FIVE, 9, 11);
      }


  }

}
