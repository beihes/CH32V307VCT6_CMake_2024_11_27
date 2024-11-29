/*
 * usart.h
 *
 *  Created on: 2024年11月29日
 *      Author: 26448
 */

#ifndef APP_DRIVERS_JY61P_USART2_H_
#define APP_DRIVERS_JY61P_USART2_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "define.h"
#include "USART.h"

    void USART2_Init(uint32_t USARTx_BaudRate);

    void DMA1_USART2_Init(void);

    void Ring_Buffer_Push_Huge_Usart2(uint8_t *buffer, uint16_t len);

    uint8_t Ring_Buffer_Pop_Usart2();

    void USART2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
    void DMA1_Channel6_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

    void send_Bufffer_Usart2(const char *sendBuffer, uint16_t bufferSize);

    DmaCtrl *getUsart2DmaCtrl(void);

    RingBuffer *getRingBufferUsart2(void);

    void setUsart2DmaCtrlNULL(void);

    void setRingBufferUsart2NULL(void);

#ifdef __cplusplus
}
#endif

#endif /* APP_DRIVERS_JY61P_USART2_H_ */
