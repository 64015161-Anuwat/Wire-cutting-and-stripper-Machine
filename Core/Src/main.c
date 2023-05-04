/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "stdio.h"
#include "string.h"

#include "ILI9341_Touchscreen.h"
#include "ILI9341_STM32_Driver.h"
#include "ILI9341_GFX.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define STEP1_PORT GPIOD
#define STEP1_PIN GPIO_PIN_0
#define DIR1_PORT GPIOD
#define DIR1_PIN GPIO_PIN_1

#define STEP2_PORT GPIOF
#define STEP2_PIN GPIO_PIN_1
#define DIR2_PORT GPIOF
#define DIR2_PIN GPIO_PIN_0

#define mmPerStep 0.15096

uint32_t currentMillis;
uint32_t previousMillis;

uint8_t start = 0;
uint8_t ok = 0;
uint8_t cancel = 0;
uint8_t pause = 0;
uint8_t touch = 0;

int adc_val = 0;

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
int map(int, int, int, int, int);
void delayMicro(uint16_t);
void step1(int, uint8_t);
void step2(int, uint8_t);
int numpad(int, char*);
void textStatus(char*);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int map(int val, int in_min, int in_max, int out_min, int out_max) {
    return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
void delayMicro(uint16_t us)
{
	__HAL_TIM_SET_COUNTER(&htim1,0);  // set the counter value a 0
	while (__HAL_TIM_GET_COUNTER(&htim1) < us);  // wait for the counter to reach the us input in the parameter
}
void step1(int steps, uint8_t direction)
{
	int stepsToTake = steps/mmPerStep;

	if (direction == 0) HAL_GPIO_WritePin(DIR1_PORT, DIR1_PIN, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(DIR1_PORT, DIR1_PIN, GPIO_PIN_RESET);
	for(int i = 0; i < stepsToTake; i+=1)
	{
		if(cancel == 1) break;
		while(pause == 1);
		HAL_GPIO_WritePin(STEP1_PORT, STEP1_PIN, GPIO_PIN_SET);
		delayMicro(adc_val);
		HAL_GPIO_WritePin(STEP1_PORT, STEP1_PIN, GPIO_PIN_RESET);
		delayMicro(adc_val);
	}
}
void step2(int steps, uint8_t direction)
{
	int stepsToTake = steps;

	if (direction == 0) HAL_GPIO_WritePin(DIR2_PORT, DIR2_PIN, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(DIR2_PORT, DIR2_PIN, GPIO_PIN_RESET);
	for(int i = 0; i < stepsToTake; i+=1)
	{
		if(cancel == 1) break;
		while(pause == 1);
		HAL_GPIO_WritePin(STEP2_PORT, STEP2_PIN, GPIO_PIN_SET);
		delayMicro(adc_val);
		HAL_GPIO_WritePin(STEP2_PORT, STEP2_PIN, GPIO_PIN_RESET);
		delayMicro(adc_val);
	}
}
int numpad(int val, char* title)
{
	int num = val;
	// WireLength1
	//					  ILI9341_Draw_Filled_Rectangle_Coord(15, 75, 60, 100, BLACK);
	// Container
	ILI9341_Draw_Filled_Rectangle_Coord(35, 15, 235, 230, 0xA514);
	// Title
	ILI9341_Draw_Text(title, 45, 20, BLACK, 2, 0xA514);
	// NumPad 1 2 3
	ILI9341_Draw_Hollow_Rectangle_Coord(45, 75, 103, 100, BLACK);
	ILI9341_Draw_Text("1", 71, 80, BLACK, 2, 0xA514);
	ILI9341_Draw_Hollow_Rectangle_Coord(108, 75, 166, 100, BLACK);
	ILI9341_Draw_Text("2", 133, 80, BLACK, 2, 0xA514);
	ILI9341_Draw_Hollow_Rectangle_Coord(171, 75, 224, 100, BLACK);
	ILI9341_Draw_Text("3", 194, 80, BLACK, 2, 0xA514);
	// NumPad 4 5 6
	ILI9341_Draw_Hollow_Rectangle_Coord(45, 105, 103, 130, BLACK);
	ILI9341_Draw_Text("4", 69, 110, BLACK, 2, 0xA514);
	ILI9341_Draw_Hollow_Rectangle_Coord(108, 105, 166, 130, BLACK);
	ILI9341_Draw_Text("5", 133, 110, BLACK, 2, 0xA514);
	ILI9341_Draw_Hollow_Rectangle_Coord(171, 105, 224, 130, BLACK);
	ILI9341_Draw_Text("6", 194, 110, BLACK, 2, 0xA514);
	// NumPad 7 8 9
	ILI9341_Draw_Hollow_Rectangle_Coord(45, 135, 103, 160, BLACK);
	ILI9341_Draw_Text("7", 69, 140, BLACK, 2, 0xA514);
	ILI9341_Draw_Hollow_Rectangle_Coord(108, 135, 166, 160, BLACK);
	ILI9341_Draw_Text("8", 133, 140, BLACK, 2, 0xA514);
	ILI9341_Draw_Hollow_Rectangle_Coord(171, 135, 224, 160, BLACK);
	ILI9341_Draw_Text("9", 194, 140, BLACK, 2, 0xA514);
	// NumPad 0 del
	ILI9341_Draw_Hollow_Rectangle_Coord(45, 165, 166, 190, BLACK);
	ILI9341_Draw_Text("0", 100, 170, BLACK, 2, 0xA514);
	ILI9341_Draw_Hollow_Rectangle_Coord(171, 165, 224, 190, BLACK);
	ILI9341_Draw_Text("DEL", 181, 170, BLACK, 2, 0xA514);
	// NumPad Cancel OK
	ILI9341_Draw_Filled_Rectangle_Coord(45, 195, 134, 220, 0xF800);
	ILI9341_Draw_Text("Cancel", 55, 200, WHITE, 2, 0xF800);
	ILI9341_Draw_Filled_Rectangle_Coord(139, 195, 224, 220, 0x0580);
	ILI9341_Draw_Text("OK", 171, 199, WHITE, 2, 0x0580);

	while(ok == 0 && cancel == 0)
	{
		// Number Monitor
		ILI9341_Draw_Filled_Rectangle_Coord(45, 40, 225, 70, WHITE);
		char valBuff[5];
		sprintf(valBuff, "%d", num);
		ILI9341_Draw_Text(valBuff, 55, 47, BLACK, 2, WHITE);
		while(ok == 0 && cancel == 0)
		{
			if(TP_Touchpad_Pressed())
			{
				uint16_t x_pos = 0;
			    uint16_t y_pos = 0;

			    uint16_t position_array[2];
				if(TP_Read_Coordinates(position_array) == TOUCHPAD_DATA_OK)
				{
					x_pos = position_array[1];
					y_pos = 240 - position_array[0];
					if(((x_pos >= 45)&&(x_pos <= 103)) && ((y_pos >= 75)&&(y_pos <= 100)) && touch == 0)
					{
					  // 1
					  num *= 10;
					  num += 1;
					  touch = 1;
					}
					else if(((x_pos >= 108)&&(x_pos <= 166)) && ((y_pos >= 75)&&(y_pos <= 100)) && touch == 0)
					{
					  // 2
					  num *= 10;
					  num += 2;
					  touch = 1;
					}
					else if(((x_pos >= 171)&&(x_pos <= 224)) && ((y_pos >= 75)&&(y_pos <= 100)) && touch == 0)
					{
					  // 3
					  num *= 10;
					  num += 3;
					  touch = 1;
					}
					else if(((x_pos >= 45)&&(x_pos <= 103)) && ((y_pos >= 105)&&(y_pos <= 130)) && touch == 0)
					{
					  // 4
					  num *= 10;
					  num += 4;
					  touch = 1;
					}
					else if(((x_pos >= 108)&&(x_pos <= 166)) && ((y_pos >= 105)&&(y_pos <= 130)) && touch == 0)
					{
					  // 5
					  num *= 10;
					  num += 5;
					  touch = 1;
					}
					else if(((x_pos >= 171)&&(x_pos <= 224)) && ((y_pos >= 105)&&(y_pos <= 130)) && touch == 0)
					{
					  // 6
					  num *= 10;
					  num += 6;
					  touch = 1;
					}
					else if(((x_pos >= 45)&&(x_pos <= 103)) && ((y_pos >= 135)&&(y_pos <= 160)) && touch == 0)
					{
					  // 7
					  num *= 10;
					  num += 7;
					  touch = 1;
					}
					else if(((x_pos >= 108)&&(x_pos <= 166)) && ((y_pos >= 135)&&(y_pos <= 160)) && touch == 0)
					{
					  // 8
					  num *= 10;
					  num += 8;
					  touch = 1;
					}
					else if(((x_pos >= 171)&&(x_pos <= 224)) && ((y_pos >= 135)&&(y_pos <= 160)) && touch == 0)
					{
					  // 9
					  num *= 10;
					  num += 9;
					  touch = 1;
					}
					else if(((x_pos >= 45)&&(x_pos <= 166)) && ((y_pos >= 165)&&(y_pos <= 190)) && touch == 0)
					{
					  // 0
					  num *= 10;
					  num += 0;
					  touch = 1;
					}
					else if(((x_pos >= 171)&&(x_pos <= 224)) && ((y_pos >= 165)&&(y_pos <= 190)) && touch == 0)
					{
					  // DEL
					  num /= 10;
					  touch = 1;
					}
					else if(((x_pos >= 45)&&(x_pos <= 134)) && ((y_pos >= 195)&&(y_pos <= 220)) && touch == 0)
					{
					  // CANCEL
					  touch = 1;
					  cancel = 1;
					  return val;
					}
					else if(((x_pos >= 139)&&(x_pos <= 224)) && ((y_pos >= 195)&&(y_pos <= 220)) && touch == 0)
					{
					  // OK
					  touch = 1;
					  ok = 1;
					}
				}break;
			}else touch = 0;
		}
	}
	if(ok == 1) return num;
	if(cancel == 1) return val;
	return 0;
}
void textStatus(char* text)
{
	ILI9341_Draw_Filled_Rectangle_Coord(110, 140, 230, 162, WHITE);
	ILI9341_Draw_Text(text, 115, 145, BLACK, 2, WHITE);
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

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

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
  MX_TIM1_Init();
  MX_ADC1_Init();
  MX_TIM2_Init();
  MX_SPI5_Init();
  /* USER CODE BEGIN 2 */

  ILI9341_Init(); //initial driver setup to drive ili9341

  HAL_TIM_Base_Start(&htim1);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
  HAL_ADC_Start_IT(&hadc1);

  int qty = 2;
  int wireLength1 = 10;
  int wireLength2 = 25;
  int wireLength3 = 10;

  htim2.Instance->CCR4 = 55;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  ILI9341_Fill_Screen(WHITE);
	  ILI9341_Set_Rotation(SCREEN_HORIZONTAL_1);
	  ILI9341_Draw_Text("WIRE CUTTER &", 10, 10, BLACK, 2, WHITE);
	  ILI9341_Draw_Text("STRIPPER MACHINE", 10, 25, BLACK, 2, WHITE);

	  ILI9341_Draw_Filled_Rectangle_Coord(20, 55, 255, 60, 0xE3E9);

	  ILI9341_Draw_Filled_Rectangle_Coord(20, 50, 55, 65, 0x5BFF);
	  ILI9341_Draw_Filled_Rectangle_Coord(70, 50, 205, 65, 0x5BFF);
	  ILI9341_Draw_Filled_Rectangle_Coord(220, 50, 255, 65, 0x5BFF);

	  ILI9341_Draw_Filled_Rectangle_Coord(15, 75, 60, 100, 0xFFEE);
	  ILI9341_Draw_Filled_Rectangle_Coord(115, 75, 160, 100, 0xFFEE);
	  ILI9341_Draw_Filled_Rectangle_Coord(215, 75, 260, 100, 0xFFEE);

	  char wireBuff[5];
	  sprintf(wireBuff, "%03d", wireLength1);
	  ILI9341_Draw_Text(wireBuff, 20, 80, BLACK, 2, 0xFFEE);
//	  if(wireLength1 > 99) ILI9341_Draw_Text(wireBuff, 20, 80, BLACK, 2, 0xFFEE);
//	  else if(wireLength1 > 9) ILI9341_Draw_Text(wireBuff, 27, 80, BLACK, 2, 0xFFEE);
//	  else ILI9341_Draw_Text(wireBuff, 33, 80, BLACK, 2, 0xFFEE);
	  sprintf(wireBuff, "%03d", wireLength2);
	  ILI9341_Draw_Text(wireBuff, 120, 80, BLACK, 2, 0xFFEE);
//	  if(wireLength2 > 99) ILI9341_Draw_Text(wireBuff, 120, 80, BLACK, 2, 0xFFEE);
//	  else if(wireLength2 > 9) ILI9341_Draw_Text(wireBuff, 127, 80, BLACK, 2, 0xFFEE);
//	  else ILI9341_Draw_Text(wireBuff, 133, 80, BLACK, 2, 0xFFEE);
	  sprintf(wireBuff, "%03d", wireLength3);
	  ILI9341_Draw_Text(wireBuff, 220, 80, BLACK, 2, 0xFFEE);
//	  if(wireLength3 > 99) ILI9341_Draw_Text(wireBuff, 220, 80, BLACK, 2, 0xFFEE);
//	  else if(wireLength3 > 9) ILI9341_Draw_Text(wireBuff, 227, 80, BLACK, 2, 0xFFEE);
//	  else ILI9341_Draw_Text(wireBuff, 233, 80, BLACK, 2, 0xFFEE);

	  ILI9341_Draw_Text("***dimensions are in millimeters.", 10, 105, BLACK, 1, WHITE);

	  ILI9341_Draw_Text("QTY :", 10, 120, BLACK, 2, WHITE);
	  ILI9341_Draw_Filled_Rectangle_Coord(75, 115, 120, 140, 0xFFEE);
	  char qtyBuff[5];
	  sprintf(qtyBuff, "%03d", qty);
	  ILI9341_Draw_Text(qtyBuff, 80, 120, BLACK, 2, 0xFFEE);

	  ILI9341_Draw_Text("Stepper1 :", 10, 145, BLACK, 2, WHITE);
	  ILI9341_Draw_Hollow_Rectangle_Coord(140, 140, 185, 165, BLACK);
	  ILI9341_Draw_Text("<", 158, 145, BLACK, 2, WHITE);
	  ILI9341_Draw_Hollow_Rectangle_Coord(195, 140, 240, 165, BLACK);
	  ILI9341_Draw_Text(">", 215, 145, BLACK, 2, WHITE);

	  ILI9341_Draw_Text("Stepper2 :", 10, 175, BLACK, 2, WHITE);
	  ILI9341_Draw_Hollow_Rectangle_Coord(140, 170, 185, 195, BLACK);
	  ILI9341_Draw_Text("<", 158, 175, BLACK, 2, WHITE);
	  ILI9341_Draw_Hollow_Rectangle_Coord(195, 170, 240, 195, BLACK);
	  ILI9341_Draw_Text(">", 215, 175, BLACK, 2, WHITE);

	  ILI9341_Draw_Filled_Rectangle_Coord(70, 200, 205, 230, 0xF800);
	  ILI9341_Draw_Text("START", 110, 206, WHITE, 2, 0xF800);

	  while(start == 0)
	  {
		  if(TP_Touchpad_Pressed())
		  {
			  if(ok == 1 || cancel == 1) break;
			  uint16_t x_pos = 0;
			  uint16_t y_pos = 0;

			  uint16_t position_array[2];

			  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14|GPIO_PIN_7, GPIO_PIN_SET);

			  if(TP_Read_Coordinates(position_array) == TOUCHPAD_DATA_OK)
			  {
				  x_pos = position_array[1];
				  y_pos = 240 - position_array[0];

				  if(((x_pos >= 15)&&(x_pos <= 60)) && ((y_pos >= 75)&&(y_pos <= 100)) && touch == 0)
				  {
					  // WireLength1
					  touch = 1;
					  wireLength1 = numpad(wireLength1, "Wire Length 1");
				  }
				  else if(((x_pos >= 115)&&(x_pos <= 160)) && ((y_pos >= 75)&&(y_pos <= 100)) && touch == 0)
				  {
					  // WireLength2
					  touch = 1;
					  wireLength2 = numpad(wireLength2, "Wire Length 2");
				  }
				  else if(((x_pos >= 215)&&(x_pos <= 260)) && ((y_pos >= 75)&&(y_pos <= 100)) && touch == 0)
				  {
					  // WireLength3
					  touch = 1;
					  wireLength3 = numpad(wireLength3, "Wire Length 3");
				  }
				  else if(((x_pos >= 75)&&(x_pos <= 120)) && ((y_pos >= 115)&&(y_pos <= 140)) && touch == 0)
				  {
					  // QTY
					  touch = 1;
					  qty = numpad(qty, "QUANTITY");
				  }
				  else if(((x_pos >= 140)&&(x_pos <= 185)) && ((y_pos >= 140)&&(y_pos <= 165)) && touch == 0)
				  {
					  // Stepper1 <
					  while(TP_Touchpad_Pressed())
					  {
						  HAL_GPIO_WritePin(DIR1_PORT, DIR1_PIN, GPIO_PIN_SET);
						  HAL_GPIO_WritePin(STEP1_PORT, STEP1_PIN, GPIO_PIN_SET);
						  delayMicro(adc_val);
						  HAL_GPIO_WritePin(STEP1_PORT, STEP1_PIN, GPIO_PIN_RESET);
						  delayMicro(adc_val);
					  }
				  }
				  else if(((x_pos >= 195)&&(x_pos <= 240)) && ((y_pos >= 140)&&(y_pos <= 165)) && touch == 0)
				  {
					  // Stepper1 >
					  while(TP_Touchpad_Pressed())
					  {
						  HAL_GPIO_WritePin(DIR1_PORT, DIR1_PIN, GPIO_PIN_RESET);
						  HAL_GPIO_WritePin(STEP1_PORT, STEP1_PIN, GPIO_PIN_SET);
						  delayMicro(adc_val);
						  HAL_GPIO_WritePin(STEP1_PORT, STEP1_PIN, GPIO_PIN_RESET);
						  delayMicro(adc_val);
					  }
				  }
				  else if(((x_pos >= 140)&&(x_pos <= 185)) && ((y_pos >= 170)&&(y_pos <= 195)) && touch == 0)
				  {
					  // Stepper2 <
					  while(TP_Touchpad_Pressed())
					  {
						  HAL_GPIO_WritePin(DIR2_PORT, DIR2_PIN, GPIO_PIN_RESET);
						  HAL_GPIO_WritePin(STEP2_PORT, STEP2_PIN, GPIO_PIN_SET);
						  delayMicro(adc_val);
						  HAL_GPIO_WritePin(STEP2_PORT, STEP2_PIN, GPIO_PIN_RESET);
						  delayMicro(adc_val);
					  }
				  }
				  else if(((x_pos >= 195)&&(x_pos <= 240)) && ((y_pos >= 170)&&(y_pos <= 195)) && touch == 0)
				  {
					  // Stepper2 >
					  while(TP_Touchpad_Pressed())
					  {
						  HAL_GPIO_WritePin(DIR2_PORT, DIR2_PIN, GPIO_PIN_SET);
						  HAL_GPIO_WritePin(STEP2_PORT, STEP2_PIN, GPIO_PIN_SET);
						  delayMicro(adc_val);
						  HAL_GPIO_WritePin(STEP2_PORT, STEP2_PIN, GPIO_PIN_RESET);
						  delayMicro(adc_val);
					  }
				  }
				  else if(((x_pos >= 70)&&(x_pos <= 205)) && ((y_pos >= 200)&&(y_pos <= 230)) && touch == 0)
				  {
					  // Start
					  touch = 1;
					  ILI9341_Draw_Filled_Rectangle_Coord(70, 200, 205, 230, BLACK);
					  start = 1;
				  }
			  }
		  } else touch = 0;
	  }

	  if(start == 1)
	  {
		  ILI9341_Fill_Screen(WHITE);
		  ILI9341_Draw_Text("WIRE CUTTER &", 10, 10, BLACK, 2, WHITE);
		  ILI9341_Draw_Text("STRIPPER MACHINE", 10, 25, BLACK, 2, WHITE);

		  ILI9341_Draw_Filled_Rectangle_Coord(20, 55, 255, 60, 0xE3E9);

		  ILI9341_Draw_Filled_Rectangle_Coord(20, 50, 55, 65, 0x5BFF);
		  ILI9341_Draw_Filled_Rectangle_Coord(70, 50, 205, 65, 0x5BFF);
		  ILI9341_Draw_Filled_Rectangle_Coord(220, 50, 255, 65, 0x5BFF);

		  ILI9341_Draw_Filled_Rectangle_Coord(15, 75, 60, 100, 0xFFEE);
		  ILI9341_Draw_Filled_Rectangle_Coord(115, 75, 160, 100, 0xFFEE);
		  ILI9341_Draw_Filled_Rectangle_Coord(215, 75, 260, 100, 0xFFEE);

		  char wireBuff[5];
		  sprintf(wireBuff, "%03d", wireLength1);
		  ILI9341_Draw_Text(wireBuff, 20, 80, BLACK, 2, 0xFFEE);
		  sprintf(wireBuff, "%03d", wireLength2);
		  ILI9341_Draw_Text(wireBuff, 120, 80, BLACK, 2, 0xFFEE);
		  sprintf(wireBuff, "%03d", wireLength3);
		  ILI9341_Draw_Text(wireBuff, 220, 80, BLACK, 2, 0xFFEE);

		  ILI9341_Draw_Text("***dimensions are in millimeters.", 10, 105, BLACK, 1, WHITE);

		  ILI9341_Draw_Text("QTY :", 10, 120, BLACK, 2, WHITE);
		  ILI9341_Draw_Filled_Rectangle_Coord(75, 115, 120, 140, 0xFFEE);
		  char qtyBuff[5];
		  sprintf(qtyBuff, "%03d", qty);
		  ILI9341_Draw_Text(qtyBuff, 80, 120, BLACK, 2, 0xFFEE);
		  ILI9341_Draw_Text("Status :", 10, 145, BLACK, 2, WHITE);
		  ILI9341_Draw_Text("Running", 115, 145, BLACK, 2, WHITE);
		  ILI9341_Draw_Text("Finished QTY :", 10, 170, BLACK, 2, WHITE);
		  ILI9341_Draw_Filled_Rectangle_Coord(187, 165, 227, 190, 0xFFEE);

		  char buff[5];
		  int i = 0;
		  uint8_t work = 0;
		  for(i = 0; i < qty && cancel == 0; i+=1)
		  {
			  work = 0;
			  sprintf(buff, "%03d", i);
			  ILI9341_Draw_Text(buff, 190, 170, BLACK, 2, 0xFFEE);

			  step1(wireLength1, 0);
			  if(wireLength1 > 0)
			  {
				  step2(200, 0);
			  }

			  step1(wireLength2, 0);
			  if(wireLength3 > 0)
			  {
				  step2(200, 0);
			  }

			  step1(wireLength3, 0);
			  htim2.Instance->CCR4 = 62;
			  HAL_Delay(2000);
			  step2(200, 0);
			  htim2.Instance->CCR4 = 55;
			  HAL_Delay(2000);
			  work = 1;
		  }
		  if(i == qty && work == 1)
		  {
			  sprintf(buff, "%03d", i);
			  ILI9341_Draw_Text(buff, 190, 170, BLACK, 2, 0xFFEE);
			  textStatus("Complete");
		  }
		  else
		  {
			  sprintf(buff, "%03d", i);
			  ILI9341_Draw_Text(buff, 190, 170, BLACK, 2, 0xFFEE);
		  }
		  ILI9341_Draw_Text("---Press Any to Continue---", 70, 205, BLACK, 1, WHITE);
	  }

	  htim2.Instance->CCR4 = 55;
	  ok = 0;
	  cancel = 0;
	  start = 0;
	  pause = 0;

	  while(1)
	  {
		  if(TP_Touchpad_Pressed())
		  {
			  break;
		  }
	  }

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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{

}

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
