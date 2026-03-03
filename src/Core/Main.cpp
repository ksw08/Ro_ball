#include "main.h"
#include "stdio.h"
#include "gpio.h"
#include "spi.h"
#include "usart.h"
#include "stm32g4xx_it.h"
#include "tim.h"
#include "i2c.h"
#include "Interface/HCMSDisplay.h"
#include "Interface/UartPrint.h"
#include <iomanip>
#include "motor.h"
#include "Device.h"


void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

int16_t gyro_int_x = 0;
int16_t gyro_int_y = 0;
int16_t gyro_int_z = 0;
double gyro_x = 0;
double gyro_y = 0;
double gyro_z = 0;
uint8_t get_xyz[6] = {0};

////////////////////////////////////////////////////////////////////////
//dynamic packet
uint32_t angle = 0x215;//0x25B; // 170 중앙 280
uint32_t anglespeed = 0x3E8;
uint8_t p2 = angle&0xff;
uint8_t p3 = (angle>>8)&0xff;
uint8_t p4 = anglespeed&0xff;
uint8_t p5 = (anglespeed>>8)&0xff;

uint8_t head = 0xff;
uint8_t id = 0x01;
uint8_t len = 0x07;
uint8_t inst = 0x03;
uint8_t p1 = 0x1E;
uint8_t chksum = ~(id+len+inst+p1+p2+p3+p4+p5);
uint8_t test[11] = {head,head,id,len,inst,p1,p2,p3,p4,p5,chksum};
uint8_t test1[9] = {0};
uint8_t trq = 0x24;
uint8_t trqdeb = 0x00;
uint8_t trqsum = ~(id+0x04+inst+trq+trqdeb);
uint8_t torqueoff[8] = {0xff,0xff,0x01,0x04,0x03,trq,trqdeb,trqsum};
///////////////////////////////////////////////////////////////////////
//dc motor pid parameter
double now = 0;
double sensum = 0;
int num = 0; // 인터럽트 발생 횟수 측정
int Tk = 0; // 엔코더 값
int16_t tick = 0; // 엔코더 값 변환
int dir = 0; // 엔코더 방향 측정
double mp = 0.5;
double mi = 0.0000005;
double md = 0.004;
double motor_p = 0;
double motor_i = 0;
double motor_d = 0;
double M_PID = 0;
double goalpoint = 0;
double motorprev = 0;
double motorerr = goalpoint - tick;
double motorerrdiff = tick - motorprev;
///////////////////////////////////////////////////////////////////////
//dynamic pid parameter
  double kp = 0.25; // 튜닝 // 0.5 //0.4
  double ki = 0.001; // 튜닝 // 0.07
  double kd = 0.12; // 튜닝 // 0.13 //0.1
  // double now = 0; // 현재 output
  double prev = 0; // 전 output
  double goal = 0; // 목표 값
  double err = goal - now; // 전 출력값
  double t = 0.02; // imu interrupt 주기
  double dt = 0.002;
  double P = kp + err;
  double I = ki*err*t;
  double ERRdiff = now - prev;
  double D = ERRdiff / t * kd;
  double output = P+I+D;
  double I_err;
  double maxangle = 0x2B2;
  double minangle = 0x169;
  uint8_t pidtest[1] = {0};
  /////////////////////////////////////////////////////

