/**
 ******************************************************************************
 * @file    buttons.c
 * @brief   Handling the mechanic switch inputs
 *
 * Copyright 2026 Jan Kolář
 * SPDX-License-Identifier: Apache-2.0
 ******************************************************************************
 **/

/* Includes */
#include "buttons.h"


/* Globals definitions */
static const Button_t bttns[BTTNS_COUNT] = {
    {KEY_0_GPIO_Port, KEY_0_Pin},
	{KEY_1_GPIO_Port, KEY_1_Pin},
	{KEY_2_GPIO_Port, KEY_2_Pin},
	{KEY_3_GPIO_Port, KEY_3_Pin},
	{KEY_4_GPIO_Port, KEY_4_Pin},
	{KEY_5_GPIO_Port, KEY_5_Pin},
	{KEY_6_GPIO_Port, KEY_6_Pin},
	{KEY_7_GPIO_Port, KEY_7_Pin},
	{KEY_8_GPIO_Port, KEY_8_Pin}
};

static uint8_t bttnState[BTTNS_STATE_SIZE];




void bttnsInit(void){
	for(uint8_t i = 0; i < BTTNS_STATE_SIZE; i++) bttnState[i] = 0U;
}


uint8_t* bttnsRead(uint8_t readSnapshot[BTTNS_STATE_SIZE]){
	if(NULL == readSnapshot) return NULL;
	return memcpy(readSnapshot, bttnState, BTTNS_STATE_SIZE);
}

uint8_t bttnsUpdate(void){
	uint8_t lastBttnState[BTTNS_STATE_SIZE];
	memcpy(lastBttnState, bttnState, BTTNS_STATE_SIZE);

	for(uint8_t i = 0; i < BTTNS_COUNT; i++){
		uint8_t byte = i / 8U;
		uint8_t bit = i % 8U;

		uint8_t mask = (uint8_t)(1U << bit);

		if(HAL_GPIO_ReadPin(bttns[i].port, bttns[i].pin) == GPIO_PIN_RESET){
			bttnState[byte] |= mask;
		}
		else{
			bttnState[byte] &= (uint8_t)~mask;
		}
	}
	for(uint8_t i = 0; i < BTTNS_STATE_SIZE; i++){
		if(bttnState[i] != lastBttnState[i]) return 1U;
	}
	return 0;
}
