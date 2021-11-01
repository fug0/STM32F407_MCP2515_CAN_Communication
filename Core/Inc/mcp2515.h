#ifndef __MCP2515_H
#define	__MCP2515_H

#include "stm32f4xx_hal.h"
#include "stdbool.h"

/* MCP2515 SPI Instruction Set */
#define MCP2515_RESET           0xC0

#define MCP2515_READ            0x03
#define MCP2515_READ_RXB0SIDH   0x90
#define MCP2515_READ_RXB0D0     0x92
#define MCP2515_READ_RXB1SIDH   0x94
#define MCP2515_READ_RXB1D0     0x96

#define MCP2515_WRITE           0x02
#define MCP2515_LOAD_TXB0SIDH   0x40    /* TX0 ID location */
#define MCP2515_LOAD_TXB0D0     0x41    /* TX0 Data location */
#define MCP2515_LOAD_TXB1SIDH   0x42    /* TX1 ID location */
#define MCP2515_LOAD_TXB1D0     0x43    /* TX1 Data location */
#define MCP2515_LOAD_TXB2SIDH   0x44    /* TX2 ID location */
#define MCP2515_LOAD_TXB2D0     0x45    /* TX2 Data location */

#define MCP2515_RTS_TX0         0x81
#define MCP2515_RTS_TX1         0x82
#define MCP2515_RTS_TX2         0x84
#define MCP2515_RTS_ALL         0x87
#define MCP2515_READ_STATUS     0xA0
#define MCP2515_RX_STATUS       0xB0
#define MCP2515_BIT_MOD         0x05

/* MCP25152515 Register Adresses */
#define MCP2515_RXF0SIDH	0x00
#define MCP2515_RXF0SIDL	0x01
#define MCP2515_RXF0EID8	0x02
#define MCP2515_RXF0EID0	0x03
#define MCP2515_RXF1SIDH	0x04
#define MCP2515_RXF1SIDL	0x05
#define MCP2515_RXF1EID8	0x06
#define MCP2515_RXF1EID0	0x07
#define MCP2515_RXF2SIDH	0x08
#define MCP2515_RXF2SIDL	0x09
#define MCP2515_RXF2EID8	0x0A
#define MCP2515_RXF2EID0	0x0B
#define MCP2515_CANSTAT		0x0E
#define MCP2515_CANCTRL		0x0F

#define MCP2515_RXF3SIDH	0x10
#define MCP2515_RXF3SIDL	0x11
#define MCP2515_RXF3EID8	0x12
#define MCP2515_RXF3EID0	0x13
#define MCP2515_RXF4SIDH	0x14
#define MCP2515_RXF4SIDL	0x15
#define MCP2515_RXF4EID8	0x16
#define MCP2515_RXF4EID0	0x17
#define MCP2515_RXF5SIDH	0x18
#define MCP2515_RXF5SIDL	0x19
#define MCP2515_RXF5EID8	0x1A
#define MCP2515_RXF5EID0	0x1B
#define MCP2515_TEC		0x1C
#define MCP2515_REC		0x1D

#define MCP2515_RXM0SIDH	0x20
#define MCP2515_RXM0SIDL	0x21
#define MCP2515_RXM0EID8	0x22
#define MCP2515_RXM0EID0	0x23
#define MCP2515_RXM1SIDH	0x24
#define MCP2515_RXM1SIDL	0x25
#define MCP2515_RXM1EID8	0x26
#define MCP2515_RXM1EID0	0x27
#define MCP2515_CNF3		0x28
#define MCP2515_CNF2		0x29
#define MCP2515_CNF1		0x2A
#define MCP2515_CANINTE		0x2B
#define MCP2515_CANINTF		0x2C
#define MCP2515_EFLG		0x2D

#define MCP2515_TXB0CTRL	0x30
#define MCP2515_TXB1CTRL	0x40
#define MCP2515_TXB2CTRL	0x50
#define MCP2515_RXB0CTRL	0x60
#define MCP2515_RXB0SIDH	0x61
#define MCP2515_RXB1CTRL	0x70
#define MCP2515_RXB1SIDH	0x71

/* Defines for Rx Status */
#define MSG_IN_RXB0             0x01
#define MSG_IN_RXB1             0x02
#define MSG_IN_BOTH_BUFFERS     0x03


/* Defines for Clock and Speed */
/*
 *  Speed 8M
 */
#define MCP_8MHz_1000kBPS_CFG1 (0x00)
#define MCP_8MHz_1000kBPS_CFG2 (0x80)
#define MCP_8MHz_1000kBPS_CFG3 (0x80)

