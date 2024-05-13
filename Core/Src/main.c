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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdbool.h"
RTC_HandleTypeDef hrtc;
#include "liquidcrystal_i2c.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

RTC_HandleTypeDef hrtc;

TIM_HandleTypeDef htim1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

char time[10];///stores time displayed
char date[10];

///when time reaches the alarm time, value goes to 1, used for if statements
///when alarm turned off variable goes back to 0
uint8_t alarmA=0;
uint8_t alarmB=0;

///1 if alarm A is in use
int alarmA_on=0;
int alarmB_on=0;
int alarmC_on=0;

uint8_t sound_on=0;
uint8_t all_open=0;

///stores alarm time displayed while alarm time is being changed
char alarm1[10];
char alarm2[10];
char alarm3[10];
char time_change[10];

///open is false, close is true
bool dir=true;

uint8_t motor=0;

///hour being changed 0, minute being changed 1
int a1_hm=0;
int a2_hm=0;
int a3_hm=0;
int time_hm=0;

/// 0=current time,1=change alarm 1, 2=change alarm2
/// 3=change alarm 3, 4=change current time
uint8_t state=0;


int zero=0;
///stores hour of alarm 1
int a1h=0;
///stores minute of alarm 1
int a1m=0;
///stores  copy of alarm 1 hour, a1h is changed to value of a1h_temp when user presses enter
int a1h_temp=0;
int a1m_temp=0;


int a2h=0;
int a2m=0;
int a2h_temp=0;
int a2m_temp=0;


int a3h=0;
int a3m=0;
int a3h_temp=0;
int a3m_temp=0;

int timeh_temp=0;
int timem_temp=0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_RTC_Init(void);
static void MX_TIM1_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void delay (uint16_t us)
{
  __HAL_TIM_SET_COUNTER(&htim1, 0);
  while (__HAL_TIM_GET_COUNTER(&htim1) < us);
}

#define stepsperrev 4096

void stepper_set_rpm (int rpm)  // Set rpm--> max 13, min 1,,,  went to 14 rev/min
{
	delay(60000000/stepsperrev/rpm);
}


void stepper_half_drive_1 (int step)
{
	switch (step){
		case 0:
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);   // IN1
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);   // IN2
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);   // IN3
			  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);   // IN4
			  break;

		case 1:
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);   // IN1
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);   // IN2
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);   // IN3
			  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);   // IN4
			  break;

		case 2:
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);   // IN1
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);   // IN2
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);   // IN3
			  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);   // IN4
			  break;

		case 3:
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);   // IN1
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);   // IN2
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);   // IN3
			  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);   // IN4
			  break;

		case 4:
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);   // IN1
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);   // IN2
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);   // IN3
			  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);   // IN4
			  break;

		case 5:
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);   // IN1
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);   // IN2
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);   // IN3
			  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);   // IN4
			  break;

		case 6:
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);   // IN1
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);   // IN2
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);   // IN3
			  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);   // IN4
			  break;

		case 7:
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);   // IN1
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);   // IN2
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);   // IN3
			  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);   // IN4
			  break;

		}
}

void stepper_half_drive_2 (int step)
{
	switch (step){
		case 0:
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);   // IN1
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);   // IN2
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);   // IN3
			  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);   // IN4
			  break;

		case 1:
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);   // IN1
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);   // IN2
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);   // IN3
			  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);   // IN4
			  break;

		case 2:
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);   // IN1
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);   // IN2
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);   // IN3
			  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);   // IN4
			  break;

		case 3:
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);   // IN1
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);   // IN2
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);   // IN3
			  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);   // IN4
			  break;

		case 4:
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);   // IN1
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);   // IN2
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);   // IN3
			  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);   // IN4
			  break;

		case 5:
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);   // IN1
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);   // IN2
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);   // IN3
			  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);   // IN4
			  break;

		case 6:
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);   // IN1
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);   // IN2
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);   // IN3
			  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);   // IN4
			  break;

		case 7:
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);   // IN1
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);   // IN2
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);   // IN3
			  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);   // IN4
			  break;

		}
}

