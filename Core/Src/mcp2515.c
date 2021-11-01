#include "MCP2515.h"

/* Required to be modified according to the PIN setting. Modify below items for your SPI configurations */
extern SPI_HandleTypeDef        hspi2;
#define SPI_CAN                 &hspi2
#define SPI_TIMEOUT             10

void MCP2515_CS_LOW(uint8_t CS) {
	switch(CS)
	{
		case 0:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET);
			break;
		case 1:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_RESET);
			break;
	}

}

void MCP2515_CS_HIGH(uint8_t CS) {
	switch(CS)
	{
		case 0:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_SET);
			break;
		case 1:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_SET);
			break;
	}
}

/* Prototypes */
static void SPI_Tx(uint8_t data);
static void SPI_TxBuffer(uint8_t *buffer, uint8_t length);
static uint8_t SPI_Rx(void);
static void SPI_RxBuffer(uint8_t *buffer, uint8_t length);

/* MCP2515 initializing via SPI */
bool MCP2515_Initialize(uint8_t CS)
{
  MCP2515_CS_HIGH(CS);

  uint8_t loop = 10;

  do {
    /* Checking if SPI is ready */
    if(HAL_SPI_GetState(SPI_CAN) == HAL_SPI_STATE_READY)
      return true;

    loop--;
  } while(loop > 0);

  return false;
}

/* Setting MCP2515 in Configuration Mode */
bool MCP2515_SetConfigMode(uint8_t CS)
{
  /* CANCTRL Register Configuration mode setting */
  MCP2515_WriteByte(MCP2515_CANCTRL, 0x80, CS);

  uint8_t loop = 10;

  do {
    /* Mode change confirmation */
    if((MCP2515_ReadByte(MCP2515_CANSTAT, CS) & 0xE0) == 0x80)
      return true;

    loop--;
  } while(loop > 0);

  return false;
}

/* Setting MCP2515 in Normal Mode */
bool MCP2515_SetNormalMode(uint8_t CS)
{
  /* CANCTRL Register Normal mode setting */
  MCP2515_WriteByte(MCP2515_CANCTRL, 0x00, CS);

  uint8_t loop = 10;

  do {
    /* Mode change confirmation */
    if((MCP2515_ReadByte(MCP2515_CANSTAT, CS) & 0xE0) == 0x00)
      return true;

    loop--;
  } while(loop > 0);

  return false;
}

/* Setting MCP2515 in Sleep Mode */
bool MCP2515_SetSleepMode(uint8_t CS)
{
  /* CANCTRL Register Sleep mode setting */
  MCP2515_WriteByte(MCP2515_CANCTRL, 0x20, CS);

  uint8_t loop = 10;

  do {
    /* Mode change confirmation */
    if((MCP2515_ReadByte(MCP2515_CANSTAT, CS) & 0xE0) == 0x20)
      return true;

    loop--;
  } while(loop > 0);

  return false;
}

/* MCP2515 SPI-Reset */
void MCP2515_Reset(uint8_t CS)
{
  MCP2515_CS_LOW(CS);

  SPI_Tx(MCP2515_RESET);

  MCP2515_CS_HIGH(CS);
}

