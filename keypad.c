/*
 * Keypad.c
 *
 * Created on: Apr 12, 2023
 * Author: alec.cheng
 *
 */

 #include "stm32l4xx.h"

 #define COL_PORT GPIOC
 #define ROW_PORT GPIOB
 #define ROW_MASK 240
 #define COL_MASK 240
 
 void keypad_config(void)
 {
     RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
 
     COL_PORT->MODER &= ~(GPIO_MODER_MODE4);
     COL_PORT->MODER &= ~(GPIO_MODER_MODE5);
     COL_PORT->MODER &= ~(GPIO_MODER_MODE6);
     COL_PORT->MODER &= ~(GPIO_MODER_MODE7);
 
     COL_PORT->MODER |= (1 << GPIO_MODER_MODE4_Pos);
     COL_PORT->MODER |= (1 << GPIO_MODER_MODE5_Pos);
     COL_PORT->MODER |= (1 << GPIO_MODER_MODE6_Pos);
     COL_PORT->MODER |= (1 << GPIO_MODER_MODE7_Pos);
 
     COL_PORT->OTYPER &= ~(GPIO_OTYPER_OT4);
     COL_PORT->OTYPER &= ~(GPIO_OTYPER_OT5);
     COL_PORT->OTYPER &= ~(GPIO_OTYPER_OT6);
     COL_PORT->OTYPER &= ~(GPIO_OTYPER_OT7);
 
     GPIOC->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED4);
     GPIOC->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED5);
     GPIOC->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED6);
     GPIOC->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED7);
 
     GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPD4);
     GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPD5);
     GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPD6);
     GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPD7);
 
     GPIOC->ODR &= ~(GPIO_ODR_OD4);
     GPIOC->ODR &= ~(GPIO_ODR_OD5);
     GPIOC->ODR &= ~(GPIO_ODR_OD6);
     GPIOC->ODR &= ~(GPIO_ODR_OD7);
 
     RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
 
     ROW_PORT->MODER &= ~(GPIO_MODER_MODE4);
     ROW_PORT->MODER &= ~(GPIO_MODER_MODE5);
     ROW_PORT->MODER &= ~(GPIO_MODER_MODE6);
     ROW_PORT->MODER &= ~(GPIO_MODER_MODE7);
 
     ROW_PORT->PUPDR &= ~(GPIO_PUPDR_PUPD4);
     ROW_PORT->PUPDR &= ~(GPIO_PUPDR_PUPD5);
     ROW_PORT->PUPDR &= ~(GPIO_PUPDR_PUPD6);
     ROW_PORT->PUPDR &= ~(GPIO_PUPDR_PUPD7);
 
     ROW_PORT->PUPDR |= (2 << GPIO_PUPDR_PUPD4_Pos);
     ROW_PORT->PUPDR |= (2 << GPIO_PUPDR_PUPD5_Pos);
     ROW_PORT->PUPDR |= (2 << GPIO_PUPDR_PUPD6_Pos);
     ROW_PORT->PUPDR |= (2 << GPIO_PUPDR_PUPD7_Pos);
 }
 
 int32_t keypad_read()
 {
     int32_t rows = ROW_PORT->IDR | ROW_MASK;
     int8_t offset = 0;
     int8_t button = -1;
 
     if (rows == 0) {
         return button;
     }
     else {
         int32_t cols = 0;
         while (cols < 4)
         {
             COL_PORT->ODR |= (GPIO_ODR_OD4 << cols);
             rows = (ROW_PORT->IDR & ROW_MASK) >> 4;
 
             if (rows > 0) {
                 while (rows > 0) {
                     rows = rows >> 1;
                     offset++;
                 }
 
                 button = cols + 4 * (offset - 1);
 
                 if (button == 0) {
                     button = 1;
                 }
                 else if (button == 1) {
                     button = 2;
                 }
                 else if (button == 2) {
                     button = 3;
                 }
                 else if (button == 3) {   // A
                     button = 12;
                 }
                 else if (button == 4) {
                     button = 4;
                 }
                 else if (button == 5) {
                     button = 5;
                 }
                 else if (button == 6) {
                     button = 6;
                 }
                 else if (button == 7) {   // B
                     button = 13;
                 }
                 else if (button == 8) {
                     button = 7;
                 }
                 else if (button == 9) {
                     button = 8;
                 }
                 else if (button == 10) {
                     button = 9;
                 }
                 else if (button == 11) {
                     button = 14;
                 }
                 else if (button == 12) {  // *
                     button = 10;
                 }
                 else if (button == 13) {
                     button = 0;
                 }
                 else if (button == 14) {  // #
                     button = 11;
                 }
                 else if (button == 15) {  // D
                     button = 15;
                 }
 
                 COL_PORT->ODR &= ~(GPIO_ODR_OD4 << cols);
                 return button;
             }
 
             COL_PORT->ODR &= ~(GPIO_ODR_OD4 << cols);
             cols++;
         }
     }
 
     return button;
 }