#ifndef __CAN_SPI_H
#define	__CAN_SPI_H

#include "stm32f4xx_hal.h"
#include <stdbool.h>

#define dSTANDARD_CAN_MSG_ID_2_0B 0x00000000U
#define dEXTENDED_CAN_MSG_ID_2_0B 0x00000004U
#define CAN_MAX_DLEN 8
#define MCP2515_FIRST 0
#define MCP2515_SECOND 1

typedef union {
  struct {
    uint8_t idType;
    uint32_t id;
    uint8_t dlc;
    uint8_t data0;
    uint8_t data1;
    uint8_t data2;
    uint8_t data3;
    uint8_t data4;
    uint8_t data5;
    uint8_t data6;
    uint8_t data7;
  } frame;
  uint8_t array[14];
} uCAN_MSG;



bool CANSPI_Initialize(uint8_t CS);
void CANSPI_Sleep(uint8_t CS);
uint8_t CANSPI_Transmit(uCAN_MSG *tempCanMsg, uint8_t CS);
uint8_t CANSPI_Receive(uCAN_MSG *tempCanMsg, uint8_t CS);
uint8_t CANSPI_messagesInBuffer(uint8_t CS);
uint8_t CANSPI_isBussOff(uint8_t CS);
uint8_t CANSPI_isRxErrorPassive(uint8_t CS);
uint8_t CANSPI_isTxErrorPassive(uint8_t CS);

#endif	/* __CAN_SPI_H */
