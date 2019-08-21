/*
 * esp32interface.h
 *
 * Created: 16/08/2019 6:55:52
 *  Author: miguel
 */ 

#include "asf.h"

#ifndef ESP32INTERFACE_H_
#define ESP32INTERFACE_H_

/*
*	ESP32InterfaceInit:
*		Inicializa el modulo.
*/
void ESP32InterfaceInit(void);

/*
*	SendDataToEsp32:
*		Envia por UART un buffer de longitud ALenght
*	Parametros:
*		uint8_t *AData		Puntero a los datos
*		uint32_t ALength	Longitud del buffer
*	Retorna:
*		0		Operacion exitosa
*		-1		El ESP32 esta ocupado procesando un mensaje previo
*/
int32_t SendDataToEsp32(uint8_t *AData, uint32_t ALength);

/*
*	GetFlagEsp32Busy:
*		Devuelve el estado del canal de comunicacion con el ESP32
*	Retorna:
*		0		Libre 
*		-1		Ocupado 
*/
uint32_t GetFlagEsp32Busy(void);

#endif /* ESP32INTERFACE_H_ */