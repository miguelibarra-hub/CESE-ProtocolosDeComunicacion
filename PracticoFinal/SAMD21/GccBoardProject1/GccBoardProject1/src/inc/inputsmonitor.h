/*
 * inputmonitor.h
 *
 * Created: 14/06/2019 23:08:10
 *  Author: miguel
 */ 


#ifndef INPUTSMONITOR_H_
#define INPUTSMONITOR_H_

#define MAX_DELAY_DEBOUNCE	40		//	retardo en ms para esperar hasta que el pin este estable, debe ser multiplo del periodo de muestreo.
#define SAMPLER_PERIOD		10		//	periodo en ms de muestreo del pin

#include "asf.h"

typedef void (*TOnInputChange)(uint8_t AInputNumber,bool AInputValue);				//	tipo definido para la funcion de callback 

/*
*	RegCallbackOnInputChange:
*		Registra la funcion que se llamara cuando se detecte un cambio en alguna entrada.
*	Parametros
*	TOnInputChange AOnInputChange		funcion callback
*/
void RegCallbackOnInputChange(TOnInputChange AOnInputChange);						 
/*
*	InputsMonitorInit:
*		Inicializa el modulo.
*/
void InputsMonitorInit(void);														
/*
*	InputsMonitorEnable:
*		Habilita/Deshabilita el modulo en lo referente al monitoreo de las entradas.
*/
void InputsMonitorEnable(bool AEnable);												
/*
*	InputsMonitorUpdate:
*		Rutina usada para registrarse en el scheduler , con un periodo de 10ms.
*	De aca se llama a la maquina de estados si esta habilitado el modulo.
*/
void InputsMonitorUpdate(void);														
	
#endif /* INPUTSMONITOR_H_ */