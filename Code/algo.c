#include "algo.h"
#include "stm32l4xx.h"
#include <stdio.h>
#include <string.h>
#include "ds18b20.h"


extern ADC_HandleTypeDef hadc1;
extern UART_HandleTypeDef huart1;


void PUMP_ON(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
}

void PUMP_OFF(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
}
void FAN_ON(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
}

void FAN_OFF(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
}


static uint32_t ReadADC_Channel(uint32_t channel)
{
    ADC_ChannelConfTypeDef sConfig = {0};

    sConfig.Channel = channel;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_47CYCLES_5;
    sConfig.SingleDiff = ADC_SINGLE_ENDED;
    sConfig.OffsetNumber = ADC_OFFSET_NONE;
    sConfig.Offset = 0;

    HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 100);

    uint32_t value = HAL_ADC_GetValue(&hadc1);

    HAL_ADC_Stop(&hadc1);

    return value;
}


void ReadMoistureSensor(void)
{
    uint32_t humidity_value;
    char msg[100];

    humidity_value = ReadADC_Channel(ADC_CHANNEL_6); // PA1 = HUM_ADC

    if(humidity_value > 2800)
    {
        sprintf(msg, "HUM = %lu -> Terre seche, arrosage conseille\r\n", humidity_value);
    }
    else if(humidity_value > 2500)
    {
        sprintf(msg, "HUM = %lu -> Humidite moyenne\r\n", humidity_value);
    }
    else
    {
        sprintf(msg, "HUM = %lu -> Terre humide\r\n", humidity_value);
    }

    HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 100);
}



void ReadLDRSensor(void)
{
    uint32_t ldr_value;
    char msg[100];

    ldr_value = ReadADC_Channel(ADC_CHANNEL_5); // PA0 = LDR_ADC

    if(ldr_value > 3000)
    {
        sprintf(msg, "LDR = %lu -> Forte luminosite\r\n", ldr_value);
    }
    else if(ldr_value > 1500)
    {
        sprintf(msg, "LDR = %lu -> Luminosite moyenne\r\n", ldr_value);
    }
    else
    {
        sprintf(msg, "LDR = %lu -> Faible luminosite\r\n", ldr_value);
    }

    HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 100);
}

void ReadTemperatureSensor(void)
{
    float temperature;
    char msg[100];

    temperature = DS18B20_ReadTemp();

    sprintf(msg,
            "TEMP = %.2f C\r\n",
			temperature);

    HAL_UART_Transmit(&huart1,
                     (uint8_t*)msg,
                     strlen(msg),
                     100);
}


void LED_PWM_Start(void)
{
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
}

void LED_SetBrightness(uint16_t value)
{
    char msg[50];

    if(value > 999)
        value = 999;

    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, value);

    sprintf(msg, "LED PWM = %u\r\n", value);
    HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 100);
}
