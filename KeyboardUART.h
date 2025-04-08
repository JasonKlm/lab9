/*
 * KeyboardUART.h
 *
 *  Created on: Apr 20, 2024
 */

#ifndef KEYBOARDUART_H_
#define KEYBOARDUART_H_

/**
 * initialize UART2 for 2000 bps baud rate.<br>
 * PA22 UART2 Rx from other microcontroller PA8 Tx<br>
 * no transmit, interrupt on receive timeout
 * @param none
 * @return none
 * @brief  Initialize UART1
*/
void UART2_Init(void);

/**
 * Get new serial port receive data from FIFO1<br>
 * Return 0 if the FIFO1 is empty<br>
 * Return nonzero data from the FIFO if available
 * @param none
 * @return char ASCII code from other computer
 * @brief get data from FIFO
 */
char UART2_InChar(void);




#endif /* KEYBOARDUART_H_ */
