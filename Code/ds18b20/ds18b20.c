#include "ds18b20.h"

#define DS18B20_PORT GPIOB
#define DS18B20_PIN  GPIO_PIN_6

static void delay_us(uint16_t us)
{
    uint32_t start = DWT->CYCCNT;
    uint32_t ticks = us * (HAL_RCC_GetHCLKFreq() / 1000000);

    while((DWT->CYCCNT - start) < ticks);
}

static void OneWire_Pin_Output(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = DS18B20_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(DS18B20_PORT, &GPIO_InitStruct);
}

static void OneWire_Pin_Input(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = DS18B20_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;

    HAL_GPIO_Init(DS18B20_PORT, &GPIO_InitStruct);
}

static uint8_t OneWire_Reset(void)
{
    uint8_t presence;

    OneWire_Pin_Output();

    HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, GPIO_PIN_RESET);
    delay_us(480);

    OneWire_Pin_Input();
    delay_us(80);

    presence = HAL_GPIO_ReadPin(DS18B20_PORT, DS18B20_PIN);

    delay_us(400);

    return !presence;
}

static void OneWire_WriteBit(uint8_t bit)
{
    OneWire_Pin_Output();

    HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, GPIO_PIN_RESET);

    if(bit)
    {
        delay_us(1);
        OneWire_Pin_Input();
        delay_us(60);
    }
    else
    {
        delay_us(60);
        OneWire_Pin_Input();
    }
}

static uint8_t OneWire_ReadBit(void)
{
    uint8_t bit = 0;

    OneWire_Pin_Output();

    HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, GPIO_PIN_RESET);
    delay_us(2);

    OneWire_Pin_Input();
    delay_us(8);

    bit = HAL_GPIO_ReadPin(DS18B20_PORT, DS18B20_PIN);

    delay_us(50);

    return bit;
}

static void OneWire_WriteByte(uint8_t data)
{
    for(int i = 0; i < 8; i++)
    {
        OneWire_WriteBit(data & (1 << i));
    }
}

static uint8_t OneWire_ReadByte(void)
{
    uint8_t data = 0;

    for(int i = 0; i < 8; i++)
    {
        if(OneWire_ReadBit())
        {
            data |= (1 << i);
        }
    }

    return data;
}

float DS18B20_ReadTemp(void)
{
    uint8_t temp_lsb, temp_msb;
    int16_t temp;

    if(!OneWire_Reset())
    {
        return -127.0;
    }

    OneWire_WriteByte(0xCC);
    OneWire_WriteByte(0x44);

    HAL_Delay(750);

    OneWire_Reset();

    OneWire_WriteByte(0xCC);
    OneWire_WriteByte(0xBE);

    temp_lsb = OneWire_ReadByte();
    temp_msb = OneWire_ReadByte();

    temp = (temp_msb << 8) | temp_lsb;

    return (float)temp / 16.0;
}
