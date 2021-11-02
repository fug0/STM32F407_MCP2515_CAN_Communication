/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usbd_cdc_if.h"
#include "CANSPI.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define MCP2515_FIRST 0
#define MCP2515_SECOND 1
#define DELAY(t_del) uint32_t x = 0; while(x != t_del) { x ++; }
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CAN_HandleTypeDef hcan1;
CAN_HandleTypeDef hcan2;

SPI_HandleTypeDef hspi2;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim14;

/* USER CODE BEGIN PV */


CAN_TxHeaderTypeDef TxHeader1;
CAN_TxHeaderTypeDef TxHeader2;
CAN_RxHeaderTypeDef RxHeader;

uCAN_MSG MCP2515txMessageFirst;
uCAN_MSG MCP2515rxMessageFirst;
uCAN_MSG MCP2515txMessageSecond;
uCAN_MSG MCP2515rxMessageSecond;

uint8_t TxData1[8] = {1, 2, 3, 4, 5, 6, 7, 8};
uint8_t TxData2[8] = {101, 102, 103, 104, 105, 106, 107, 108};
uint8_t RxData[8]  = {};

uint8_t OutputBuffer[100] = {};

uint32_t TxMailbox = 0;

uint8_t PLC_Transmit_Flag = 0;
uint8_t MCP_Transmit_Flag = 0;

uint8_t MCP_Receiving_State = 0;
/* 0 - none of the modules received a message
 * 1 - only one module received a message
 * 2 - both modules received a message */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_SPI2_Init(void);
static void MX_CAN2_Init(void);
static void MX_CAN1_Init(void);
static void MX_TIM14_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


void HAL_CAN_TxMailbox0CompleteCallback (CAN_HandleTypeDef * hcan)
{
	PLC_Transmit_Flag = 1;
}


void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK)
    {
    	switch(RxHeader.StdId)
    	{
    	case 0x400:
    		MCP_Transmit_Flag = 0;

			TxHeader1.StdId = 0x300;
			TxHeader1.ExtId = RxHeader.ExtId;
			TxHeader1.RTR   = RxHeader.RTR;
			TxHeader1.IDE   = RxHeader.IDE;
			TxHeader1.DLC   = RxHeader.DLC;
			TxHeader1.TransmitGlobalTime = 0;

			for(uint8_t i = 0; i < TxHeader1.DLC; i++)
			{
				TxData1[i] = RxData[i] + 2;
			}

			HAL_CAN_AddTxMessage(hcan, &TxHeader1, TxData1, &TxMailbox);

			break;
    	case 0x401:
    		MCP_Transmit_Flag = 0;

			TxHeader2.StdId = 0x301;
			TxHeader2.ExtId = RxHeader.ExtId;
			TxHeader2.RTR   = RxHeader.RTR;
			TxHeader2.IDE   = RxHeader.IDE;
			TxHeader2.DLC   = RxHeader.DLC;
			TxHeader2.TransmitGlobalTime = 0;

			for(uint8_t i = 0; i < TxHeader2.DLC; i++)
			{
				TxData2[i] = RxData[i] + 2;
			}

			HAL_CAN_AddTxMessage(hcan, &TxHeader2, TxData2, &TxMailbox);

			break;
    	case 0x300:
    		MCP_Transmit_Flag = 0;

			TxHeader1.StdId = 0x412;
			TxHeader1.ExtId = RxHeader.ExtId;
			TxHeader1.RTR   = RxHeader.RTR;
			TxHeader1.IDE   = RxHeader.IDE;
			TxHeader1.DLC   = RxHeader.DLC;
			TxHeader1.TransmitGlobalTime = 0;

			for(uint8_t i = 0; i < TxHeader1.DLC; i++)
			{
				TxData1[i] = RxData[i] + 2;
			}

			MCP_Receiving_State--;
			HAL_CAN_AddTxMessage(hcan, &TxHeader1, TxData1, &TxMailbox);

			break;
    	case 0x301:
    		MCP_Transmit_Flag = 0;

			TxHeader2.StdId = 0x258;
			TxHeader2.ExtId = RxHeader.ExtId;
			TxHeader2.RTR   = RxHeader.RTR;
			TxHeader2.IDE   = RxHeader.IDE;
			TxHeader2.DLC   = RxHeader.DLC;
			TxHeader2.TransmitGlobalTime = 0;

			for(uint8_t i = 0; i < TxHeader2.DLC; i++)
			{
				TxData2[i] = RxData[i] + 2;
			}

			MCP_Receiving_State--;
			HAL_CAN_AddTxMessage(hcan, &TxHeader2, TxData2, &TxMailbox);

			break;
    	}
    }
}


