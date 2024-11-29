/*
 * usart.c
 *
 *  Created on: 2024年11月29日
 *      Author: 26448
 */

#include "usart2.h"

DmaCtrl Usart2DmaCtrl = {.DMA_USE_BUFFER = 0, .Rx_Buffer[0] = {0}, .Rx_Buffer[1] = {0}};
RingBuffer ringBufferUsart2 = {.buffer = {0}, .RecvPos = 0, .RemainCount = 0, .SendPos = 0};

void USART2_Init(uint32_t USARTx_BaudRate)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    /* USART2 TX-->A.2   RX-->A.3 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = USARTx_BaudRate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

    USART_Init(USART2, &USART_InitStructure);
    USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(USART2, ENABLE);
}

void DMA1_USART2_Init(void)
{
    DMA_InitTypeDef DMA_InitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    DMA_DeInit(USART2_RX_CH);
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART2->DATAR);
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)Usart2DmaCtrl.Rx_Buffer[0];
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = RX_BUFFER_LEN;
    DMA_Init(USART2_RX_CH, &DMA_InitStructure);

    DMA_ITConfig(USART2_RX_CH, DMA_IT_TC, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    DMA_Cmd(USART2_RX_CH, ENABLE);
    USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
}

void Ring_Buffer_Push_Huge_Usart2(uint8_t *buffer, uint16_t len)
{
    const uint16_t bufferRemainCount = RING_BUFFER_LEN - ringBufferUsart2.RemainCount;
    if (bufferRemainCount < len)
    {
        len = bufferRemainCount;
    }

    const uint16_t bufferSize = RING_BUFFER_LEN - ringBufferUsart2.RecvPos;
    if (bufferSize >= len)
    {
        memcpy(&(ringBufferUsart2.buffer[ringBufferUsart2.RecvPos]), buffer, len);
        ringBufferUsart2.RecvPos += len;
    }
    else
    {
        uint16_t otherSize = len - bufferSize;
        memcpy(&(ringBufferUsart2.buffer[ringBufferUsart2.RecvPos]), buffer, bufferSize);
        memcpy(ringBufferUsart2.buffer, &(buffer[bufferSize]), otherSize);
        ringBufferUsart2.RecvPos = otherSize;
    }
    ringBufferUsart2.RemainCount += len;
}

uint8_t Ring_Buffer_Pop_Usart2()
{
    uint8_t data = ringBufferUsart2.buffer[ringBufferUsart2.SendPos];

    ringBufferUsart2.SendPos++;
    if (ringBufferUsart2.SendPos >= RING_BUFFER_LEN)
    {
        ringBufferUsart2.SendPos = 0;
    }
    ringBufferUsart2.RemainCount--;
    return data;
}

void USART2_IRQHandler(void)
{
    if (USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
    {
        // IDLE
        uint16_t rxlen = (RX_BUFFER_LEN - USART2_RX_CH->CNTR);
        uint8_t oldbuffer = Usart2DmaCtrl.DMA_USE_BUFFER;

        Usart2DmaCtrl.DMA_USE_BUFFER = !oldbuffer;

        DMA_Cmd(USART2_RX_CH, DISABLE);
        DMA_SetCurrDataCounter(USART2_RX_CH, RX_BUFFER_LEN);
        // Switch buffer
        USART2_RX_CH->MADDR = (uint32_t)(Usart2DmaCtrl.Rx_Buffer[Usart2DmaCtrl.DMA_USE_BUFFER]);
        DMA_Cmd(USART2_RX_CH, ENABLE);

        USART_ReceiveData(USART2); // clear IDLE flag
        Ring_Buffer_Push_Huge_Usart2(Usart2DmaCtrl.Rx_Buffer[oldbuffer], rxlen);
    }
}

void DMA1_Channel6_IRQHandler(void)
{
    uint16_t rxlen = RX_BUFFER_LEN;
    uint8_t oldbuffer = Usart2DmaCtrl.DMA_USE_BUFFER;
    // FULL

    Usart2DmaCtrl.DMA_USE_BUFFER = !oldbuffer;

    DMA_Cmd(USART2_RX_CH, DISABLE);
    DMA_SetCurrDataCounter(USART2_RX_CH, RX_BUFFER_LEN);
    // Switch buffer
    USART2_RX_CH->MADDR = (uint32_t)(Usart2DmaCtrl.Rx_Buffer[Usart2DmaCtrl.DMA_USE_BUFFER]);
    DMA_Cmd(USART2_RX_CH, ENABLE);

    Ring_Buffer_Push_Huge_Usart2(Usart2DmaCtrl.Rx_Buffer[oldbuffer], rxlen);

    DMA_ClearITPendingBit(DMA1_IT_TC5);
}

void send_Bufffer_Usart2(const char *sendBuffer, uint16_t bufferSize)
{
    int TxNum = 0;
    while (TxNum < bufferSize)
    {
        USART_SendData(USART2, sendBuffer[TxNum++]);
        while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
        {
            /* waiting for sending finish */
        }
    }
}

DmaCtrl *getUsart2DmaCtrl(void)
{
    return &Usart2DmaCtrl;
}

RingBuffer *getRingBufferUsart2(void)
{
    return &ringBufferUsart2;
}

void setUsart2DmaCtrlNULL(void)
{
    Usart2DmaCtrl.DMA_USE_BUFFER = 0;
    for (size_t i = 0; i < 2; i++)
    {
        memset(Usart2DmaCtrl.Rx_Buffer[i], 0x00, RX_BUFFER_LEN);
    }
}

void setRingBufferUsart2NULL(void)
{
    while (ringBufferUsart2.RemainCount > 0)
    {
        Ring_Buffer_Pop_Usart2();
    }
}