#define MCP_8MHz_500kBPS_CFG1 (0x00)
#define MCP_8MHz_500kBPS_CFG2 (0x90)
#define MCP_8MHz_500kBPS_CFG3 (0x82)

#define MCP_8MHz_250kBPS_CFG1 (0x00)
#define MCP_8MHz_250kBPS_CFG2 (0xB1)
#define MCP_8MHz_250kBPS_CFG3 (0x85)

#define MCP_8MHz_200kBPS_CFG1 (0x00)
#define MCP_8MHz_200kBPS_CFG2 (0xB4)
#define MCP_8MHz_200kBPS_CFG3 (0x86)

#define MCP_8MHz_125kBPS_CFG1 (0x01)
#define MCP_8MHz_125kBPS_CFG2 (0xB1)
#define MCP_8MHz_125kBPS_CFG3 (0x85)

#define MCP_8MHz_100kBPS_CFG1 (0x01)
#define MCP_8MHz_100kBPS_CFG2 (0xB4)
#define MCP_8MHz_100kBPS_CFG3 (0x86)

#define MCP_8MHz_80kBPS_CFG1 (0x01)
#define MCP_8MHz_80kBPS_CFG2 (0xBF)
#define MCP_8MHz_80kBPS_CFG3 (0x87)

#define MCP_8MHz_50kBPS_CFG1 (0x03)
#define MCP_8MHz_50kBPS_CFG2 (0xB4)
#define MCP_8MHz_50kBPS_CFG3 (0x86)

#define MCP_8MHz_40kBPS_CFG1 (0x03)
#define MCP_8MHz_40kBPS_CFG2 (0xBF)
#define MCP_8MHz_40kBPS_CFG3 (0x87)

#define MCP_8MHz_33k3BPS_CFG1 (0x47)
#define MCP_8MHz_33k3BPS_CFG2 (0xE2)
#define MCP_8MHz_33k3BPS_CFG3 (0x85)

#define MCP_8MHz_31k25BPS_CFG1 (0x07)
#define MCP_8MHz_31k25BPS_CFG2 (0xA4)
#define MCP_8MHz_31k25BPS_CFG3 (0x84)

#define MCP_8MHz_20kBPS_CFG1 (0x07)
#define MCP_8MHz_20kBPS_CFG2 (0xBF)
#define MCP_8MHz_20kBPS_CFG3 (0x87)

#define MCP_8MHz_10kBPS_CFG1 (0x0F)
#define MCP_8MHz_10kBPS_CFG2 (0xBF)
#define MCP_8MHz_10kBPS_CFG3 (0x87)

#define MCP_8MHz_5kBPS_CFG1 (0x1F)
#define MCP_8MHz_5kBPS_CFG2 (0xBF)
#define MCP_8MHz_5kBPS_CFG3 (0x87)

/*
 *  speed 16M
 */
#define MCP_16MHz_1000kBPS_CFG1 (0x00)
#define MCP_16MHz_1000kBPS_CFG2 (0xD0)
#define MCP_16MHz_1000kBPS_CFG3 (0x82)

#define MCP_16MHz_500kBPS_CFG1 (0x00)
#define MCP_16MHz_500kBPS_CFG2 (0xF0)
#define MCP_16MHz_500kBPS_CFG3 (0x86)

#define MCP_16MHz_250kBPS_CFG1 (0x41)
#define MCP_16MHz_250kBPS_CFG2 (0xF1)
#define MCP_16MHz_250kBPS_CFG3 (0x85)

#define MCP_16MHz_200kBPS_CFG1 (0x01)
#define MCP_16MHz_200kBPS_CFG2 (0xFA)
#define MCP_16MHz_200kBPS_CFG3 (0x87)

#define MCP_16MHz_125kBPS_CFG1 (0x03)
#define MCP_16MHz_125kBPS_CFG2 (0xF0)
#define MCP_16MHz_125kBPS_CFG3 (0x86)

#define MCP_16MHz_100kBPS_CFG1 (0x03)
#define MCP_16MHz_100kBPS_CFG2 (0xFA)
#define MCP_16MHz_100kBPS_CFG3 (0x87)

#define MCP_16MHz_80kBPS_CFG1 (0x03)
#define MCP_16MHz_80kBPS_CFG2 (0xFF)
#define MCP_16MHz_80kBPS_CFG3 (0x87)

#define MCP_16MHz_83k3BPS_CFG1 (0x03)
#define MCP_16MHz_83k3BPS_CFG2 (0xBE)
#define MCP_16MHz_83k3BPS_CFG3 (0x07)

