#include "CANSPI.h"
#include "MCP2515.h"

/** Local Function Prototypes */
static uint32_t convertReg2ExtendedCANid(uint8_t tempRXBn_EIDH, uint8_t tempRXBn_EIDL, uint8_t tempRXBn_SIDH, uint8_t tempRXBn_SIDL);
static uint32_t convertReg2StandardCANid(uint8_t tempRXBn_SIDH, uint8_t tempRXBn_SIDL) ;
static void convertCANid2Reg(uint32_t tempPassedInID, uint8_t canIdType, id_reg_t *passedIdReg);

/** Local Variables */
ctrl_status_t ctrlStatus;
ctrl_error_status_t errorStatus;
id_reg_t idReg;

/** CAN SPI APIs */

/* Entering sleep mode */
void CANSPI_Sleep(uint8_t CS)
{
  /* Clear CAN bus wakeup interrupt */
  MCP2515_BitModify(MCP2515_CANINTF, 0x40, 0x00, CS);

  /* Enable CAN bus activity wakeup */
  MCP2515_BitModify(MCP2515_CANINTE, 0x40, 0x40, CS);

  MCP2515_SetSleepMode(CS);
}

/* CAN initializing */
bool CANSPI_Initialize(uint8_t CS)
{
  RXF0 RXF0reg;
  RXF1 RXF1reg;
  RXF2 RXF2reg;
  RXF3 RXF3reg;
  RXF4 RXF4reg;
  RXF5 RXF5reg;
  RXM0 RXM0reg;
  RXM1 RXM1reg;

  switch(CS)
  {

  case MCP2515_FIRST:

	  /* Rx Mask values */
	  RXM0reg.RXM0SIDH = 0xFF;
	  RXM0reg.RXM0SIDL = 0xE0;
	  RXM0reg.RXM0EID8 = 0x00;
	  RXM0reg.RXM0EID0 = 0x00;

	  RXM1reg.RXM1SIDH = 0xFF;
	  RXM1reg.RXM1SIDL = 0xE0;
	  RXM1reg.RXM1EID8 = 0x00;
	  RXM1reg.RXM1EID0 = 0x00;

	  /* Rx Filter values */
	  RXF0reg.RXF0SIDH = 0x82;
	  RXF0reg.RXF0SIDL = 0x40;      //Starndard Filter
	  RXF0reg.RXF0EID8 = 0x00;
	  RXF0reg.RXF0EID0 = 0x00;

	  RXF1reg.RXF1SIDH = 0x82;
	  RXF1reg.RXF1SIDL = 0x40;      //Exntended Filter
	  RXF1reg.RXF1EID8 = 0x00;
	  RXF1reg.RXF1EID0 = 0x00;

	  RXF2reg.RXF2SIDH = 0x00;
	  RXF2reg.RXF2SIDL = 0x00;
	  RXF2reg.RXF2EID8 = 0x00;
	  RXF2reg.RXF2EID0 = 0x00;

	  RXF3reg.RXF3SIDH = 0x00;
	  RXF3reg.RXF3SIDL = 0x00;
	  RXF3reg.RXF3EID8 = 0x00;
	  RXF3reg.RXF3EID0 = 0x00;

	  RXF4reg.RXF4SIDH = 0x00;
	  RXF4reg.RXF4SIDL = 0x00;
	  RXF4reg.RXF4EID8 = 0x00;
	  RXF4reg.RXF4EID0 = 0x00;

	  RXF5reg.RXF5SIDH = 0x00;
	  RXF5reg.RXF5SIDL = 0x00;
	  RXF5reg.RXF5EID8 = 0x00;
	  RXF5reg.RXF5EID0 = 0x00;

	  break;
  case MCP2515_SECOND:

	  /* Rx Mask values */
	  RXM0reg.RXM0SIDH = 0xFF;
	  RXM0reg.RXM0SIDL = 0xE0;
	  RXM0reg.RXM0EID8 = 0x00;
	  RXM0reg.RXM0EID0 = 0x00;

	  RXM1reg.RXM1SIDH = 0xFF;
	  RXM1reg.RXM1SIDL = 0xE0;
	  RXM1reg.RXM1EID8 = 0x00;
	  RXM1reg.RXM1EID0 = 0x00;

	  /* Rx Filter values */
	  RXF0reg.RXF0SIDH = 0x4B;
	  RXF0reg.RXF0SIDL = 0x00;      //Starndard Filter
	  RXF0reg.RXF0EID8 = 0x00;
	  RXF0reg.RXF0EID0 = 0x00;

	  RXF1reg.RXF1SIDH = 0x4B;
	  RXF1reg.RXF1SIDL = 0x00;      //Exntended Filter
	  RXF1reg.RXF1EID8 = 0x00;
	  RXF1reg.RXF1EID0 = 0x00;

	  RXF2reg.RXF2SIDH = 0x00;
	  RXF2reg.RXF2SIDL = 0x00;
	  RXF2reg.RXF2EID8 = 0x00;
	  RXF2reg.RXF2EID0 = 0x00;

	  RXF3reg.RXF3SIDH = 0x00;
	  RXF3reg.RXF3SIDL = 0x00;
	  RXF3reg.RXF3EID8 = 0x00;
	  RXF3reg.RXF3EID0 = 0x00;

	  RXF4reg.RXF4SIDH = 0x00;
	  RXF4reg.RXF4SIDL = 0x00;
	  RXF4reg.RXF4EID8 = 0x00;
	  RXF4reg.RXF4EID0 = 0x00;

	  RXF5reg.RXF5SIDH = 0x00;
	  RXF5reg.RXF5SIDL = 0x00;
	  RXF5reg.RXF5EID8 = 0x00;
	  RXF5reg.RXF5EID0 = 0x00;
	  break;
  }


  /* MCP2515 initialization, SPI communication status check */
  if(!MCP2515_Initialize(CS))
    return false;

  /* Configuration mode setting */
  if(!MCP2515_SetConfigMode(CS))
    return false;

  /* Set Filter & Mask values */
  MCP2515_WriteByteSequence(MCP2515_RXM0SIDH, MCP2515_RXM0EID0, &(RXM0reg.RXM0SIDH), CS);
  MCP2515_WriteByteSequence(MCP2515_RXM1SIDH, MCP2515_RXM1EID0, &(RXM1reg.RXM1SIDH), CS);
  MCP2515_WriteByteSequence(MCP2515_RXF0SIDH, MCP2515_RXF0EID0, &(RXF0reg.RXF0SIDH), CS);
  MCP2515_WriteByteSequence(MCP2515_RXF1SIDH, MCP2515_RXF1EID0, &(RXF1reg.RXF1SIDH), CS);
  MCP2515_WriteByteSequence(MCP2515_RXF2SIDH, MCP2515_RXF2EID0, &(RXF2reg.RXF2SIDH), CS);
  MCP2515_WriteByteSequence(MCP2515_RXF3SIDH, MCP2515_RXF3EID0, &(RXF3reg.RXF3SIDH), CS);
  MCP2515_WriteByteSequence(MCP2515_RXF4SIDH, MCP2515_RXF4EID0, &(RXF4reg.RXF4SIDH), CS);
  MCP2515_WriteByteSequence(MCP2515_RXF5SIDH, MCP2515_RXF5EID0, &(RXF5reg.RXF5SIDH), CS);

  /* Accept Valid Messages (Standard + Extended) */
  MCP2515_WriteByte(MCP2515_RXB0CTRL, 0x04, CS);    //Enable BUKT, Accept Filter 0
  MCP2515_WriteByte(MCP2515_RXB1CTRL, 0x00, CS);    //Accept Filter 0

  MCP2515_SetBitrateClock(CAN_1000KBPS, MCP_8MHZ, CS);

  /* Normal mode setting*/
  if(!MCP2515_SetNormalMode(CS))
    return false;

  return true;
}

