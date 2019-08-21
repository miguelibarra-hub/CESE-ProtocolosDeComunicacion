/*
 * at24c32.h
 *
 * Created: 19/08/2019 0:46:58
 *  Author: miguel
 */ 


#ifndef AT24C32_H_
#define AT24C32_H_

#include "asf.h"

#define PAGE_SIZE			32											//	Tamaño de pagina para esta memoria
#define MAX_PAGES			128											//	Cantidad de paginas
#define MAX_ADDRESS			((MAX_PAGES * PAGE_SIZE) - PAGE_SIZE) 		//	Direccion mas alta posible que se puede escribir una pagina

/*	AT24C32WritePage:
 * 		Escribe una pagina de 32 bytes en la EEPROM.
 * 	Parametros:
 * 		uint8_t *ABuffer		Puntero al Buffer de datos
 * 		uint8_t ALenght			Cantidad de datos a escribir
 * 		uint16_t AAddressPage	Direccion EEPROM
 * 	Retorna:
 * 		0	Operacion exitosa
 * 		-1	Falla
 * */
int32_t AT24C32WritePage(uint8_t *ABuffer, uint8_t ALenght, uint16_t AAddressPage);
/*	AT24C32ReadPage:
 * 		Lee una pagina de 32 bytes de la EEPROM.
 * 	Parametros:
 * 		uint8_t *ABuffer		Puntero al Buffer donde se guardaran los datos
 * 		uint16_t AAddressPage	Direccion EEPROM
 * 	Retorna:
 * 		0	Operacion exitosa
 * 		-1	Falla
 * */
int32_t AT24C32ReadPage(uint8_t *ABuffer, uint16_t AAddressPage);
/*
 * 	AT24C32ReadByte:
 * 		Lee un byte desde EEPROM.
 * 	Parametros:
 * 		uint8_t *ABuffer	Puntero a donde se guarda el dato
 * 		uint16_t AAddress	Direccion EEPROM
 * 	Retorna:
 * 		0	operacion exitosa
 * 		-1	falla
 * */
int32_t AT24C32ReadByte(uint8_t *ABuffer, uint16_t AAddress);
/*
 * 	AT24C32WriteByte:
 * 		Escribe en EEPROM un byte.
 * 	Parametros:
 * 		uint8_t AData		Dato a escribir
 * 		uint16_t AAddress	Direccion
 * 	Retorna:
 * 		0	Operacion exitosa
 * 		-1	Falla
 * */
int32_t AT24C32WriteByte(uint8_t AData, uint16_t AAddress);

#endif /* AT24C32_H_ */