/* Setting MCP2515 Clock and Bitrate */
bool MCP2515_SetBitrateClock(CAN_SPEED canSpeed, CAN_CLOCK canClock, uint8_t CS)
{
    uint8_t set, cnf1, cnf2, cnf3;
    set = 1;
    switch (canClock)
    {
        case (MCP_8MHZ):
        switch (canSpeed)
        {
            case (CAN_5KBPS):                                               //   5KBPS
            cnf1 = MCP_8MHz_5kBPS_CFG1;
            cnf2 = MCP_8MHz_5kBPS_CFG2;
            cnf3 = MCP_8MHz_5kBPS_CFG3;
            break;

            case (CAN_10KBPS):                                              //  10KBPS
            cnf1 = MCP_8MHz_10kBPS_CFG1;
            cnf2 = MCP_8MHz_10kBPS_CFG2;
            cnf3 = MCP_8MHz_10kBPS_CFG3;
            break;

            case (CAN_20KBPS):                                              //  20KBPS
            cnf1 = MCP_8MHz_20kBPS_CFG1;
            cnf2 = MCP_8MHz_20kBPS_CFG2;
            cnf3 = MCP_8MHz_20kBPS_CFG3;
            break;

            case (CAN_31K25BPS):                                            //  31.25KBPS
            cnf1 = MCP_8MHz_31k25BPS_CFG1;
            cnf2 = MCP_8MHz_31k25BPS_CFG2;
            cnf3 = MCP_8MHz_31k25BPS_CFG3;
            break;

            case (CAN_33KBPS):                                              //  33.333KBPS
            cnf1 = MCP_8MHz_33k3BPS_CFG1;
            cnf2 = MCP_8MHz_33k3BPS_CFG2;
            cnf3 = MCP_8MHz_33k3BPS_CFG3;
            break;

            case (CAN_40KBPS):                                              //  40Kbps
            cnf1 = MCP_8MHz_40kBPS_CFG1;
            cnf2 = MCP_8MHz_40kBPS_CFG2;
            cnf3 = MCP_8MHz_40kBPS_CFG3;
            break;

            case (CAN_50KBPS):                                              //  50Kbps
            cnf1 = MCP_8MHz_50kBPS_CFG1;
            cnf2 = MCP_8MHz_50kBPS_CFG2;
            cnf3 = MCP_8MHz_50kBPS_CFG3;
            break;

            case (CAN_80KBPS):                                              //  80Kbps
            cnf1 = MCP_8MHz_80kBPS_CFG1;
            cnf2 = MCP_8MHz_80kBPS_CFG2;
            cnf3 = MCP_8MHz_80kBPS_CFG3;
            break;

            case (CAN_100KBPS):                                             // 100Kbps
            cnf1 = MCP_8MHz_100kBPS_CFG1;
            cnf2 = MCP_8MHz_100kBPS_CFG2;
            cnf3 = MCP_8MHz_100kBPS_CFG3;
            break;

            case (CAN_125KBPS):                                             // 125Kbps
            cnf1 = MCP_8MHz_125kBPS_CFG1;
            cnf2 = MCP_8MHz_125kBPS_CFG2;
            cnf3 = MCP_8MHz_125kBPS_CFG3;
            break;

            case (CAN_200KBPS):                                             // 200Kbps
            cnf1 = MCP_8MHz_200kBPS_CFG1;
            cnf2 = MCP_8MHz_200kBPS_CFG2;
            cnf3 = MCP_8MHz_200kBPS_CFG3;
            break;

            case (CAN_250KBPS):                                             // 250Kbps
            cnf1 = MCP_8MHz_250kBPS_CFG1;
            cnf2 = MCP_8MHz_250kBPS_CFG2;
            cnf3 = MCP_8MHz_250kBPS_CFG3;
            break;

            case (CAN_500KBPS):                                             // 500Kbps
            cnf1 = MCP_8MHz_500kBPS_CFG1;
            cnf2 = MCP_8MHz_500kBPS_CFG2;
            cnf3 = MCP_8MHz_500kBPS_CFG3;
            break;

            case (CAN_1000KBPS):                                            //   1Mbps
            cnf1 = MCP_8MHz_1000kBPS_CFG1;
            cnf2 = MCP_8MHz_1000kBPS_CFG2;
            cnf3 = MCP_8MHz_1000kBPS_CFG3;
            break;

            default:
            set = 0;
            break;
        }
        break;

        case (MCP_16MHZ):
        switch (canSpeed)
        {
            case (CAN_5KBPS):                                               //   5Kbps
            cnf1 = MCP_16MHz_5kBPS_CFG1;
            cnf2 = MCP_16MHz_5kBPS_CFG2;
            cnf3 = MCP_16MHz_5kBPS_CFG3;
            break;

            case (CAN_10KBPS):                                              //  10Kbps
            cnf1 = MCP_16MHz_10kBPS_CFG1;
            cnf2 = MCP_16MHz_10kBPS_CFG2;
            cnf3 = MCP_16MHz_10kBPS_CFG3;
            break;

            case (CAN_20KBPS):                                              //  20Kbps
            cnf1 = MCP_16MHz_20kBPS_CFG1;
            cnf2 = MCP_16MHz_20kBPS_CFG2;
            cnf3 = MCP_16MHz_20kBPS_CFG3;
            break;

            case (CAN_33KBPS):                                              //  33.333Kbps
            cnf1 = MCP_16MHz_33k3BPS_CFG1;
            cnf2 = MCP_16MHz_33k3BPS_CFG2;
            cnf3 = MCP_16MHz_33k3BPS_CFG3;
            break;

            case (CAN_40KBPS):                                              //  40Kbps
            cnf1 = MCP_16MHz_40kBPS_CFG1;
            cnf2 = MCP_16MHz_40kBPS_CFG2;
            cnf3 = MCP_16MHz_40kBPS_CFG3;
            break;

            case (CAN_50KBPS):                                              //  50Kbps
            cnf1 = MCP_16MHz_50kBPS_CFG1;
            cnf2 = MCP_16MHz_50kBPS_CFG2;
            cnf3 = MCP_16MHz_50kBPS_CFG3;
            break;

            case (CAN_80KBPS):                                              //  80Kbps
            cnf1 = MCP_16MHz_80kBPS_CFG1;
            cnf2 = MCP_16MHz_80kBPS_CFG2;
            cnf3 = MCP_16MHz_80kBPS_CFG3;
            break;

            case (CAN_83K3BPS):                                             //  83.333Kbps
            cnf1 = MCP_16MHz_83k3BPS_CFG1;
            cnf2 = MCP_16MHz_83k3BPS_CFG2;
            cnf3 = MCP_16MHz_83k3BPS_CFG3;
            break;

            case (CAN_100KBPS):                                             // 100Kbps
            cnf1 = MCP_16MHz_100kBPS_CFG1;
            cnf2 = MCP_16MHz_100kBPS_CFG2;
            cnf3 = MCP_16MHz_100kBPS_CFG3;
            break;

            case (CAN_125KBPS):                                             // 125Kbps
            cnf1 = MCP_16MHz_125kBPS_CFG1;
            cnf2 = MCP_16MHz_125kBPS_CFG2;
            cnf3 = MCP_16MHz_125kBPS_CFG3;
            break;

            case (CAN_200KBPS):                                             // 200Kbps
		    cnf1 = MCP_16MHz_200kBPS_CFG1;
            cnf2 = MCP_16MHz_200kBPS_CFG2;
            cnf3 = MCP_16MHz_200kBPS_CFG3;
            break;

            case (CAN_250KBPS):                                             // 250Kbps
            cnf1 = MCP_16MHz_250kBPS_CFG1;
            cnf2 = MCP_16MHz_250kBPS_CFG2;
            cnf3 = MCP_16MHz_250kBPS_CFG3;
            break;

            case (CAN_500KBPS):                                             // 500Kbps
            cnf1 = MCP_16MHz_500kBPS_CFG1;
            cnf2 = MCP_16MHz_500kBPS_CFG2;
            cnf3 = MCP_16MHz_500kBPS_CFG3;
            break;

            case (CAN_1000KBPS):                                            //   1Mbps
            cnf1 = MCP_16MHz_1000kBPS_CFG1;
            cnf2 = MCP_16MHz_1000kBPS_CFG2;
            cnf3 = MCP_16MHz_1000kBPS_CFG3;
            break;

            default:
            set = 0;
            break;
        }
        break;

        case (MCP_20MHZ):
        switch (canSpeed)
        {
            case (CAN_33KBPS):                                              //  33.333Kbps
			cnf1 = MCP_20MHz_33k3BPS_CFG1;
            cnf2 = MCP_20MHz_33k3BPS_CFG2;
            cnf3 = MCP_20MHz_33k3BPS_CFG3;
	    break;

            case (CAN_40KBPS):                                              //  40Kbps
            cnf1 = MCP_20MHz_40kBPS_CFG1;
            cnf2 = MCP_20MHz_40kBPS_CFG2;
            cnf3 = MCP_20MHz_40kBPS_CFG3;
            break;

            case (CAN_50KBPS):                                              //  50Kbps
            cnf1 = MCP_20MHz_50kBPS_CFG1;
            cnf2 = MCP_20MHz_50kBPS_CFG2;
            cnf3 = MCP_20MHz_50kBPS_CFG3;
            break;

            case (CAN_80KBPS):                                              //  80Kbps
            cnf1 = MCP_20MHz_80kBPS_CFG1;
            cnf2 = MCP_20MHz_80kBPS_CFG2;
            cnf3 = MCP_20MHz_80kBPS_CFG3;
            break;

            case (CAN_83K3BPS):                                             //  83.333Kbps
            cnf1 = MCP_20MHz_83k3BPS_CFG1;
            cnf2 = MCP_20MHz_83k3BPS_CFG2;
            cnf3 = MCP_20MHz_83k3BPS_CFG3;
	    break;

            case (CAN_100KBPS):                                             // 100Kbps
            cnf1 = MCP_20MHz_100kBPS_CFG1;
            cnf2 = MCP_20MHz_100kBPS_CFG2;
            cnf3 = MCP_20MHz_100kBPS_CFG3;
            break;

            case (CAN_125KBPS):                                             // 125Kbps
            cnf1 = MCP_20MHz_125kBPS_CFG1;
            cnf2 = MCP_20MHz_125kBPS_CFG2;
            cnf3 = MCP_20MHz_125kBPS_CFG3;
            break;

            case (CAN_200KBPS):                                             // 200Kbps
            cnf1 = MCP_20MHz_200kBPS_CFG1;
            cnf2 = MCP_20MHz_200kBPS_CFG2;
            cnf3 = MCP_20MHz_200kBPS_CFG3;
            break;

            case (CAN_250KBPS):                                             // 250Kbps
            cnf1 = MCP_20MHz_250kBPS_CFG1;
            cnf2 = MCP_20MHz_250kBPS_CFG2;
            cnf3 = MCP_20MHz_250kBPS_CFG3;
            break;

            case (CAN_500KBPS):                                             // 500Kbps
            cnf1 = MCP_20MHz_500kBPS_CFG1;
            cnf2 = MCP_20MHz_500kBPS_CFG2;
            cnf3 = MCP_20MHz_500kBPS_CFG3;
            break;

            case (CAN_1000KBPS):                                            //   1Mbps
            cnf1 = MCP_20MHz_1000kBPS_CFG1;
            cnf2 = MCP_20MHz_1000kBPS_CFG2;
            cnf3 = MCP_20MHz_1000kBPS_CFG3;
            break;

            default:
            set = 0;
            break;
        }
        break;

        default:
        set = 0;
        break;
    }

    if (set) {
    	MCP2515_WriteByte(MCP2515_CNF1, cnf1, CS);
    	MCP2515_WriteByte(MCP2515_CNF2, cnf2, CS);
    	MCP2515_WriteByte(MCP2515_CNF3, cnf3, CS);
        return true;
    }
    else {
        return false;
    }
}

