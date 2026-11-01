/**
 ******************************************************************************
 * @file    usb_transport.h
 * @brief   Header for usb_transport.c
 *
 * Copyright 2026 Jan Kolář
 * SPDX-License-Identifier: Apache-2.0
 ******************************************************************************
 **/
#ifndef INC_USB_TRANSPORT_H_
#define INC_USB_TRANSPORT_H_

#include <stdint.h>
#include <string.h>
#include "usb_device.h"
#include "usbd_customhid.h"

#define USB_QUEUE_SIZE 32U
#define USB_QUEUE_ELEM_SIZE 2U

extern USBD_HandleTypeDef hUsbDeviceFS;

/*
 * @brief Mainloop call for updating the USB scheduler
 * @param None
 * @retval None
 */
void USB_update(void);

/*
 * @brief Mainloop call for adding report to the Tx queue
 * @param None
 * @retval None
 */
uint8_t USB_Enqueue_Report(uint8_t* element);

#endif /* INC_USB_TRANSPORT_H_ */