void stepper_half_drive_3 (int step)
{
	switch (step){
		case 0:
			  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);   // IN1
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);   // IN2
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);   // IN3
			  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);   // IN4
			  break;

		case 1:
			  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);   // IN1
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);   // IN2
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);   // IN3
			  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);   // IN4
			  break;

		case 2:
			  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);   // IN1
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);   // IN2
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);   // IN3
			  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);   // IN4
			  break;

		case 3:
			  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);   // IN1
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);   // IN2
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);   // IN3
			  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);   // IN4
			  break;

		case 4:
			  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);   // IN1
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);   // IN2
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);   // IN3
			  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);   // IN4
			  break;

		case 5:
			  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);   // IN1
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);   // IN2
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);   // IN3
			  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);   // IN4
			  break;

		case 6:
			  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);   // IN1
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);   // IN2
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);   // IN3
			  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);   // IN4
			  break;

		case 7:
			  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);   // IN1
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);   // IN2
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);   // IN3
			  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);   // IN4
			  break;

		}
}

void stepper_step_angle_1 (float angle, int direction, int rpm)
{
	float anglepersequence = 0.703125;  // 360 = 512 sequences
	int numberofsequences = (int) (angle/anglepersequence);

	for (int seq=0; seq<numberofsequences; seq++)
	{
		if (direction == 0)  // for clockwise
		{
			for (int step=7; step>=0; step--)
			{
				stepper_half_drive_1(step);
				stepper_set_rpm(rpm);
			}

		}

		else if (direction == 1)  // for anti-clockwise
		{
			for (int step=0; step<8; step++)
			{
				stepper_half_drive_1(step);
				stepper_set_rpm(rpm);
			}
		}
	}
}

void stepper_step_angle_2 (float angle, int direction, int rpm)
{
	float anglepersequence = 0.703125;  // 360 = 512 sequences
	int numberofsequences = (int) (angle/anglepersequence);

	for (int seq=0; seq<numberofsequences; seq++)
	{
		if (direction == 0)  // for clockwise
		{
			for (int step=7; step>=0; step--)
			{
				stepper_half_drive_2(step);
				stepper_set_rpm(rpm);
			}

		}

		else if (direction == 1)  // for anti-clockwise
		{
			for (int step=0; step<8; step++)
			{
				stepper_half_drive_2(step);
				stepper_set_rpm(rpm);
			}
		}
	}
}

void stepper_step_angle_3 (float angle, int direction, int rpm)
{
	float anglepersequence = 0.703125;  // 360 = 512 sequences
	int numberofsequences = (int) (angle/anglepersequence);

	for (int seq=0; seq<numberofsequences; seq++)
	{
		if (direction == 0)  // for clockwise
		{
			for (int step=7; step>=0; step--)
			{
				stepper_half_drive_3(step);
				stepper_set_rpm(rpm);
			}

		}

		else if (direction == 1)  // for anti-clockwise
		{
			for (int step=0; step<8; step++)
			{
				stepper_half_drive_3(step);
				stepper_set_rpm(rpm);
			}
		}
	}
}



void set_time(uint8_t hour, uint8_t minute)
{

	  RTC_TimeTypeDef sTime = {0};
	  RTC_DateTypeDef sDate = {0};
	  /** Initialize RTC and set the Time and Date
	  */

	  sTime.Hours = hour;
	  sTime.Minutes = minute;
	  sTime.Seconds = 0x00;
	  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
	  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
	  sDate.Month = RTC_MONTH_NOVEMBER;
	  sDate.Date = 0x8;
	  sDate.Year = 0x23;

	  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
	  {
	    Error_Handler();
	  }

	  HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0x32F2);

}




void get_time(void)
{
	RTC_DateTypeDef gDate;
	RTC_TimeTypeDef gTime;
	/* Get the RTC current Time */
	 HAL_RTC_GetTime(&hrtc, &gTime, RTC_FORMAT_BIN);
	/* Get the RTC current Date */
	 HAL_RTC_GetDate(&hrtc, &gDate, RTC_FORMAT_BIN);
	/* Display time Format: hh:mm:ss */
	 sprintf((char*)time,"%02d:%02d:%02d",gTime.Hours, gTime.Minutes, gTime.Seconds);
	/* Display date Format: dd-mm-yy */
	 sprintf((char*)date,"%02d-%02d-%2d",gDate.Date, gDate.Month, 2000 + gDate.Year);
}

/*
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
	HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5);
	HD44780_Clear();
	HD44780_SetCursor(0,0);
	HD44780_PrintStr("Alarm A on");
}
*/

