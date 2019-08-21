/*
 * rtccalendar.h
 *
 * Created: 15/06/2019 22:01:53
 *  Author: miguel
 */ 


#ifndef RTCCALENDAR_H_
#define RTCCALENDAR_H_

#include "asf.h"

/*
*	RTCCalendarInit:
*		Inicializa el modulo
*/
void RTCCalendarInit(void);	

/*
*	GetTime:
*		Obtiene la fecha y hora actuales, y las escribe en el puntero *ATimeString
*   Parametros
*		char *ATimeString		Puntero a un buffer donde se guarda la informacion de tiempo
*	Devuelve 
*		La cantidad de caracteres escritos
*/		
uint8_t GetTime(char *ATimeString);	



#endif /* RTCCALENDAR_H_ */