/* Reading of one byte */
uint8_t MCP2515_ReadByte (uint8_t address, uint8_t CS)
{
  uint8_t retVal;

  MCP2515_CS_LOW(CS);

  SPI_Tx(MCP2515_READ);
  SPI_Tx(address);
  retVal = SPI_Rx();

  MCP2515_CS_HIGH(CS);

  return retVal;
}

/* Reading Sequential Bytes */
void MCP2515_ReadRxSequence(uint8_t instruction, uint8_t *data, uint8_t length, uint8_t CS)
{
  MCP2515_CS_LOW(CS);

  SPI_Tx(instruction);
  SPI_RxBuffer(data, length);

  MCP2515_CS_HIGH(CS);
}

/* Writing 1 byte */
void MCP2515_WriteByte(uint8_t address, uint8_t data, uint8_t CS)
{
  MCP2515_CS_LOW(CS);

  SPI_Tx(MCP2515_WRITE);
  SPI_Tx(address);
  SPI_Tx(data);

  MCP2515_CS_HIGH(CS);
}

/* Writing Sequential Bytes */
void MCP2515_WriteByteSequence(uint8_t startAddress, uint8_t endAddress, uint8_t *data, uint8_t CS)
{
  MCP2515_CS_LOW(CS);

  SPI_Tx(MCP2515_WRITE);
  SPI_Tx(startAddress);
  SPI_TxBuffer(data, (endAddress - startAddress + 1));

  MCP2515_CS_HIGH(CS);
}

