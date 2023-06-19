/*
 * esp8266.c
 *
 *  Created on: Jun 18, 2023
 *      Author: user0
 */
#include <esp8266.h>

static UART_HandleTypeDef* uart = NULL;
static uint8_t ServerStatus;
static uint16_t port;
static uint32_t ip;
static uint8_t state[3];
static esp_PinMode mode[3];
static float duty[3];


HAL_StatusTypeDef esp_Init(UART_HandleTypeDef* huart)
{
	uart = huart;
	ServerStatus = 1;
	return HAL_OK;
}

HAL_StatusTypeDef esp_SendCommand(esp_command cmd)
{
	HAL_UART_Transmit_DMA(uart, (uint8_t*)&cmd, sizeof(cmd));
	return HAL_OK;
}



HAL_StatusTypeDef esp_ReciveCommand(esp_command* cmd)
{
	*cmd = (esp_command){ .cmdline = NULL, .cmd = ESP_StartServer }; // TODO
	//HAL_UART_Receive_DMA(uart, cmd, sizeof(cmd));
	return HAL_OK;
}


HAL_StatusTypeDef esp_ProcessCommand(esp_command* cmd)
{
	switch (cmd->cmd)
		{
			case ESP_StartServer:
				break;
			case ESP_StopServer:
				break;
			case ESP_SetPinDuty:
				mode[cmd->port] = ESP_Variable;
				duty[cmd->port] = cmd->duty;
				break;

			case ESP_SetPinMode:
				mode[cmd->port] = cmd->mode;
				break;

			case ESP_SetPinState:
				mode[cmd->port] = ESP_Discrete;
				state[cmd->port] = cmd->mode;
				break;

			case ESP_SetServerIP:
				ip = cmd->ip;
				break;

			case ESP_SetServerPort:
				port = cmd->port;
				break;
			case ESP_GetState:
				{
					esp_command status;
					status = (esp_command) { .cmd = ESP_GetServerIP, .ip = ip};
					esp_SendCommand(status);
					status = (esp_command) { .cmd = ESP_GetServerPort, .port = port};
					esp_SendCommand(status);
					status = (esp_command) { .cmd = ESP_GetServerStatus, .data = 1};
					esp_SendCommand(status);
					for (int i = 0; i < 3; ++i) {
						status = (esp_command) { .cmd = ESP_GetPinMode, .mode = mode[i] };
						esp_SendCommand(status);
						if (mode[i] == ESP_Variable)
							status = (esp_command) { .cmd = ESP_GetPinDuty, .duty = duty[i] };
						else
							status = (esp_command) { .cmd = ESP_GetPinState, .PinState = state[i] };
						esp_SendCommand(status);
					}
				}
				break;
			case ESP_GetServerIP:
				cmd->ip = ip;
				break;
			case ESP_GetServerPort:
				cmd->port = port;
				break;
			case ESP_GetServerStatus:
				cmd->data = 1;
				break;
			case ESP_GetPinMode:
				cmd->mode = mode[cmd->port];
				break;
			case ESP_GetPinDuty:
				cmd->duty = duty[cmd->port];
				break;
			case ESP_GetPinState:
				cmd->PinState = state[cmd->port];
				break;

		}

	return HAL_OK;
}
