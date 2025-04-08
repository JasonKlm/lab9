/*
 * Keyboard.cpp
 *
 *  Created on: Apr 21, 2024
 *      Author: jason
 */

#include <stdint.h>
#include <stdio.h>
#include "FIFO.h"
#include "FIFO2.h"
#include "Keyboard.h"
#include "KeyboardUART.h"

uint8_t shiftFlag = 0;
bool ignore = false;
char ScanCodetoASCII[0xFF];
char ShiftScanCodetoASCII[0xFF];
int ReceiveCount = 0;
void Keyboard_Init(void) {
    for (int i = 0; i < 0xFF; ++i) {
        ScanCodetoASCII[i] = 0;
    }
    for (int i = 0; i < 0xFF; ++i) {
            ShiftScanCodetoASCII[i] = 0;
        }
    ScanCodetoASCII[0x1C] = 0x61; // A
    ScanCodetoASCII[0x32] = 0x62; // B
    ScanCodetoASCII[0x21] = 0x63; // C
    ScanCodetoASCII[0x23] = 0x64; // D
    ScanCodetoASCII[0x24] = 0x65; // E
    ScanCodetoASCII[0x2B] = 0x66; // F
    ScanCodetoASCII[0x34] = 0x67; // G
    ScanCodetoASCII[0x33] = 0x68; // H
    ScanCodetoASCII[0x43] = 0x69; // I
    ScanCodetoASCII[0x3B] = 0x6A; // J
    ScanCodetoASCII[0x42] = 0x6B; // K
    ScanCodetoASCII[0x4B] = 0x6C; // L
    ScanCodetoASCII[0x3A] = 0x6D; // M
    ScanCodetoASCII[0x31] = 0x6E; // N
    ScanCodetoASCII[0x44] = 0x6F; // O
    ScanCodetoASCII[0x4D] = 0x70; // P
    ScanCodetoASCII[0x15] = 0x71; // Q
    ScanCodetoASCII[0x2D] = 0x72; // R
    ScanCodetoASCII[0x1B] = 0x73; // S
    ScanCodetoASCII[0x2C] = 0x74; // T
    ScanCodetoASCII[0x3C] = 0x75; // U
    ScanCodetoASCII[0x2A] = 0x76; // V
    ScanCodetoASCII[0x1D] = 0x77; // W
    ScanCodetoASCII[0x22] = 0x78; // X
    ScanCodetoASCII[0x35] = 0x79; // Y
    ScanCodetoASCII[0x1A] = 0x7A; // Z

    ScanCodetoASCII[0x16] = 0x31; // 1
    ScanCodetoASCII[0x1E] = 0x32; // 2
    ScanCodetoASCII[0x26] = 0x33; // 3
    ScanCodetoASCII[0x25] = 0x34; // 4
    ScanCodetoASCII[0x2E] = 0x35; // 5
    ScanCodetoASCII[0x36] = 0x36; // 6
    ScanCodetoASCII[0x3D] = 0x37; // 7
    ScanCodetoASCII[0x3E] = 0x38; // 8
    ScanCodetoASCII[0x46] = 0x39; // 9
    ScanCodetoASCII[0x45] = 0x3A; // 0
    ScanCodetoASCII[0x4E] = 0x2D; //-
    ScanCodetoASCII[0x55] = 0x3D; //=
    ScanCodetoASCII[0x5D] = 0x5C; // \backslash
    ScanCodetoASCII[0x54] = 0x5B; // [
    ScanCodetoASCII[0x5B] = 0x5D; // ]
    ScanCodetoASCII[0x4C] = 0x3B; // ;
    ScanCodetoASCII[0x52] = 0x27; // '
    ScanCodetoASCII[0x41] = 0x2C; // ,
    ScanCodetoASCII[0x49] = 0x2E; // .
    ScanCodetoASCII[0x4A] = 0x2F; // /slash


    ScanCodetoASCII[0x29] = 0x20; // space


 /* numpad */
    ScanCodetoASCII[0x69] = 0x31; // 1
    ScanCodetoASCII[0x72] = 0x32; // 2
    ScanCodetoASCII[0x7A] = 0x33; // 3
    ScanCodetoASCII[0x6B] = 0x34; // 4
    ScanCodetoASCII[0x73] = 0x35; // 5
    ScanCodetoASCII[0x74] = 0x36; // 6
    ScanCodetoASCII[0x6C] = 0x37; // 7
    ScanCodetoASCII[0x75] = 0x38; // 8
    ScanCodetoASCII[0x7D] = 0x39; // 9
    ScanCodetoASCII[0x70] = 0x3A; // 0
/* action keys */
    ScanCodetoASCII[0x71] = 0x17; // delete
    ScanCodetoASCII[0x76] = 27; // ESCAPE
    ScanCodetoASCII[0x66] = 0x08; // backspace
    ScanCodetoASCII[0x5A] = 10; // enter
    ScanCodetoASCII[0x0D] = 0x17; // tab




// shift table
    ShiftScanCodetoASCII[0x1C] = 0x41; // A
    ShiftScanCodetoASCII[0x32] = 0x42; // B
    ShiftScanCodetoASCII[0x21] = 0x43; // C
    ShiftScanCodetoASCII[0x23] = 0x44; // D
    ShiftScanCodetoASCII[0x24] = 0x45; // E
    ShiftScanCodetoASCII[0x2B] = 0x46; // F
    ShiftScanCodetoASCII[0x34] = 0x47; // G
    ShiftScanCodetoASCII[0x33] = 0x48; // H
    ShiftScanCodetoASCII[0x43] = 0x49; // I
    ShiftScanCodetoASCII[0x3B] = 0x4A; // J
    ShiftScanCodetoASCII[0x42] = 0x4B; // K
    ShiftScanCodetoASCII[0x4B] = 0x4C; // L
    ShiftScanCodetoASCII[0x3A] = 0x4D; // M
    ShiftScanCodetoASCII[0x31] = 0x4E; // N
    ShiftScanCodetoASCII[0x44] = 0x4F; // O
    ShiftScanCodetoASCII[0x4D] = 0x50; // P
    ShiftScanCodetoASCII[0x15] = 0x51; // Q
    ShiftScanCodetoASCII[0x2D] = 0x52; // R
    ShiftScanCodetoASCII[0x1B] = 0x53; // S
    ShiftScanCodetoASCII[0x2C] = 0x54; // T
    ShiftScanCodetoASCII[0x3C] = 0x55; // U
    ShiftScanCodetoASCII[0x2A] = 0x56; // V
    ShiftScanCodetoASCII[0x1D] = 0x57; // W
    ShiftScanCodetoASCII[0x22] = 0x58; // X
    ShiftScanCodetoASCII[0x35] = 0x59; // Y
    ShiftScanCodetoASCII[0x1A] = 0x5A; // Z

    ShiftScanCodetoASCII[0x16] = 0x21; // !
    ShiftScanCodetoASCII[0x1E] = 0x40; // @
    ShiftScanCodetoASCII[0x26] = 0x23; // #
    ShiftScanCodetoASCII[0x25] = 0x24; // $
    ShiftScanCodetoASCII[0x2E] = 0x25; // %
    ShiftScanCodetoASCII[0x36] = 0x5E; // ^
    ShiftScanCodetoASCII[0x3D] = 0x26; // &
    ShiftScanCodetoASCII[0x3E] = 0x2A; // *
    ShiftScanCodetoASCII[0x46] = 0x28; // (
    ShiftScanCodetoASCII[0x45] = 0x29; // )
    ShiftScanCodetoASCII[0x4E] = 0x5F; // _
    ShiftScanCodetoASCII[0x55] = 0x2B; // +
    ShiftScanCodetoASCII[0x5D] = 0x7C; // |
    ShiftScanCodetoASCII[0x0E] = 0x37; // ~
    ShiftScanCodetoASCII[0x54] = 0x7B; // {
    ShiftScanCodetoASCII[0x5B] = 0x7D; // }
    ShiftScanCodetoASCII[0x4C] = 0x3A; // :
    ShiftScanCodetoASCII[0x52] = 0x22; // "
    ShiftScanCodetoASCII[0x41] = 0x3C; // <
    ShiftScanCodetoASCII[0x49] = 0x3E; // >
    ShiftScanCodetoASCII[0x4A] = 0x3F; // ?


    ShiftScanCodetoASCII[0x29] = 0x20; // space


/* action keys */
    ShiftScanCodetoASCII[0x74] = 0x17; // right
    ShiftScanCodetoASCII[0x75] = 0x17; // up
    ShiftScanCodetoASCII[0x6B] = 0x17; // left
    ShiftScanCodetoASCII[0x72] = 0x17; // down
    ShiftScanCodetoASCII[0x71] = 0x17; // delete
    ShiftScanCodetoASCII[0x76] = 27; // ESCAPE
    ShiftScanCodetoASCII[0x66] = 0x08; // backspace
    ShiftScanCodetoASCII[0x5A] = 10; // enter
    ShiftScanCodetoASCII[0x0D] = 0x17; // tab
    shiftFlag = 0;
    ignore = false;
};
void processScanCodetoASCII(void) {


  while (RxFifo_Size() > 0) {
    char data = RxFifo_Get();
    if (data == 0) break; // user input invalid

    if (ignore == false) {
      if (data != 0xF0 && data != 0x12 && data != 0xE0 && data != 0x59) /* check for non-characters* lshift, 0x12, rshift, 0x59*/ {

        if (shiftFlag == 0) {
          ASCII_Fifo_Put(ScanCodetoASCII[data]);
        } else if (shiftFlag == 1) {
          ASCII_Fifo_Put(ShiftScanCodetoASCII[data]);
        }

      }

      else if (data == 0x12 || data == 0x59) {
        shiftFlag = 1;
        break;
      }  else if (data == 0xE0) {
          while(RxFifo_Size() == 0){} // wait for next input, because of E0
          data = RxFifo_Get();
          if (data == 0xF0) {ignore = true; break;}
          ASCII_Fifo_Put(ShiftScanCodetoASCII[data]);
          }
      else { /* case for when data = 0xF0 */
        ignore = true;
        break;
      }
    } /* if the ignore flag was true and the data is 0x12, that means shift was released */
    if (data == 0x12 || data == 0x59) {
      shiftFlag = 0;
    } /* reset ignore flag after one iteration */
    ignore = false;
  }

}