/* Writing Sequential Bytes to TxBuffer */
void MCP2515_LoadTxSequence(uint8_t instruction, uint8_t *idReg, uint8_t dlc, uint8_t *data, uint8_t CS)
{
  MCP2515_CS_LOW(CS);

  SPI_Tx(instruction);
  SPI_TxBuffer(idReg, 4);
  SPI_Tx(dlc);
  SPI_TxBuffer(data, dlc);

  MCP2515_CS_HIGH(CS);
}

/* Writing 1 Byte to TxBuffer */
void MCP2515_LoadTxBuffer(uint8_t instruction, uint8_t data, uint8_t CS)
{
  MCP2515_CS_LOW(CS);

  SPI_Tx(instruction);
  SPI_Tx(data);

  MCP2515_CS_HIGH(CS);
}

/* TxBuffer transfer via RTS command */
void MCP2515_RequestToSend(uint8_t instruction, uint8_t CS)
{
  MCP2515_CS_LOW(CS);

  SPI_Tx(instruction);

  MCP2515_CS_HIGH(CS);
}

/* Reading MCP2515 Status */
uint8_t MCP2515_ReadStatus(uint8_t CS)
{
  uint8_t retVal;

  MCP2515_CS_LOW(CS);

  SPI_Tx(MCP2515_READ_STATUS);
  retVal = SPI_Rx();

  MCP2515_CS_HIGH(CS);

  return retVal;
}

