/*
 * usart.h
 *
 * Created: 16/06/2019 20:40:09
 *  Author: miguel
 */ 


#ifndef USARTMODULE_H_
#define USARTMODULE_H_

#include "asf.h"

#define MAX_RX_BUFFER_LENGTH 1
#define MAX_TX_BUFFER_LENGTH 40
/*
*	UsartInit:
*		Inicializa y habilita el puerto serie con los parametros de configuracion.
*
*/
void UsartInit(uint32_t ABaudRate, uint32_t AParity, uint32_t AStopBits, uint8_t ADataSize);

/*
*	SendBuffer:
*		Envia un buffer de datos por el puerto serie.
*	Parametros
*		uint8_t *ABuffer		Puntero a los datos a transmitir
*		uint32_t ALenght		Longitud de los datos
*	Devuelve
*		0		Funcion exitosa
*		-1		Falla	 
*/
int32_t SendBuffer(uint8_t *ABuffer, uint32_t ALenght);											

typedef void (*TOnDataReceived)(uint8_t AData);		//	tipo de la funcion callback
/*
 * 	RegOnDataReceived:
 * 		Funcion que registra el callback que se va a llamar cuando llegue un byte al puerto serie
 * */
void RegOnDataReceived(TOnDataReceived AOnDataReceived);
/*
 * 	CheckRx:
 * 		Esta funcion debe ser llamada asincronamente desde el main para darle funcionalidad al driver
 * 		de recepcion por callback esto es propio de la biblioteca de Atmel
 * */
void CheckRx(void);

#endif /* USART_H_ */