#define MCP_16MHz_50kBPS_CFG1 (0x07)
#define MCP_16MHz_50kBPS_CFG2 (0xFA)
#define MCP_16MHz_50kBPS_CFG3 (0x87)

#define MCP_16MHz_40kBPS_CFG1 (0x07)
#define MCP_16MHz_40kBPS_CFG2 (0xFF)
#define MCP_16MHz_40kBPS_CFG3 (0x87)

#define MCP_16MHz_33k3BPS_CFG1 (0x4E)
#define MCP_16MHz_33k3BPS_CFG2 (0xF1)
#define MCP_16MHz_33k3BPS_CFG3 (0x85)

#define MCP_16MHz_20kBPS_CFG1 (0x0F)
#define MCP_16MHz_20kBPS_CFG2 (0xFF)
#define MCP_16MHz_20kBPS_CFG3 (0x87)

#define MCP_16MHz_10kBPS_CFG1 (0x1F)
#define MCP_16MHz_10kBPS_CFG2 (0xFF)
#define MCP_16MHz_10kBPS_CFG3 (0x87)

#define MCP_16MHz_5kBPS_CFG1 (0x3F)
#define MCP_16MHz_5kBPS_CFG2 (0xFF)
#define MCP_16MHz_5kBPS_CFG3 (0x87)

/*
 *  speed 20M
 */
#define MCP_20MHz_1000kBPS_CFG1 (0x00)
#define MCP_20MHz_1000kBPS_CFG2 (0xD9)
#define MCP_20MHz_1000kBPS_CFG3 (0x82)

#define MCP_20MHz_500kBPS_CFG1 (0x00)
#define MCP_20MHz_500kBPS_CFG2 (0xFA)
#define MCP_20MHz_500kBPS_CFG3 (0x87)

#define MCP_20MHz_250kBPS_CFG1 (0x41)
#define MCP_20MHz_250kBPS_CFG2 (0xFB)
#define MCP_20MHz_250kBPS_CFG3 (0x86)

#define MCP_20MHz_200kBPS_CFG1 (0x01)
#define MCP_20MHz_200kBPS_CFG2 (0xFF)
#define MCP_20MHz_200kBPS_CFG3 (0x87)

#define MCP_20MHz_125kBPS_CFG1 (0x03)
#define MCP_20MHz_125kBPS_CFG2 (0xFA)
#define MCP_20MHz_125kBPS_CFG3 (0x87)

#define MCP_20MHz_100kBPS_CFG1 (0x04)
#define MCP_20MHz_100kBPS_CFG2 (0xFA)
#define MCP_20MHz_100kBPS_CFG3 (0x87)

#define MCP_20MHz_83k3BPS_CFG1 (0x04)
#define MCP_20MHz_83k3BPS_CFG2 (0xFE)
#define MCP_20MHz_83k3BPS_CFG3 (0x87)

#define MCP_20MHz_80kBPS_CFG1 (0x04)
#define MCP_20MHz_80kBPS_CFG2 (0xFF)
#define MCP_20MHz_80kBPS_CFG3 (0x87)

#define MCP_20MHz_50kBPS_CFG1 (0x09)
#define MCP_20MHz_50kBPS_CFG2 (0xFA)
#define MCP_20MHz_50kBPS_CFG3 (0x87)

#define MCP_20MHz_40kBPS_CFG1 (0x09)
#define MCP_20MHz_40kBPS_CFG2 (0xFF)
#define MCP_20MHz_40kBPS_CFG3 (0x87)

#define MCP_20MHz_33k3BPS_CFG1 (0x0B)
#define MCP_20MHz_33k3BPS_CFG2 (0xFF)
#define MCP_20MHz_33k3BPS_CFG3 (0x87)

typedef union{
  struct{
    unsigned RX0IF      : 1;
    unsigned RX1IF      : 1;
    unsigned TXB0REQ    : 1;
    unsigned TX0IF      : 1;
    unsigned TXB1REQ    : 1;
    unsigned TX1IF      : 1;
    unsigned TXB2REQ    : 1;
    unsigned TX2IF      : 1;
  };
  uint8_t ctrl_status;
}ctrl_status_t;

typedef union{
  struct{
    unsigned filter     : 3;
    unsigned msgType    : 2;
    unsigned unusedBit  : 1;
    unsigned rxBuffer   : 2;
  };
  uint8_t ctrl_rx_status;
}ctrl_rx_status_t;

typedef union{
  struct{
    unsigned EWARN      :1;
    unsigned RXWAR      :1;
    unsigned TXWAR      :1;
    unsigned RXEP       :1;
    unsigned TXEP       :1;
    unsigned TXBO       :1;
    unsigned RX0OVR     :1;
    unsigned RX1OVR     :1;
  };
  uint8_t error_flag_reg;
}ctrl_error_status_t;