/* CAN transmitting via SPI */
uint8_t CANSPI_Transmit(uCAN_MSG *tempCanMsg, uint8_t CS)
{
  uint8_t returnValue = 0;

  idReg.tempSIDH = 0;
  idReg.tempSIDL = 0;
  idReg.tempEID8 = 0;
  idReg.tempEID0 = 0;

  ctrlStatus.ctrl_status = MCP2515_ReadStatus(CS);

  /* Transmission of the buffer without currently pending transmission */
  if (ctrlStatus.TXB0REQ == 0)
  {
    /* Convert to ID Type */
    convertCANid2Reg(tempCanMsg->frame.id, tempCanMsg->frame.idType, &idReg);

    /* Loading data to send to Tx Buffer */
    MCP2515_LoadTxSequence(MCP2515_LOAD_TXB0SIDH, &(idReg.tempSIDH), tempCanMsg->frame.dlc, &(tempCanMsg->frame.data0), CS);

    /* Tx Buffer data transfer request*/
    MCP2515_RequestToSend(MCP2515_RTS_TX0, CS);

    returnValue = 1;
  }
  else if (ctrlStatus.TXB1REQ == 0)
  {
    convertCANid2Reg(tempCanMsg->frame.id, tempCanMsg->frame.idType, &idReg);

    MCP2515_LoadTxSequence(MCP2515_LOAD_TXB1SIDH, &(idReg.tempSIDH), tempCanMsg->frame.dlc, &(tempCanMsg->frame.data0), CS);

    MCP2515_RequestToSend(MCP2515_RTS_TX1, CS);

    returnValue = 1;
  }
  else if (ctrlStatus.TXB2REQ == 0)
  {
    convertCANid2Reg(tempCanMsg->frame.id, tempCanMsg->frame.idType, &idReg);

    MCP2515_LoadTxSequence(MCP2515_LOAD_TXB2SIDH, &(idReg.tempSIDH), tempCanMsg->frame.dlc, &(tempCanMsg->frame.data0), CS);

    MCP2515_RequestToSend(MCP2515_RTS_TX2, CS);

    returnValue = 1;
  }

  return (returnValue);
}