void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
    uint8_t er[100] = {};

    snprintf((char *)OutputBuffer, 100, "ERROR_CODE is: %d \r\n", (uint8_t)HAL_CAN_GetError(hcan));
    CDC_Transmit_FS(OutputBuffer, strlen((char *)OutputBuffer));

    CDC_Transmit_FS(er, 8);
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if((htim == &htim2) && (PLC_Transmit_Flag == 1))
	{
		if(PLC_Transmit_Flag == 1)
		{
			 HAL_GPIO_TogglePin(GPIOD, LED_Green0_Pin);
		}
		if(CANSPI_Receive(&MCP2515rxMessageFirst, MCP2515_FIRST))
		{
		  MCP_Receiving_State++;
		  PLC_Transmit_Flag = 0;

		  MCP2515txMessageFirst.frame.idType = MCP2515rxMessageFirst.frame.idType;
		  MCP2515txMessageFirst.frame.id     = 0x400;
		  MCP2515txMessageFirst.frame.dlc    = MCP2515rxMessageFirst.frame.dlc;
		  MCP2515txMessageFirst.frame.data0  = MCP2515rxMessageFirst.frame.data0 + 1;
		  MCP2515txMessageFirst.frame.data1  = MCP2515rxMessageFirst.frame.data1 + 1;
		  MCP2515txMessageFirst.frame.data2  = MCP2515rxMessageFirst.frame.data2 + 1;
		  MCP2515txMessageFirst.frame.data3  = MCP2515rxMessageFirst.frame.data3 + 1;
		  MCP2515txMessageFirst.frame.data4  = MCP2515rxMessageFirst.frame.data4 + 1;
		  MCP2515txMessageFirst.frame.data5  = MCP2515rxMessageFirst.frame.data5 + 1;
		  MCP2515txMessageFirst.frame.data6  = MCP2515rxMessageFirst.frame.data6 + 1;
		  MCP2515txMessageFirst.frame.data7  = MCP2515rxMessageFirst.frame.data7 + 1;
		}

		if(CANSPI_Receive(&MCP2515rxMessageSecond, MCP2515_SECOND))
		{
		  MCP_Receiving_State++;
	      PLC_Transmit_Flag = 0;

		  MCP2515txMessageSecond.frame.idType = MCP2515rxMessageSecond.frame.idType;
		  MCP2515txMessageSecond.frame.id     = 0x401;
		  MCP2515txMessageSecond.frame.dlc    = MCP2515rxMessageSecond.frame.dlc;
		  MCP2515txMessageSecond.frame.data0  = MCP2515rxMessageSecond.frame.data0 + 1;
		  MCP2515txMessageSecond.frame.data1  = MCP2515rxMessageSecond.frame.data1 + 1;
		  MCP2515txMessageSecond.frame.data2  = MCP2515rxMessageSecond.frame.data2 + 1;
		  MCP2515txMessageSecond.frame.data3  = MCP2515rxMessageSecond.frame.data3 + 1;
		  MCP2515txMessageSecond.frame.data4  = MCP2515rxMessageSecond.frame.data4 + 1;
		  MCP2515txMessageSecond.frame.data5  = MCP2515rxMessageSecond.frame.data5 + 1;
		  MCP2515txMessageSecond.frame.data6  = MCP2515rxMessageSecond.frame.data6 + 1;
		  MCP2515txMessageSecond.frame.data7  = MCP2515rxMessageSecond.frame.data7 + 1;
		}
		/* MCP2515_1 and MCP2515_2 messages transmitting */
		if((CANSPI_Transmit(&MCP2515txMessageFirst, MCP2515_FIRST)) &&
		   (CANSPI_Transmit(&MCP2515txMessageSecond, MCP2515_SECOND)))
		{
			  MCP_Transmit_Flag = 1;

			  HAL_GPIO_TogglePin(GPIOD, LED_Red0_Pin);
		}
	}
	else if(htim == &htim14)
	{
		snprintf((char *)OutputBuffer, 100, "Last data received by CAN1 or CAN2 is: ID %d DATA %s \r\n", (uint16_t)RxHeader.StdId, RxData);
		CDC_Transmit_FS(OutputBuffer, strlen((char *)OutputBuffer));
	}
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USB_DEVICE_Init();
  MX_TIM2_Init();
  MX_SPI2_Init();
  MX_CAN2_Init();
  MX_CAN1_Init();
  MX_TIM14_Init();
  /* USER CODE BEGIN 2 */

  /* MCP2515 initializing */
  CANSPI_Initialize(MCP2515_FIRST);
  CANSPI_Initialize(MCP2515_SECOND);

  /* TIM2 and TIM14 Interrupt Mode Activating */
  HAL_TIM_Base_Start_IT(&htim2);
  HAL_TIM_Base_Start_IT(&htim14);

  /* CAN Starting */
  HAL_CAN_Start(&hcan1);
  HAL_CAN_Start(&hcan2);

  /* Activating CAN notifications */
  HAL_CAN_ActivateNotification(&hcan1, CAN_IT_TX_MAILBOX_EMPTY |
									   CAN_IT_RX_FIFO1_MSG_PENDING |
									   CAN_IT_RX_FIFO0_MSG_PENDING |
									   CAN_IT_ERROR |
									   CAN_IT_BUSOFF |
									   CAN_IT_LAST_ERROR_CODE);

  HAL_CAN_ActivateNotification(&hcan2, CAN_IT_TX_MAILBOX_EMPTY |
									   CAN_IT_RX_FIFO1_MSG_PENDING |
									   CAN_IT_RX_FIFO0_MSG_PENDING |
									   CAN_IT_ERROR |
									   CAN_IT_BUSOFF |
		                               CAN_IT_LAST_ERROR_CODE);

  /* Messages Configuration */
  TxHeader1.StdId = 0x412;
  TxHeader1.ExtId = 0;
  TxHeader1.RTR   = CAN_RTR_DATA;
  TxHeader1.IDE   = CAN_ID_STD;
  TxHeader1.DLC   = 8;
  TxHeader1.TransmitGlobalTime = 0;

  TxHeader2.StdId = 0x258;
  TxHeader2.ExtId = 0;
  TxHeader2.RTR   = CAN_RTR_DATA;
  TxHeader2.IDE   = CAN_ID_STD;
  TxHeader2.DLC   = 8;
  TxHeader2.TransmitGlobalTime = 0;

  /* CAN2 to MCP2515_FIRST and MCP2515_SECOND messages transmitting */

  while(!(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_READY));

  while(HAL_CAN_GetTxMailboxesFreeLevel(&hcan2) == 0);

  HAL_CAN_AddTxMessage(&hcan2, &TxHeader1, TxData1, &TxMailbox);
  HAL_CAN_AddTxMessage(&hcan2, &TxHeader2, TxData2, &TxMailbox);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CAN1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN1_Init(void)
{

  /* USER CODE BEGIN CAN1_Init 0 */

  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 6;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_2TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_1TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = ENABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = ENABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = ENABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */
  CAN_FilterTypeDef  FilterConfigCAN1;

  FilterConfigCAN1.FilterBank = 0;
  FilterConfigCAN1.FilterMode = CAN_FILTERMODE_IDMASK;
  FilterConfigCAN1.FilterScale = CAN_FILTERSCALE_32BIT;
  FilterConfigCAN1.FilterIdHigh = 0x400 << 5;
  FilterConfigCAN1.FilterIdLow = 0x0000;
  FilterConfigCAN1.FilterMaskIdHigh = 0x7FE << 5;
  FilterConfigCAN1.FilterMaskIdLow = 0x0000;
  FilterConfigCAN1.FilterFIFOAssignment = CAN_RX_FIFO0;
  FilterConfigCAN1.FilterActivation = ENABLE;
  FilterConfigCAN1.SlaveStartFilterBank = 14;

  if(HAL_CAN_ConfigFilter(&hcan1, &FilterConfigCAN1) != HAL_OK)
  {
	  Error_Handler();
  }

  /* USER CODE END CAN1_Init 2 */

}

