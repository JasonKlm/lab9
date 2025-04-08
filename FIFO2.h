/*!
 * @defgroup FIFO
 * @brief First in first out queue
 * @{*/
/**
 * @file      FIFO.h
 * @brief     Provide functions for a first in first out queue
 * @details   Runs on any Microcontroller.
 * Provide functions that initialize a FIFO, put data in, get data out,
 * and return the current size.  The file includes two FIFOs
 * using index implementation.
 * @version   ECE319K v1.0
 * @author    Daniel Valvano and Jonathan Valvano
 * @copyright Copyright 2023 by Jonathan W. Valvano, valvano@mail.utexas.edu,
 * @warning   AS-IS
 * @note      For more information see  http://users.ece.utexas.edu/~valvano/
 * @date      August 13, 2023

 */

#ifndef __FIFO2_H__
#define __FIFO2_H__


/**
 * \brief FIFOSIZE the size of the receive FIFO, which can hold 0 to RXFIFOSIZE-1 elements
 * The size must be a power of 2.
 */
#define ASCIIFIFOSIZE 64 // must be a power of 2

/**
 * Initialize the receive FIFO
 * @param none
 * @return none
 * @see Fifo_Put() Fifo_Get() Fifo_Size()
 * @brief  Initialize FIFO
 * @note RXFIFOSIZE the size of the receive FIFO
 */
void ASCII_Fifo_Init(void);

/**
 * Put character into the receive FIFO
 * @param data is a new character to save
 * @return 0 for fail because full, 1 for success
 * @see RxFifo_Init() RxFifo_Get() RxFifo_Size()
 * @brief Put FIFO
 * @note FIFOSIZE the size of the receive FIFO
 */
void ASCII_Fifo_Put(char data);

/**
 * Get character from the receive FIFO
 * @param none
 * @return 0 for fail because empty, nonzero is data
 * @see Fifo_Init() Fifo_Put() Fifo_Size()
 * @brief Get FIFO
 * @note FIFOSIZE the size of the receive FIFO
 */
char ASCII_Fifo_Get(void);

/**
 * Determine how many elements are currently stored in the receive FIFO
 * @param none
 * @return number of elements in FIFO
 * @see Fifo_Init() Fifo_Put() Fifo_Get()
 * @brief number of elements in FIFO
 * @note Does not change the FIFO
 */
int ASCII_Fifo_Size(void);

#endif //  __FIFO_H__
/** @}*/
