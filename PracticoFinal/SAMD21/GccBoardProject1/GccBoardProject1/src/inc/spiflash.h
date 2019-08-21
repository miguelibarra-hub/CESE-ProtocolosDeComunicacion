/*
 * spiflash.h
 *
 * Created: 31/07/2019 21:38:39
 *  Author: miguel
 */ 


#ifndef SPIFLASH_H_
#define SPIFLASH_H_

/* SPIFlashInit
*  Inicializa el módulo
*/
void SPIFlashInit(void);

/* SPIFlashWriteMessage
*  Guarda un mensaje en memoria serial.
*  Parámetros
*		AMessage 			puntero a los datos
*		AMessageLenght  	longitud de los datos
*  Devuelve:
*		0				OK
*		-1				E_QUEUE_FULL
*/
int32_t SPIFlashWriteMessage(uint8_t *AMessage, uint8_t AMessageLenght );

/* SPIFlashGetMessage
*  Obtiene un mensaje de la cola.
*  Parámetros:
*		AMessage 		puntero al buffer donde se guardará el mensaje
*		ADataRead		puntero donde se guarda la cantidad de datos leidos
*  Devuelve:
*		0				OK
*		-1				E_QUEUE_EMPTY
*/
int32_t SPIFlashGetMessage(uint8_t *AMessage, uint8_t *ADataRead);





#endif /* SPIFLASH_H_ */