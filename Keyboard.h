/*
 * Keyboard.h
 *
 *  Created on: Apr 21, 2024
 *      Author: jason
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include "FIFO.h"

class Keyboard {
public:
    void processScanCodetoASCII(void);
    void Keyboard_Init(void);
};
void Keyboard_Init(void);
/*
 * @param: void
 * @converts scancode to ascii or actionable
 */
void processScanCodetoASCII(void);





#endif /* KEYBOARD_H_ */
