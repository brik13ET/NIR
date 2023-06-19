/*
 * esp8266.h
 *
 *  Created on: Jun 18, 2023
 *      Author: user0
 */

#ifndef ESP_AT_DRIVER_ESP8266_H_
#define ESP_AT_DRIVER_ESP8266_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"

typedef enum {
	ESP_StartServer,
	ESP_StopServer,
	ESP_GetServerStatus,

	ESP_SetServerPort, // 80 - 65535
	ESP_GetServerPort,

	ESP_SetServerIP, // local net possible ip
	ESP_GetServerIP,

	ESP_SetPinMode, // Discrete | Analog
	ESP_GetPinMode,

	ESP_SetPinState, // ON | OFF
	ESP_GetPinState,

	ESP_SetPinDuty, // 0 - 100 %
	ESP_GetPinDuty,

	ESP_GetState
} esp_cmd_type;

typedef enum {
	ESP_Discrete,
	ESP_Variable
} esp_PinMode;

typedef struct {
	char* cmdline;
	esp_cmd_type cmd;
	union {
		uint16_t port;
		uint32_t ip;

		esp_PinMode mode;
		uint8_t PinState;
		float duty;
		uint32_t data;
	};
} esp_command;

HAL_StatusTypeDef esp_Init(UART_HandleTypeDef* uart);

HAL_StatusTypeDef esp_SendCommand(esp_command cmd);

HAL_StatusTypeDef esp_ReciveCommand(esp_command* cmd);

HAL_StatusTypeDef esp_ProcessCommand(esp_command* cmd);

#endif /* ESP_AT_DRIVER_ESP8266_H_ */
