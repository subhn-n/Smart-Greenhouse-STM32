#include "projet.h"
#include "algo.h"
#include "stm32l4xx.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>



extern UART_HandleTypeDef huart1;
uint8_t rx;


void setup(){
	LED_PWM_Start();
}



//void loop(){
//    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
//    HAL_Delay(500);
//
//    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
//    HAL_Delay(500);
//}

char buffer[20];
int i = 0;

void loop(void){


    if (HAL_UART_Receive(&huart1, &rx, 1, 10) == HAL_OK)
    {
        HAL_UART_Transmit(&huart1, &rx, 1, HAL_MAX_DELAY); // echo debug

        if (i < 19)
        {
            buffer[i++] = rx;
            buffer[i] = '\0';
        }

        if (strcmp(buffer, "LED_ON") == 0)
        {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
            i = 0;
            buffer[0] = '\0';
        }

        if (strcmp(buffer, "LED_OFF") == 0)
        {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
            i = 0;
            buffer[0] = '\0';
        }

        if (strcmp(buffer, "PUMP_ON") == 0)
        {
            PUMP_ON();
            i = 0;
            buffer[0] = '\0';
        }

        if (strcmp(buffer, "PUMP_OFF") == 0)
        {
        	PUMP_OFF();
            i = 0;
            buffer[0] = '\0';
        }

        if (strcmp(buffer, "FAN_ON") == 0)
        {
            FAN_ON();
            i = 0;
            buffer[0] = '\0';
        }

        if (strcmp(buffer, "FAN_OFF") == 0)
        {
            FAN_OFF();
            i = 0;
            buffer[0] = '\0';
        }

        if(strcmp(buffer, "HUM?") == 0)
        {
        	ReadMoistureSensor();
        	i = 0;
        	buffer[0] = '\0';
        }

        if(strcmp(buffer, "LIGHT?") == 0)
        {
        	ReadLDRSensor();
            i = 0;
            buffer[0] = '\0';
        }

        if(strcmp(buffer, "TEMP?") == 0)
        {
        	ReadTemperatureSensor();
            i = 0;
            buffer[0] = '\0';
        }

        if(strncmp(buffer, "LED:", 4) == 0)
        {
            uint16_t pwm_value = atoi(&buffer[4]);
            LED_SetBrightness(pwm_value);
            i = 0;
            buffer[0] = '\0';
        }



    }



}
