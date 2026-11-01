/**
 ******************************************************************************
 * @file    buttons.h
 * @brief   Header for Buttons.c
 *
 * Copyright 2026 Jan Kolář
 * SPDX-License-Identifier: Apache-2.0
 ******************************************************************************
 **/
#ifndef INC_BUTTONS_H_
#define INC_BUTTONS_H_

#include <stdint.h>
#include <string.h>
#include "main.h"


#define BTTNS_COUNT 9
#define BTTNS_STATE_SIZE ((BTTNS_COUNT + 7) / 8)

typedef struct {
	GPIO_TypeDef* port;
	uint16_t pin;
} Button_t;


/*
 * @brief Initialize the button reading
 * @param None
 * @retval None
 */
void bttnsInit(void);

/*
 * @brief read the Last button state
 * @param return value pointer
 * @retval NULL on bad readSnapshot pointer, otherwise readSnapshot pointer
 */
uint8_t* bttnsRead(uint8_t readSnapshot[BTTNS_STATE_SIZE]);

/*
 * @brief Read the current buttons pressed and update the internal bttnState
 * @param None
 * @retval None
 */
uint8_t bttnsUpdate(void);

#endif /* INC_BUTTONS_H_ */