/* Receiving CAN messages */
uint8_t CANSPI_Receive(uCAN_MSG *tempCanMsg, uint8_t CS)
{
  uint8_t returnValue = 0;
  rx_reg_t rxReg;
  ctrl_rx_status_t rxStatus;

  rxStatus.ctrl_rx_status = MCP2515_GetRxStatus(CS);

  /* Checking if there are received messages in the buffer */
  if (rxStatus.rxBuffer != 0)
  {
    /* Checking which buffer contains the message and reading it */
    if ((rxStatus.rxBuffer == MSG_IN_RXB0)|(rxStatus.rxBuffer == MSG_IN_BOTH_BUFFERS))
    {
      MCP2515_ReadRxSequence(MCP2515_READ_RXB0SIDH, rxReg.rx_reg_array, sizeof(rxReg.rx_reg_array), CS);
    }
    else if (rxStatus.rxBuffer == MSG_IN_RXB1)
    {
      MCP2515_ReadRxSequence(MCP2515_READ_RXB1SIDH, rxReg.rx_reg_array, sizeof(rxReg.rx_reg_array), CS);
    }

    /* Extended type */
    if (rxStatus.msgType == dEXTENDED_CAN_MSG_ID_2_0B)
    {
      tempCanMsg->frame.idType = (uint8_t) dEXTENDED_CAN_MSG_ID_2_0B;
      tempCanMsg->frame.id = convertReg2ExtendedCANid(rxReg.RXBnEID8, rxReg.RXBnEID0, rxReg.RXBnSIDH, rxReg.RXBnSIDL);
    }
    else
    {
      /* Standard type */
      tempCanMsg->frame.idType = (uint8_t) dSTANDARD_CAN_MSG_ID_2_0B;
      tempCanMsg->frame.id = convertReg2StandardCANid(rxReg.RXBnSIDH, rxReg.RXBnSIDL);
    }

    tempCanMsg->frame.dlc   = rxReg.RXBnDLC;
    tempCanMsg->frame.data0 = rxReg.RXBnD0;
    tempCanMsg->frame.data1 = rxReg.RXBnD1;
    tempCanMsg->frame.data2 = rxReg.RXBnD2;
    tempCanMsg->frame.data3 = rxReg.RXBnD3;
    tempCanMsg->frame.data4 = rxReg.RXBnD4;
    tempCanMsg->frame.data5 = rxReg.RXBnD5;
    tempCanMsg->frame.data6 = rxReg.RXBnD6;
    tempCanMsg->frame.data7 = rxReg.RXBnD7;

    returnValue = 1;
  }

  return (returnValue);
}

/* Checking if there are any messages in the receive buffer */
uint8_t CANSPI_messagesInBuffer(uint8_t CS)
{
  uint8_t messageCount = 0;

  ctrlStatus.ctrl_status = MCP2515_ReadStatus(CS);

  if(ctrlStatus.RX0IF != 0)
  {
    messageCount++;
  }

  if(ctrlStatus.RX1IF != 0)
  {
    messageCount++;
  }

  return (messageCount);
}

