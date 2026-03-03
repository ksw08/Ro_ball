#include "motor.h"
#include "stdio.h"
#include "gpio.h"
#include "tim.h"

void frontmove(){
  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_RESET);
}

void backmove(){
  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_SET);
}

void stopmove(){
  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_RESET);
}

void slcspeed(int spd){
  switch(spd){
    case 10: htim2.Instance->CCR1 = 10-1; break;
    case 20: htim2.Instance->CCR1 = 20-1; break;
    case 30: htim2.Instance->CCR1 = 30-1; break;
    case 40: htim2.Instance->CCR1 = 40-1; break;
    case 50: htim2.Instance->CCR1 = 50-1; break;
    case 60: htim2.Instance->CCR1 = 60-1; break;
    case 70: htim2.Instance->CCR1 = 70-1; break;
    case 80: htim2.Instance->CCR1 = 80-1; break;
    case 90: htim2.Instance->CCR1 = 90-1; break;
    default: htim2.Instance->CCR1 = 50-1; break;
  }
}



// void dynamic_Init(){
//   uint32_t angle = 0x205;
//   uint8_t p2 = angle&0xff;
//   uint8_t p3 = (angle>>8)&0xff; // 초기 각도값 > 150(중앙)으로 맞춤

//   uint8_t head = 0xff;
//   uint8_t id = 0x01;
//   uint8_t len = 0x05;
//   uint8_t inst = 0x03;
//   uint8_t p1 = 0x1E;
//   uint8_t chksum = ~(id+len+inst+p1+p2+p3);
//   uint8_t normal[9] = {head,head,id,len,inst,p1,p2,p3,chksum}; // 초기 각도 값
//   uint8_t test1[9] = {0};
//   uint8_t trq = 0x24;
//   uint8_t trqdeb = 0x00;
//   uint8_t trqsum = ~(id+0x04+inst+trq+trqdeb);
//   uint8_t torqueoff[8] = {0xff,0xff,0x01,0x04,0x03,trq,trqdeb,trqsum};
// }

  
  // while(1) {
  //  if(speed>0x3fe){
  //   speed = 0;
  //  }
  //  else{
  //   speed+=10;
  //   p2 = test[6] = speed&0xff;
  //   p3 = test[7] = (speed>>8)&0xff;
  //   test[8] = ~(id+len+inst+p1+p2+p3);
  //  }
  //   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_SET);
  //   HAL_UART_Transmit(&huart3,test,9,30);
  //   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_RESET);
  //   HAL_Delay(10);
  // }

  //     HAL_I2C_Mem_Read(&hi2c3, 0x50<<1, 0x3d, I2C_MEMADD_SIZE_8BIT, &get_xyz[0], 6, 10);
  //   //tx.Print("%d\n",counter); 엔코더 값 받기
  //   //HAL_Delay(100);
  //   gyro_int_x = ((get_xyz[1]<<8)|get_xyz[0]);
  //   gyro_int_y = ((get_xyz[3]<<8)|get_xyz[2]);
  //   gyro_int_z = ((get_xyz[5]<<8)|get_xyz[4]);
  //   gyro_x = (double)gyro_int_x / 32768*180;
  //   gyro_y = (double)gyro_int_y / 32768*180;
  //   gyro_z = (double)gyro_int_z / 32768*180;
  //   tx.Print("Y:%d\n",(int)gyro_y);


