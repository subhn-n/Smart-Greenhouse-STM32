/*
 * algo.h
 *
 *  Created on: May 15, 2026
 *      Author: svbhn_n
 */

#ifndef INC_ALGO_H_
#define INC_ALGO_H_

void PUMP_ON(void);
void PUMP_OFF(void);
void FAN_ON(void);
void FAN_OFF(void);
void ReadMoistureSensor(void);
void ReadLDRSensor(void);
void ReadTemperatureSensor(void);
void LED_PWM_Start(void);
void LED_SetBrightness(uint16_t value);

#endif /* INC_ALGO_H_ */
