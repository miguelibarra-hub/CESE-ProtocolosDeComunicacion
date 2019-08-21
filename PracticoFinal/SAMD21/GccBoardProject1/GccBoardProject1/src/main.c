/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * This is a bare minimum user application template.
 *
 * For documentation of the board, go \ref group_common_boards "here" for a link
 * to the board-specific documentation.
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to system_init()
 * -# Basic usage of on-board LED and button
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */
#include <asf.h>
#include "inc/schedulertimer.h"
#include "inc/gpioboard.h"
#include "inc/inputsmonitor.h"
#include "inc/rtccalendar.h"
#include "inc/usartmodule.h"
#include "inc/ds1307.h"
#include "inc/esp32interface.h"
#include "inc/spiflash.h"
#include "inc/I2CMasterChannel.h"
#include "inc/at24c32.h"
#include "inc/messagestore.h"


RTCTime TimeDS;
uint32_t FMessagePending;

/*
 * 	CheckForPendingMessages:
 *		Funcion que verifica cada 5 segundos si hay mensajes pendientes y los transmite
 * */
void CheckForPendingMessages(void)
{
	uint8_t Buffer[PAGE_SIZE];
	if(FMessagePending != 0){									//	Verifica si hay mensajes pendientes
		if(!GetFlagEsp32Busy()){								//	Verifica si el canal de comunicacios esta libre
			GetMessage(&Buffer[0]);
			if(SendDataToEsp32(&Buffer[0],(PAGE_SIZE-2)) == 0){	//	Envia el mensaje
				FMessagePending--;
			}
		}
	}
}

/*
*	SetTimeToString:
*		Convierte los datos del tiempo a una cadena la escribe en el puntero *ATimeString
*	Devuelve
*		La cantidad de caracteres escritos
*/
uint8_t SetTimeToString(char *ATimeString, RTCTime *ATime)
{
	uint8_t CharCount = 0;
	CharCount = sprintf(ATimeString,"%d",ATime->Date);
	CharCount = CharCount + sprintf((ATimeString + CharCount),"/");
	CharCount = CharCount + sprintf((ATimeString + CharCount),"%d",ATime->Month);
	CharCount = CharCount + sprintf((ATimeString + CharCount),"/");
	CharCount = CharCount + sprintf((ATimeString + CharCount),"%d",ATime->Year);
	CharCount = CharCount + sprintf((ATimeString + CharCount),":");
	CharCount = CharCount + sprintf((ATimeString + CharCount),"%d",ATime->Hours);
	CharCount = CharCount + sprintf((ATimeString + CharCount),"/");
	CharCount = CharCount + sprintf((ATimeString + CharCount),"%d",ATime->Minutes);
	CharCount = CharCount + sprintf((ATimeString + CharCount),"/");
	CharCount = CharCount + sprintf((ATimeString + CharCount),"%d \n\r",ATime->Seconds);
	return CharCount;
}

/*
*	InputsChangeHandler:
*		Esta funcion es la encargada de procesar los eventos que se generan en el modulo inputsmonitor.c
*	para esta demo se detectan solamente flancos de bajada . Aca se arma un buffer con la info de la entrada
*	y el valor , y se agrega informacion de tiempo. Luego se envia por el puerto serie en formato de texto.
*	Parametros
*	uint8_t AInputNumber	Numero de entrada
*	bool AInputValue		Valor
*/
void InputsChangeHandler(uint8_t AInputNumber,bool AInputValue)
{
	uint8_t BufferInputs[40];
	uint8_t BufferPage[40];
	uint8_t BytesCount = 0;
	port_pin_toggle_output_level(LED0_PIN);
	DS1307GetTime(&TimeDS);	
	if(AInputValue == 0){																	//	Verifica que sea un flanco de bajada
		BytesCount = sprintf(&BufferInputs[0],"Input %d = %d ",AInputNumber,AInputValue);	//	Imprime en el buffer la Entrada y el valor
		BytesCount = BytesCount + SetTimeToString(&BufferInputs[BytesCount],&TimeDS);		//	Agrega la info del tiempo
		if(BytesCount < MAX_TX_BUFFER_LENGTH){												//	Verifica que entre en el buffer de transmision
			//SendBuffer(&BufferInputs[0],BytesCount);										//	Envia por serial.
			if(SendDataToEsp32(&BufferInputs[0],BytesCount) != 0){
				SetMessage(&BufferInputs[0],BytesCount-2);
				 FMessagePending++;
			}
		}
	}
}

int main (void)
{
	system_init();

	/* Insert application code here, after the board has been initialized. */
	GpioConfigure();													
	InputsMonitorInit();
	InputsMonitorEnable(true);
	RTCCalendarInit();
	I2CMasterInit();
	DS1307Init();
	MessageStoreInit();
	UsartInit(115200, USART_PARITY_NONE, USART_STOPBITS_2, USART_CHARACTER_SIZE_8BIT);	
	RegCallbackOnInputChange(&InputsChangeHandler);			//	registra la funcion que atiende el evento de cambio en alguna entrada	
	ESP32InterfaceInit();
	SchedulerTimerInit();
	SchedulerAddTask(&InputsMonitorUpdate,0,SAMPLER_PERIOD);
	SchedulerAddTask(&CheckForPendingMessages, 10, 5000);
	SchedulerTimerStart();
	/* This skeleton code simply sets the LED to the state of the button. */
	system_interrupt_enable_global();
	port_pin_toggle_output_level(LED0_PIN);
	FMessagePending = 0;
	while (1) {
		SchedulerDispatchTasks();
		CheckRx();
	}
}
