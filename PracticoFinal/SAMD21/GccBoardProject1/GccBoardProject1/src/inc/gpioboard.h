/*
 * gpioboard.h
 *
 * Created: 14/06/2019 21:41:01
 *  Author: miguel
 */ 


#ifndef GPIOBOARD_H_
#define GPIOBOARD_H_

#include "asf.h"

typedef enum {INPUT0,INPUT1,INPUT2,INPUT3}Input_t;	//	Tipo de dato usado para representar las entradas

/*
*	GpioGetPinState :
*		Devuelve el estado de la entrada AGpioPin, este valor internamente es mapeado
*	al nombre del pin.
*	Parametros
*	Input_t AGpioPin	Tipo generico asignado para describir las entradas
*/	
bool GpioGetPinState(Input_t AGpioPin);			

/*
*	GpioConfigure:
*		Inicializa los pines usados en la placa.
*/
void GpioConfigure(void);					



#endif /* GPIOBOARD_H_ */