uint8_t blue_get[1] = {0};

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
   if((blue_get[0] == 0) || (blue_get[0] == 48)){
    goal = 0;
      //HAL_TIM_Base_Start_IT(&htim4);
      // HAL_TIM_Encoder_Start(&htim1,TIM_CHANNEL_1);
      // dir = __HAL_TIM_IS_TIM_COUNTING_DOWN(&htim1); // dir 0: front, dir 1: back
      // if(tick>374||tick<-374){
      // TIM1->CNT = 0;
      // }
      // tick = (int16_t)(TIM1->CNT)/2;
      // motorerr = goalpoint - tick;
      // motorerrdiff = tick - motorprev;
      // motorprev = tick;
      // motor_p = mp*motorerr;
      // motor_i += mi*motorerr*0.01;
      // motor_d = md*motorerrdiff/0.01;
      // M_PID = motor_p + motor_i + motor_d; 

      // if(M_PID > 0){
      //   frontmove();
      // }else if(M_PID < 0){
      //   backmove();
      // }
      stopmove();

    } 
    if(blue_get[0] == 85){ //U
      //HAL_TIM_Encoder_Stop(&htim1,TIM_CHANNEL_1);
      goal = 0;
      frontmove();
      
    }
    if(blue_get[0] == 68){ //D
      //HAL_TIM_Encoder_Stop(&htim1,TIM_CHANNEL_1);
      goal = 0;
      backmove();
    }
    if(blue_get[0] == 82){ // R
      //HAL_TIM_Encoder_Stop(&htim1,TIM_CHANNEL_1);
      goal = -30;
      frontmove();
    }
    if(blue_get[0] == 76){ // L
      goal = 30;
      // //HAL_TIM_Encoder_Stop(&htim1,TIM_CHANNEL_1);
      frontmove();
    }
    HAL_UART_Receive_IT(&huart2,blue_get,1);
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){

  if(htim->Instance == TIM4){ // imu sensor
    HAL_I2C_Mem_Read(&hi2c3, 0x50<<1, 0x3d, I2C_MEMADD_SIZE_8BIT, &get_xyz[0], 6,10); // sensor_get

    gyro_int_y = (int16_t)((get_xyz[3]<<8)|get_xyz[2]);
    now = (double)gyro_int_y / 32768*180;

    if(num<9){
      sensum+=now;
      prev = now;
      num++;
    }
    else if(num>=9){
      now = sensum / 9;
      err = goal - now;
      ERRdiff = now - prev;
      P = kp*err;
      I_err = err*t;
      I += ki*I_err;
      D = kd*ERRdiff/dt;

      if((-1 < I) && (I < 1)){
        I = 0;
      }
      if(I>7){ //5
        I = 7;
      }else if(I<-7){
        I = -7;
      }
     
      output = P+I+D;
      num = 0;
      sensum = 0;
    }
  }

}


int main(void)
{
  HAL_Init();
  SystemClock_Config();

  MX_GPIO_Init();
  MX_SPI3_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init(); // bluetooth
  MX_USART3_UART_Init(); // dynamic
  MX_TIM1_Init(); //encoder
  MX_TIM2_Init(); //DC pwm
  MX_TIM3_Init(); //bluetooth interrupt
  MX_TIM4_Init(); //imu sensor interrupt
  MX_I2C3_Init(); //imu sensor

  HCMSDisplay vfd(
    GPIOE ,GPIO_PIN_5,
    GPIOB ,GPIO_PIN_6,
    GPIOB ,GPIO_PIN_7,
    GPIOE ,GPIO_PIN_6,
    GPIOB ,GPIO_PIN_9
  );

  Device::tx = new UartPrint(&huart1, 115200);

  vfd.Print("Hi bart");

  Device::tx -> Print("Welcome to the M4\n");
  HAL_Delay(10);
  uint8_t blue_get[1] = {0};
  uint8_t senout = 0;
  
  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_SET);
  
  HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_6,GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_SET);
  HAL_UART_Transmit(&huart3,test,11,30);
  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_RESET);
  //HAL_TIM_Encoder_Start(&htim1,TIM_CHANNEL_1); // encoder interrupt start
  //HAL_TIM_Base_Start_IT(&htim3); // bluetooth interrupt start
  
  HAL_TIM_Base_Start_IT(&htim4); // imu sensor interrupt start
  HAL_UART_Receive_IT(&huart2,blue_get,1); // bluetooth

  HAL_UART_Transmit(&huart3,test,11,10);

  while (1)
  {  

    senout = (uint8_t)output;
    if(goal == 0){
      angle = (uint32_t)((int)(0x215) - (int)(output/0.29));
      }
    if(goal == -30){//L
      angle = (uint32_t)((int)(0x27D) - (int)(output/0.29));
      }
    if(goal == 30){//R
      angle = (uint32_t)((int)(0x18C) - (int)(output/0.29));
      }
      if(angle > maxangle){
        angle = maxangle;
      }else if (angle < minangle){
        angle = minangle;
      }
      p2 = test[6] = angle&0xff;
      p3 = test[7] = (angle>>8)&0xff;
      chksum = test [10] = ~(id+len+inst+p1+p2+p3+p4+p5);

      HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_SET);
      HAL_UART_Transmit(&huart3,test,11,10);
      HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_RESET);
      


  }
}

void Error_Handler(void) // 에러 발생 시 이동되는 함수
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
