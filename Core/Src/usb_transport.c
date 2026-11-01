/**
 ******************************************************************************
 * @file    usb_transport.c
 * @brief   USB transport abstraction layer
 *
 * Copyright 2026 Jan Kolář
 * SPDX-License-Identifier: Apache-2.0
 ******************************************************************************
 **/

#include "usb_transport.h"


/* Typedefs */
/* TX queue definition*/
typedef struct {
	uint8_t Buffer[USB_QUEUE_SIZE][USB_QUEUE_ELEM_SIZE];
	uint8_t head;
	uint8_t tail;
	uint8_t count;
} UsbTxQueue;

/* Private Variables */
static UsbTxQueue txQueue = {0};
static uint8_t txBuffer[USB_QUEUE_ELEM_SIZE];


/* Static Functions Declarations */

/* Tx Queue function declarations */

/*
 * @brief Check if the TX queue is empty
 * @param true if the queue is empty, otherwise false
 * @retval None
 */
static inline uint8_t usbTxQueueIsEmpty(void);

/*
 * @brief Check if the TX queue is full
 * @param true if the queue is full, otherwise false
 * @retval None
 */
static inline uint8_t usbTxQueueIsFull(void);

/*
 * @brief Fetch the next element to be popped without removing it from queue
 * @param pointer to a buffer for the result data
 * @retval false if the queue is empty
 */
static uint8_t usbTxQueuePeek(uint8_t* retBuffer);

/*
 * @brief Pop the next element from the queue
 * @param pointer to a buffer for the result data
 * @retval false if the queue is empty
 */
static uint8_t usbTxQueuePop(uint8_t* retBuffer);

/*
 * @brief Add an element to the queue
 * @param pointer to a buffer with data
 * @retval false if the queue is full
 */
static uint8_t usbTxQueuePush(uint8_t* element);




static inline uint8_t usbTxQueueIsEmpty(void){
	return (txQueue.count == 0U);
}

static inline uint8_t usbTxQueueIsFull(void){
	return (txQueue.count == USB_QUEUE_SIZE);
}

static uint8_t usbTxQueuePeek(uint8_t* retBuffer){
	if(usbTxQueueIsEmpty()){
		return 0U;
	}

	memcpy(retBuffer, txQueue.Buffer[txQueue.tail], USB_QUEUE_ELEM_SIZE);

	return 1U;
}

static uint8_t usbTxQueuePop(uint8_t* retBuffer){
	if(usbTxQueueIsEmpty()){
		return 0U;
	}

	memcpy(retBuffer, txQueue.Buffer[txQueue.tail], USB_QUEUE_ELEM_SIZE);
	txQueue.tail = (txQueue.tail + 1U) % USB_QUEUE_SIZE;
	txQueue.count--;

	return 1U;
}


static uint8_t usbTxQueuePush(uint8_t* element){
	if(usbTxQueueIsFull()){
		return 0U;
	}

	memcpy(txQueue.Buffer[txQueue.head], element, USB_QUEUE_ELEM_SIZE);
	txQueue.head = (txQueue.head + 1U) % USB_QUEUE_SIZE;
	txQueue.count++;

	return 1U;
}

void USB_update(void){
	/* Check if there is something to transmit */
	if(usbTxQueueIsEmpty()) return;

	/* Check if the USB device is ready to transmit*/
	if (hUsbDeviceFS.dev_state != USBD_STATE_CONFIGURED) return;

	/* Check Custom HID state*/
	USBD_CUSTOM_HID_HandleTypeDef *hid =
	    (USBD_CUSTOM_HID_HandleTypeDef *)hUsbDeviceFS.pClassData;

	if (hid == NULL || hid->state != CUSTOM_HID_IDLE) return;

	/* Get the next report to transmit without removing it*/
	usbTxQueuePeek(txBuffer);

	/* Attempt to send the report*/
	if (USBD_CUSTOM_HID_SendReport(
	        &hUsbDeviceFS,
	        txBuffer,
	        USB_QUEUE_ELEM_SIZE) == USBD_OK)
	{
	    /*On successful report */
	    usbTxQueuePop(txBuffer);
	}
}

uint8_t USB_Enqueue_Report(uint8_t* element){
	if(usbTxQueueIsFull()) return 0;

	usbTxQueuePush(element);

	return 1;

}
