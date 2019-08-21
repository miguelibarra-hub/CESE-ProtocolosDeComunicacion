/*
 * messagestore.h
 *
 * Created: 20/08/2019 0:15:09
 *  Author: miguel
 */ 


#ifndef MESSAGESTORE_H_
#define MESSAGESTORE_H_

#include "asf.h"
/*
 * 	SetMessage:
 *		Guarda un mensaje en EEPROM. Le agrega con los caracteres de inicio y fin y lo almacena en
 *		la primer pagina disponible.
 *	Retorna:
 *		0	Operacion exitosa
 *		-1	Falla
 * */
int32_t SetMessage(uint8_t *AMessage, uint8_t ALenght);
/*
 * 	GetMessage:
 * 		Obtiene un mensaje desde EEPROM. Si encuentra un mensaje lo carga en el puntero *AMessage
 * 		una ves cargado, marca la pagina con 0xff para hacerla disponible.
 * 	Retorna:
 * 		0	Operacion exitosa
 *		-1	Falla
 * */
int32_t GetMessage(uint8_t *AMessage);
/*
 * 	MessageStoreInit:
 * 		Inicializa el modulo
 * */
void MessageStoreInit(void);

#endif /* MESSAGESTORE_H_ */
