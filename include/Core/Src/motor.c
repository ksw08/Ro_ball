#include "stdio.h"
#include "gpio.h"


void frontmove(){
  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_RESET);
}

void backmove(){
  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_SET);
}