#define CAN_MAX_DLEN 8

typedef union{
  struct{
    uint8_t RXBnSIDH;
    uint8_t RXBnSIDL;
    uint8_t RXBnEID8;
    uint8_t RXBnEID0;
    uint8_t RXBnDLC;
    uint8_t RXBnD0;
    uint8_t RXBnD1;
    uint8_t RXBnD2;
    uint8_t RXBnD3;
    uint8_t RXBnD4;
    uint8_t RXBnD5;
    uint8_t RXBnD6;
    uint8_t RXBnD7;
  };
  uint8_t rx_reg_array[13];
}rx_reg_t;

/* MCP2515 Registers */
typedef struct{
  uint8_t RXF0SIDH;
  uint8_t RXF0SIDL;
  uint8_t RXF0EID8;
  uint8_t RXF0EID0;
}RXF0;

typedef struct{
  uint8_t RXF1SIDH;
  uint8_t RXF1SIDL;
  uint8_t RXF1EID8;
  uint8_t RXF1EID0;
}RXF1;

typedef struct{
  uint8_t RXF2SIDH;
  uint8_t RXF2SIDL;
  uint8_t RXF2EID8;
  uint8_t RXF2EID0;
}RXF2;

typedef struct{
  uint8_t RXF3SIDH;
  uint8_t RXF3SIDL;
  uint8_t RXF3EID8;
  uint8_t RXF3EID0;
}RXF3;

typedef struct{
  uint8_t RXF4SIDH;
  uint8_t RXF4SIDL;
  uint8_t RXF4EID8;
  uint8_t RXF4EID0;
}RXF4;

typedef struct{
  uint8_t RXF5SIDH;
  uint8_t RXF5SIDL;
  uint8_t RXF5EID8;
  uint8_t RXF5EID0;
}RXF5;

typedef struct{
  uint8_t RXM0SIDH;
  uint8_t RXM0SIDL;
  uint8_t RXM0EID8;
  uint8_t RXM0EID0;
}RXM0;

typedef struct{
  uint8_t RXM1SIDH;
  uint8_t RXM1SIDL;
  uint8_t RXM1EID8;
  uint8_t RXM1EID0;
}RXM1;

typedef struct{
  uint8_t tempSIDH;
  uint8_t tempSIDL;
  uint8_t tempEID8;
  uint8_t tempEID0;
}id_reg_t;

typedef enum {
	MCP_20MHZ, MCP_16MHZ, MCP_8MHZ
} CAN_CLOCK;

typedef enum {
	CAN_5KBPS,
	CAN_10KBPS,
	CAN_20KBPS,
	CAN_31K25BPS,
	CAN_33KBPS,
	CAN_40KBPS,
	CAN_50KBPS,
	CAN_80KBPS,
	CAN_83K3BPS,
	CAN_95KBPS,
	CAN_100KBPS,
	CAN_125KBPS,
	CAN_200KBPS,
	CAN_250KBPS,
	CAN_500KBPS,
	CAN_1000KBPS
} CAN_SPEED;

/* Functions */
bool MCP2515_Initialize(uint8_t CS);
bool MCP2515_SetConfigMode(uint8_t CS);
bool MCP2515_SetNormalMode(uint8_t CS);
bool MCP2515_SetSleepMode(uint8_t CS);
void MCP2515_Reset(uint8_t CS);
bool MCP2515_SetBitrateClock(CAN_SPEED canSpeed, CAN_CLOCK canClock, uint8_t CS);
uint8_t MCP2515_ReadByte (uint8_t address, uint8_t CS);
void MCP2515_ReadRxSequence(uint8_t instruction, uint8_t *data, uint8_t length, uint8_t CS);
void MCP2515_WriteByte(uint8_t address, uint8_t data, uint8_t CS);
void MCP2515_WriteByteSequence(uint8_t startAddress, uint8_t endAddress, uint8_t *data, uint8_t CS);
void MCP2515_LoadTxSequence(uint8_t instruction, uint8_t *idReg, uint8_t dlc, uint8_t *data, uint8_t CS);
void MCP2515_LoadTxBuffer(uint8_t instruction, uint8_t data, uint8_t CS);
void MCP2515_RequestToSend(uint8_t instruction, uint8_t CS);
uint8_t MCP2515_ReadStatus(uint8_t CS);
uint8_t MCP2515_GetRxStatus(uint8_t CS);
void MCP2515_BitModify(uint8_t address, uint8_t mask, uint8_t data, uint8_t CS);

#endif