/* Checking if CAN Bus is Offline */
uint8_t CANSPI_isBussOff(uint8_t CS)
{
  uint8_t returnValue = 0;

  errorStatus.error_flag_reg = MCP2515_ReadByte(MCP2515_EFLG, CS);

  if(errorStatus.TXBO == 1)
  {
    returnValue = 1;
  }

  return (returnValue);
}

/* Checking if Rx Passive Error status */
uint8_t CANSPI_isRxErrorPassive(uint8_t CS)
{
  uint8_t returnValue = 0;

  errorStatus.error_flag_reg = MCP2515_ReadByte(MCP2515_EFLG, CS);

  if(errorStatus.RXEP == 1)
  {
    returnValue = 1;
  }

  return (returnValue);
}

/* Checking if Tx Passive Error status */
uint8_t CANSPI_isTxErrorPassive(uint8_t CS)
{
  uint8_t returnValue = 0;

  errorStatus.error_flag_reg = MCP2515_ReadByte(MCP2515_EFLG, CS);

  if(errorStatus.TXEP == 1)
  {
    returnValue = 1;
  }

  return (returnValue);
}

/* A function to convert the register stored value to the Extended ID type */
static uint32_t convertReg2ExtendedCANid(uint8_t tempRXBn_EIDH, uint8_t tempRXBn_EIDL, uint8_t tempRXBn_SIDH, uint8_t tempRXBn_SIDL)
{
  uint32_t returnValue = 0;
  uint32_t ConvertedID = 0;
  uint8_t CAN_standardLo_ID_lo2bits;
  uint8_t CAN_standardLo_ID_hi3bits;

  CAN_standardLo_ID_lo2bits = (tempRXBn_SIDL & 0x03);
  CAN_standardLo_ID_hi3bits = (tempRXBn_SIDL >> 5);
  ConvertedID = (tempRXBn_SIDH << 3);
  ConvertedID = ConvertedID + CAN_standardLo_ID_hi3bits;
  ConvertedID = (ConvertedID << 2);
  ConvertedID = ConvertedID + CAN_standardLo_ID_lo2bits;
  ConvertedID = (ConvertedID << 8);
  ConvertedID = ConvertedID + tempRXBn_EIDH;
  ConvertedID = (ConvertedID << 8);
  ConvertedID = ConvertedID + tempRXBn_EIDL;
  returnValue = ConvertedID;
  return (returnValue);
}

/* A function to convert the register stored value to the Standard ID type */
static uint32_t convertReg2StandardCANid(uint8_t tempRXBn_SIDH, uint8_t tempRXBn_SIDL)
{
  uint32_t returnValue = 0;
  uint32_t ConvertedID;

  ConvertedID = (tempRXBn_SIDH << 3);
  ConvertedID = ConvertedID + (tempRXBn_SIDL >> 5);
  returnValue = ConvertedID;

  return (returnValue);
}

/* Conversion function to save CAN ID in Register */
static void convertCANid2Reg(uint32_t tempPassedInID, uint8_t canIdType, id_reg_t *passedIdReg)
{
  uint8_t wipSIDL = 0;

  if (canIdType == dEXTENDED_CAN_MSG_ID_2_0B)
  {
    //EID0
    passedIdReg->tempEID0 = 0xFF & tempPassedInID;
    tempPassedInID = tempPassedInID >> 8;

    //EID8
    passedIdReg->tempEID8 = 0xFF & tempPassedInID;
    tempPassedInID = tempPassedInID >> 8;

    //SIDL
    wipSIDL = 0x03 & tempPassedInID;
    tempPassedInID = tempPassedInID << 3;
    wipSIDL = (0xE0 & tempPassedInID) + wipSIDL;
    wipSIDL = wipSIDL + 0x08;
    passedIdReg->tempSIDL = 0xEB & wipSIDL;

    //SIDH
    tempPassedInID = tempPassedInID >> 8;
    passedIdReg->tempSIDH = 0xFF & tempPassedInID;
  }
  else
  {
    passedIdReg->tempEID8 = 0;
    passedIdReg->tempEID0 = 0;
    tempPassedInID = tempPassedInID << 5;
    passedIdReg->tempSIDL = 0xFF & tempPassedInID;
    tempPassedInID = tempPassedInID >> 8;
    passedIdReg->tempSIDH = 0xFF & tempPassedInID;
  }
}

/*
 * CANSPI.c
 *
 *  Created on: Oct 15, 2021
 *      Author: Egor.Beliaev
 */