/**
  * @brief CAN2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN2_Init(void)
{

  /* USER CODE BEGIN CAN2_Init 0 */

  /* USER CODE END CAN2_Init 0 */

  /* USER CODE BEGIN CAN2_Init 1 */

  /* USER CODE END CAN2_Init 1 */
  hcan2.Instance = CAN2;
  hcan2.Init.Prescaler = 6;
  hcan2.Init.Mode = CAN_MODE_NORMAL;
  hcan2.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan2.Init.TimeSeg1 = CAN_BS1_2TQ;
  hcan2.Init.TimeSeg2 = CAN_BS2_1TQ;
  hcan2.Init.TimeTriggeredMode = DISABLE;
  hcan2.Init.AutoBusOff = ENABLE;
  hcan2.Init.AutoWakeUp = DISABLE;
  hcan2.Init.AutoRetransmission = ENABLE;
  hcan2.Init.ReceiveFifoLocked = DISABLE;
  hcan2.Init.TransmitFifoPriority = ENABLE;
  if (HAL_CAN_Init(&hcan2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN2_Init 2 */
  CAN_FilterTypeDef  FilterConfigCAN2;

  FilterConfigCAN2.FilterBank = 14;
  FilterConfigCAN2.FilterMode = CAN_FILTERMODE_IDMASK;
  FilterConfigCAN2.FilterScale = CAN_FILTERSCALE_32BIT;
  FilterConfigCAN2.FilterIdHigh = 0x300 << 5;
  FilterConfigCAN2.FilterIdLow = 0x0000;
  FilterConfigCAN2.FilterMaskIdHigh = 0x7FE << 5;
  FilterConfigCAN2.FilterMaskIdLow = 0x0000;
  FilterConfigCAN2.FilterFIFOAssignment = CAN_RX_FIFO0;
  FilterConfigCAN2.FilterActivation = ENABLE;
  FilterConfigCAN2.SlaveStartFilterBank = 14;

  if(HAL_CAN_ConfigFilter(&hcan2, &FilterConfigCAN2) != HAL_OK)
  {
	  Error_Handler();
  }
  /* USER CODE END CAN2_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 479;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 9999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM14 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM14_Init(void)
{

  /* USER CODE BEGIN TIM14_Init 0 */

  /* USER CODE END TIM14_Init 0 */

  /* USER CODE BEGIN TIM14_Init 1 */

  /* USER CODE END TIM14_Init 1 */
  htim14.Instance = TIM14;
  htim14.Init.Prescaler = 4799;
  htim14.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim14.Init.Period = 9999;
  htim14.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim14.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim14) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM14_Init 2 */

  /* USER CODE END TIM14_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, LED_Green0_Pin|LED_Red0_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SPI2_CS1_GPIO_Port, SPI2_CS1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SPI2_CS0_GPIO_Port, SPI2_CS0_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : LED_Green0_Pin LED_Red0_Pin */
  GPIO_InitStruct.Pin = LED_Green0_Pin|LED_Red0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : SPI2_CS1_Pin SPI2_CS0_Pin */
  GPIO_InitStruct.Pin = SPI2_CS1_Pin|SPI2_CS0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