/* Reading MCP2515 RxStatus */
uint8_t MCP2515_GetRxStatus(uint8_t CS)
{
  uint8_t retVal;

  MCP2515_CS_LOW(CS);

  SPI_Tx(MCP2515_RX_STATUS);
  retVal = SPI_Rx();

  MCP2515_CS_HIGH(CS);

  return retVal;
}

/* Changing register value */
void MCP2515_BitModify(uint8_t address, uint8_t mask, uint8_t data, uint8_t CS)
{
  MCP2515_CS_LOW(CS);

  SPI_Tx(MCP2515_BIT_MOD);
  SPI_Tx(address);
  SPI_Tx(mask);
  SPI_Tx(data);

  MCP2515_CS_HIGH(CS);
}

/* SPI 1 Byte Tx Wrapper Function */
static void SPI_Tx(uint8_t data)
{
  HAL_SPI_Transmit(SPI_CAN, &data, 1, SPI_TIMEOUT);
}

/* SPI Buffer Tx Wrapper Function */
static void SPI_TxBuffer(uint8_t *buffer, uint8_t length)
{
  HAL_SPI_Transmit(SPI_CAN, buffer, length, SPI_TIMEOUT);
}

/* SPI 1 Byte Rx Wrapper Function */
static uint8_t SPI_Rx(void)
{
  uint8_t retVal;
  HAL_SPI_Receive(SPI_CAN, &retVal, 1, SPI_TIMEOUT);
  return retVal;
}

/* SPI Buffer Rx Wrapper Function */
static void SPI_RxBuffer(uint8_t *buffer, uint8_t length)
{
  HAL_SPI_Receive(SPI_CAN, buffer, length, SPI_TIMEOUT);
}
