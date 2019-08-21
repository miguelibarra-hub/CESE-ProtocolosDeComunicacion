/*
 * ds1307.h
 *
 * Created: 31/07/2019 20:46:37
 *  Author: miguel
 */ 


#ifndef DS1307_H_
#define DS1307_H_

#define DS1307_ADDRESS	0x68 

typedef struct {
	uint8_t Hours;
	uint8_t Minutes;
	uint8_t Seconds;
	uint8_t Date;
	uint8_t Month;
	uint8_t Year;
}RTCTime;			//  Mantiene la información del tiempo

/*  DS1307GetTime
*    Rellena la estructura ATime con la info del tiempo
*	Parametros:
*		*RTCTime ATime		puntero a la estructura que se actualizara con los datos del tiempo
*	Devuelve:
*		TIME_OK			Funcion exitosa. 
*		DS1307_FAIL		Funcion falla el chip no responde.
*/
int32_t DS1307GetTime(RTCTime *ATime);

/*  DS1307SetTime
*   Inicializa los registros del RTC con la info del tiempo apuntada por la estructura   
*	Parametros:    
*		RTCTime *ATime		puntero a la estructura de tiempo
*	Devuelve:
*		TIME_OK			Funcion exitosa.
*		DS1307_FAIL		Funcion falla el chip no responde.
*/
int32_t DS1307SetTime(RTCTime *ATime);

/*  DS1307Init
*    Inicializa el módulo.
*/
void DS1307Init(void);	


#endif /* DS1307_H_ */