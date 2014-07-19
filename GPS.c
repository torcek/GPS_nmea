/*
 * GPS.c
 *
 *  Created on: Jul 18, 2014
 *      Author: Grzegorz Wojcik
 */

#include <stddef.h>
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "GPS.h"

								/*** Initialization functions ***/
void GPS_GPIOinit(void){
	/* STM32F407VGT6
	 * USART1:
	 * TX: PB6
	 * RX: PB7 */

	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE );	//Enabling peripherial clock for PORTB

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* Connecting the RX and TX pins to their AlternateFunctions */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);
}

/* GPS_USARTinit function should be executed after GPS_GPIOinit function */
void GPS_USARTinit(void){
	/* STM32F407VGT6
	 * USART1:
	 * TX: PB6
	 * RX: PB7 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); 	// Enabling peripherial clock for USART1
															// Note that only USART1 and USART6 are connected
															// to APB2
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = 9600;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; 	// we want to enable the transmitter and the receiver
	USART_Init(USART1, &USART_InitStruct);

	/* Here the USART1 receive interrupt is enabled
	 * and the interrupt controller is configured
	 * to jump to the USART1_IRQHandler() function
	 * if the USART1 receive interrupt occurs */

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); 			// enable the USART1 receive interrupt

	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;		 // we want to configure the USART1 interrupts
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;// this sets the priority group of the USART1 interrupts
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;		 // this sets the subpriority inside the group
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;			 // the USART1 interrupts are globally enabled
	NVIC_Init(&NVIC_InitStruct);							 // the properties are passed to the NVIC_Init function which takes care of the low level stuff

	USART_Cmd(USART1, ENABLE);	// Enabling the complete USART1 peripherial
}

void GPS_VariablesInit(void){
	GPS_flag = 0;
	GPS_ClearDataFrame(GPS_DataFrame, 100);

	GPS GPS_AAT;	/* AAT stands for Automatic Antenna Tracker */
	GPS_AAT.Altitude = 0;
	GPS_AAT.Latitude = 0;
	GPS_AAT.Longitude = 0;
	GPS_AAT.Speed = 0;
	GPS_AAT.Time_hours = 0;
	GPS_AAT.Time_minutes = 0;
	GPS_AAT.Time_seconds = 0;
}


								/*** Functions ***/
void GPS_ClearDataFrame(unsigned char *string, uint8_t length){
	static uint8_t i = 0;
	for( i = 0; i < length; i++){
		string[i] = 0;
	}
}


void GPS_ParseGGA(){
	static uint8_t i = 0;
	static unsigned char tmp[80] = {0};
	static float result = 0;

	if( GPS_flag == 1 ){	/* DataFrame is ready to parse */
		if( (GPS_DataFrame[3] == 'G') && (GPS_DataFrame[4] == 'G') && (GPS_DataFrame[5] == 'A') ){

			for( i = 0; i < 80; i++ ){
				static uint8_t counter = 0;
				if( GPS_DataFrame[i] == ',' )
					counter++;
				if( counter >= 2 && counter <=3)

			}






		}
	else
		GPS_flag = 0;		/* Allow to receive next DataFrame */

	}
}






				/*** Interrupt Request Handler (IRQ) for ALL USART1 interrupts ***/
void USART1_IRQHandler(void){

	if( USART_GetITStatus(USART1, USART_IT_RXNE) ){

		static uint8_t cnt = 0; 	// String length
		char t = USART1->DR; 		// Received character from USART1 data register is saved in t

		if( (GPS_flag == 0) ){
			if( t == '$' ){
				cnt = 0;
				GPS_DataFrame[cnt] = t;
				cnt++;
			}
			if( t != '$' ){
				GPS_DataFrame[cnt] = t;
				cnt++;
				if( t == 0x0D ){
					//cnt = 0;
					GPS_flag = 1;
				}
			}
		}
	}
}