uint8_t check_Alarm(void)
{
	RTC_TimeTypeDef gTime;
	HAL_RTC_GetTime(&hrtc, &gTime, RTC_FORMAT_BIN);
	if(alarmA_on==1)
	{
		if(gTime.Hours==a1h&&gTime.Minutes==a1m)
		{
				if(gTime.Seconds==zero)
				{
					 motor=1;
					 return 1;
				}
		}
	}
	if(alarmB_on==1)
	{
		if(gTime.Hours==a2h)
		{
			if(gTime.Minutes==a2m)
			{
				if(gTime.Seconds==zero)
				{
					motor=2;
					return 1;
				}
			}
		}
	}

	if(alarmC_on==1)
	{
		if(gTime.Hours==a3h)
		{
			if(gTime.Minutes==a3m)
			{
				if(gTime.Seconds==zero)
				{
					motor=3;
					return 1;
				}
			}
		}
	}
	 return 0;
}

void stepper_motor(uint8_t m,uint8_t open)
{
	if(m==1)
	{
		stepper_step_angle_1(90,open,7);
	}
	else if(m==2)
	{
		stepper_step_angle_2(90,open,7);
	}
	else if(m==3)
	{
		stepper_step_angle_3(90,open,7);
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
  MX_USART2_UART_Init();
  MX_RTC_Init();
  MX_TIM1_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start(&htim1);

  HD44780_Init(2);
  HD44780_Clear();
  HD44780_SetCursor(0,0);
  ///11:16
  set_time(17,22);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		if(check_Alarm()==1&&sound_on==0)
		{
			 HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_10);
			 stepper_motor(motor,0);
			 sound_on=1;
		}

		if(state==0)
		{

			get_time();
			HD44780_SetCursor(0,0);
			HD44780_PrintStr(time);
			HAL_Delay(500);
		}
		else if(state==1)///change alarm 1
		{

			HD44780_SetCursor(0,0);
			sprintf((char*)alarm1,"%02d-%02d",a1h_temp, a1m_temp);
			HD44780_PrintStr(alarm1);
			HD44780_SetCursor(0,1);
			HD44780_PrintStr("Alarm 1 time");
			HAL_Delay(500);
		}
		else if(state==2)
		{
			HD44780_SetCursor(0,0);
			sprintf((char*)alarm2,"%02d-%02d",a2h_temp, a2m_temp);
			HD44780_PrintStr(alarm2);
			HD44780_SetCursor(0,1);
			HD44780_PrintStr("Alarm 2 time");
			HAL_Delay(500);
		}
		else if(state==3)
		{
			HD44780_SetCursor(0,0);
			sprintf((char*)alarm3,"%02d-%02d",a3h_temp, a3m_temp);
			HD44780_PrintStr(alarm3);
			HD44780_SetCursor(0,1);
			HD44780_PrintStr("Alarm 3 time");
			HAL_Delay(500);
		}
		else if(state==4)
		{
			HD44780_SetCursor(0,0);
			sprintf((char*)time_change,"%02d-%02d",timeh_temp, timem_temp);
			HD44780_PrintStr(time_change);
			HD44780_SetCursor(0,1);
			HD44780_PrintStr("Change time");
			HAL_Delay(500);
		}



		if(state==0)
		{
			if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)==1)
					{
						if(sound_on==1)
						{
							HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_10);
							sound_on=0;
							stepper_motor(motor,1);
							HD44780_Clear();
							HD44780_SetCursor(0,0);
							HD44780_PrintStr("alarm off");
							HAL_Delay(1000);
							HD44780_Clear();
						}
						else if(sound_on==0)
						{
							if(all_open==0)
							{
								stepper_motor(1,0);
								stepper_motor(2,0);
								stepper_motor(3,0);
								all_open=1;
							}
							else if(all_open==1)
							{
								stepper_motor(1,1);
								stepper_motor(2,1);
								stepper_motor(3,1);
								all_open=0;
							}
						}
					}
		}





		///button 1, changes between different state of the display
		if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_1)==1)
		{
			state=(state+1)%5;
			HD44780_Clear();
			if(state==1)
			{
				a1_hm=0;
				a1h_temp=a1h;
				a1m_temp=a1m;
			}
			else if(state==2)
			{
				a2_hm=0;
				a2h_temp=a2h;
				a2m_temp=a2m;
			}
			else if(state==3)
			{
				a3_hm=0;
				a3h_temp=a3h;
				a3m_temp=a3m;
			}
			else if(state==4)
			{
				time_hm=0;
			}
		}


		if(state==1)
		{
			///user press down button while on alarm 1 page
			if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_0)==1)
			{
				if(a1_hm==0)
				{
					if(a1h_temp==0)
					{
					  a1h_temp=23;
					}
					else
					{
					  a1h_temp--;
					}
				}
				else if(a1_hm==1)
				{
					if(a1m_temp==0)
					{
					  a1m_temp=59;
					}
					else
					{
					  a1m_temp--;
					}
				}
			}
			///user press up button while on alarm 1 page
			if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_4)==1)
			{
				if(a1_hm==0)
				{
					a1h_temp=(a1h_temp+1)%24;
				}
				else if(a1_hm==1)
				{
					a1m_temp=(a1m_temp+1)%60;
				}
			}
			///user press delete button while on alarm 1 page
			///alarm is deleted by storing the state of alarm 1 in alarmA_on, confirmation messaqe is displayed
			if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5)==1)
			{
				alarmA_on=0;
				state=0;///changes display back to displaying time
				HD44780_Clear();
				HD44780_SetCursor(0,0);
				HD44780_PrintStr("Alarm A Deleted");
				HAL_Delay(1000);
				HD44780_Clear();
			}
			///user press enter button while on alarm 1 page
			///User goes from changing hour to minute or Alarm time is changed and confirmation message is displayed
			if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)==1)
			{
				if(a1_hm==0)
				{
					a1_hm++;///goes from changing hour to minute
				}
				else
				{
					alarmA_on=1;///turns alarm A on
					a1h=a1h_temp;///copies the value from temp in to the actual alarm variable
					a1m=a1m_temp;
					state=0;///changes display back to displaying time
					///print message
					HD44780_Clear();
					HD44780_SetCursor(0,0);
					HD44780_PrintStr("Alarm A");
					HD44780_SetCursor(0,1);
					HD44780_PrintStr("Changed");
					HAL_Delay(1000);
					HD44780_Clear();
				}
			}
		}


		if(state==2)
			{
				///user press down button while on alarm 2 page
				if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_0)==1)
				{
					if(a2_hm==0)
					{
						if(a2h_temp==0)
						{
						  a2h_temp=23;
						}
						else
						{
						  a2h_temp--;
						}
					}
					else if(a2_hm==1)
					{
						if(a2m_temp==0)
						{
						  a2m_temp=59;
						}
						else
						{
						  a2m_temp--;
						}
					}
				}
				///user press up button while on alarm 2 page
				if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_4)==1)
				{
					if(a2_hm==0)
					{
						a2h_temp=(a2h_temp+1)%24;
					}
					else if(a2_hm==1)
					{
						a2m_temp=(a2m_temp+1)%60;
					}
				}
				///user press delete button while on alarm 2 page
				///alarm is deleted by storing the state of alarm 2 in alarmB_on, confirmation messaqe is displayed
				if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5)==1)
				{
					alarmB_on=0;
					state=0;///changes display back to displaying time
					HD44780_Clear();
					HD44780_SetCursor(0,0);
					HD44780_PrintStr("Alarm B Deleted");
					HAL_Delay(1000);
					HD44780_Clear();
				}
				///user press enter button while on alarm 2 page
				///User goes from changing hour to minute or Alarm time is changed and confirmation message is displayed
				if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)==1)
				{
					if(a2_hm==0)
					{
						a2_hm++;///goes from changing hour to minute
					}
					else
					{
						alarmB_on=1;///turns alarm B on
						a2h=a2h_temp;///copies the value from temp in to the actual alarm variable
						a2m=a2m_temp;
						state=0;///changes display back to displaying time
						///print message
						HD44780_Clear();
						HD44780_SetCursor(0,0);
						HD44780_PrintStr("Alarm B");
						HD44780_SetCursor(0,1);
						HD44780_PrintStr("Changed");
						HAL_Delay(1000);
						HD44780_Clear();
					}
				}
			}

		if(state==3)
		{
			///user press down button while on alarm 3 page
			if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_0)==1)
			{
				if(a3_hm==0)
				{
					if(a3h_temp==0)
					{
					  a3h_temp=23;
					}
					else
					{
					  a3h_temp--;
					}
				}
				else if(a3_hm==1)
				{
					if(a3m_temp==0)
					{
					  a3m_temp=59;
					}
					else
					{
					  a3m_temp--;
					}
				}
			}
			///user press up button while on alarm 2 page
			if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_4)==1)
			{
				if(a3_hm==0)
				{
					a3h_temp=(a3h_temp+1)%24;
				}
				else if(a3_hm==1)
				{
					a3m_temp=(a3m_temp+1)%60;
				}
			}
			///user press delete button while on alarm 3 page
			///alarm is deleted by storing the state of alarm 3 in alarmC_on, confirmation messaqe is displayed
			if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5)==1)
			{
				alarmC_on=0;
				state=0;///changes display back to displaying time
				HD44780_Clear();
				HD44780_SetCursor(0,0);
				HD44780_PrintStr("Alarm C Deleted");
				HAL_Delay(1000);
				HD44780_Clear();
			}
			///user press enter button while on alarm 3 page
			///User goes from changing hour to minute or Alarm time is changed and confirmation message is displayed
			if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)==1)
			{
				if(a3_hm==0)
				{
					a3_hm++;///goes from changing hour to minute
				}
				else
				{
					alarmC_on=1;///turns alarm A on
					a3h=a3h_temp;///copies the value from temp in to the actual alarm variable
					a3m=a3m_temp;
					state=0;///changes display back to displaying time
					///print message
					HD44780_Clear();
					HD44780_SetCursor(0,0);
					HD44780_PrintStr("Alarm C");
					HD44780_SetCursor(0,1);
					HD44780_PrintStr("Changed");
					HAL_Delay(1000);
					HD44780_Clear();
				}
			}
		}


		if(state==4)
		{
			///user press down button while on change time page
			if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_0)==1)
			{
				if(time_hm==0)
				{
					if(timeh_temp==0)
					{
					  timeh_temp=23;
					}
					else
					{
					  timeh_temp--;
					}
				}
				else if(time_hm==1)
				{
					if(timem_temp==0)
					{
					  timem_temp=59;
					}
					else
					{
					  timem_temp--;
					}
				}
			}
			///user press up button while on change time page
			if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_4)==1)
			{
				if(time_hm==0)
				{
					timeh_temp=(timeh_temp+1)%24;
				}
				else if(time_hm==1)
				{
					timem_temp=(timem_temp+1)%60;
				}
			}

			///user press enter button while on change time page
			///User goes from changing hour to minute or Alarm time is changed and confirmation message is displayed
			if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)==1)
			{
				if(time_hm==0)
				{
					time_hm++;///goes from changing hour to minute
				}
				else
				{
					set_time(timeh_temp/10*16+timeh_temp%10,timem_temp/10*16+timem_temp%10);///set alarm (hour,minute)
					state=0;///changes display back to displaying time
					///print message
					HD44780_Clear();
					HD44780_SetCursor(0,0);
					HD44780_PrintStr("Time");
					HD44780_SetCursor(0,1);
					HD44780_PrintStr("Changed");
					HAL_Delay(1000);
					HD44780_Clear();
				}
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};
  RTC_AlarmTypeDef sAlarm = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0x10;
  sTime.Minutes = 0x20;
  sTime.Seconds = 0x30;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_SUNDAY;
  sDate.Month = RTC_MONTH_NOVEMBER;
  sDate.Date = 0x5;
  sDate.Year = 0x23;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable the Alarm A
  */
  sAlarm.AlarmTime.Hours = 0x12;
  sAlarm.AlarmTime.Minutes = 0x1;
  sAlarm.AlarmTime.Seconds = 0x2;
  sAlarm.AlarmTime.SubSeconds = 0x0;
  sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
  sAlarm.AlarmMask = RTC_ALARMMASK_NONE;
  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  sAlarm.AlarmDateWeekDay = 0x1;
  sAlarm.Alarm = RTC_ALARM_A;
  if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable the Alarm B
  */
  sAlarm.AlarmTime.Hours = 0x0;
  sAlarm.AlarmTime.Minutes = 0x0;
  sAlarm.AlarmTime.Seconds = 0x0;
  sAlarm.Alarm = RTC_ALARM_B;
  if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 72-1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 0xffff-1;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5
                          |GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9
                          |GPIO_PIN_10, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_10|GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC13 PC0 PC1 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA1 PA4 PA5
                           PA6 PA7 PA8 PA9
                           PA10 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5
                          |GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9
                          |GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB10 PB6 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_10|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PC7 */
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PB3 PB4 PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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
