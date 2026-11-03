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

#define BTTNS_DEBOUNCE_MS 10U

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

static uint8_t bttnCandidate[BTTNS_STATE_SIZE];

static uint32_t bttnChangeTime[BTTNS_COUNT];

void bttnsInit(void)
{
    memset(bttnState, 0, BTTNS_STATE_SIZE);
    memset(bttnCandidate, 0, BTTNS_STATE_SIZE);

    uint32_t now = HAL_GetTick();

    for (uint8_t i = 0; i < BTTNS_COUNT; i++)
    {
        uint8_t byte = i / 8U;
        uint8_t mask = (uint8_t)(1U << (i % 8U));

        bool pressed = (HAL_GPIO_ReadPin(bttns[i].port,
                                         bttns[i].pin) == GPIO_PIN_RESET);

        if (pressed)
        {
            bttnState[byte] |= mask;
            bttnCandidate[byte] |= mask;
        }

        bttnChangeTime[i] = now;
    }
}


uint8_t* bttnsRead(uint8_t readSnapshot[BTTNS_STATE_SIZE]){
	if(NULL == readSnapshot) return NULL;
	return memcpy(readSnapshot, bttnState, BTTNS_STATE_SIZE);
}

bool bttnsUpdate(void)
{
    bool changed = false;
    uint32_t now = HAL_GetTick();

    for (uint8_t i = 0; i < BTTNS_COUNT; i++)
    {
        uint8_t byte = i / 8U;
        uint8_t mask = (uint8_t)(1U << (i % 8U));

        // Read raw GPIO state (active low)
        bool raw = (HAL_GPIO_ReadPin(bttns[i].port,
                                    bttns[i].pin) == GPIO_PIN_RESET);

        // Get candidate and stable states
        bool candidate = (bttnCandidate[byte] & mask) != 0U;
        bool stable = (bttnState[byte] & mask) != 0U;

        // Raw state changed - restart debounce timer
        if (raw != candidate)
        {
            if (raw)
                bttnCandidate[byte] |= mask;
            else
                bttnCandidate[byte] &= (uint8_t)~mask;

            bttnChangeTime[i] = now;
        }

        // Accept change only after debounce interval
        if (raw != stable &&
            (uint32_t)(now - bttnChangeTime[i]) >= BTTNS_DEBOUNCE_MS)
        {
            if (raw)
                bttnState[byte] |= mask;
            else
                bttnState[byte] &= (uint8_t)~mask;

            changed = true;
        }
    }

    return changed;
}
