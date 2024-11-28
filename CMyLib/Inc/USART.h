#ifndef ___USART_H
#define ___USART_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "define.h"
/* Global define */
#define TxSize(TxBuffer)     (size(TxBuffer))
#define size(a)    (sizeof(a) / sizeof(*(a)))
// ring buffer size
#define RING_BUFFER_LEN     (1024u)

// The length of a single buffer used by DMA
#define BUFFER_SIZE(buffer)			sizeof(buffer)
#define RX_BUFFER_LEN       (128u)

#define USART1_RX_CH         DMA1_Channel5
#define USART2_RX_CH         DMA1_Channel6
#define USART3_RX_CH         DMA1_Channel3

typedef struct
{
	volatile uint8_t DMA_USE_BUFFER;
	uint8_t Rx_Buffer[2][RX_BUFFER_LEN];
} DmaCtrl;

typedef struct
{
	uint8_t buffer[RING_BUFFER_LEN];
	volatile uint16_t RecvPos;  //
	volatile uint16_t SendPos;  //
	volatile uint16_t RemainCount;
} RingBuffer;

#ifdef __cplusplus
}
#endif

#endif /* ___USART1_H */