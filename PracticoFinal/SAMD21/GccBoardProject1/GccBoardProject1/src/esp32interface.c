/*
 * esp32interface.c
 *
 * Created: 16/08/2019 6:56:36
 *  Author: miguel
 */ 

#include "inc/esp32interface.h"
#include "inc/usartmodule.h"

#define MAX_ESP32_BUFFER	40

#define SCAN_COMMAND	0xE0
#define BUSY_COMMAND	0xE1

static uint32_t FEsp32BusyFlag;
static uint32_t FEsp32DataReadyFlag;
static uint8_t FData;
static uint8_t FBufferEsp32[MAX_ESP32_BUFFER];
static uint32_t FBytesCount;

static void ProcessDataFromEsp32(uint8_t AData);

/*
*	GetFlagEsp32Busy:
*		Devuelve el estado del canal de comunicacion con el ESP32
*	Retorna:
*		0		Libre
*		-1		Ocupado
*/
uint32_t GetFlagEsp32Busy(void)
{
	return	FEsp32BusyFlag;
}

/*
*	ProcessDataFromEsp32:
*		Esta funcion se registra en el modulo usartmodule para recibir los bytes que 
*		vienen del puerto serie de interface con el ESP32.
*	Parametros:
*		uint8_t AData			Dato del serial 
*/
static void ProcessDataFromEsp32(uint8_t AData)
{
	if(AData == SCAN_COMMAND){								//	Si es el comando de exploracion significa que el canal esta libre		
		FEsp32BusyFlag = 0;
		FData = SCAN_COMMAND;
		if(FEsp32DataReadyFlag){							//	Si hay datos para transmitir podemos hacerlo ahora
			SendBuffer(&FBufferEsp32[0], FBytesCount);
			FEsp32DataReadyFlag = 0;
		}else
			SendBuffer(&FData,1);
	}else if(AData == BUSY_COMMAND){						//	Si es un comando de ocupado el canal no esta disponible
		FEsp32BusyFlag = 1;
		FData = SCAN_COMMAND;
		SendBuffer(&FData,1);
	}
}

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
int32_t SendDataToEsp32(uint8_t *AData, uint32_t ALength)
{
	uint32_t i;
	if((FEsp32DataReadyFlag == 0) && (FEsp32BusyFlag == 0)){		//	Verificamos que el canal de comunicacion este libre y no haya datos previos
		if(ALength < MAX_ESP32_BUFFER){								
			for (i = 0; i< ALength; i++){							//	cargamos el buffer
				FBufferEsp32[i] = *(AData + i);
			}
			FEsp32DataReadyFlag = 1;								//	Avisamos que hay datos para transmitir
			FBytesCount = ALength;
			return 0;
		}else
			return -1;												//	Tamaño de datos incorrecto	
	}else
		return -2;													//	Canal ocupado o existen datos para transmitir
}

/*
*	ESP32InterfaceInit:
*		Inicializa el modulo.
*/
void ESP32InterfaceInit(void)
{
	FEsp32BusyFlag = 0;
	FEsp32DataReadyFlag = 0;
	FBytesCount = 0;
	RegOnDataReceived(&ProcessDataFromEsp32);						//	Nos registramos para recibir los bytes desde el ESP32
}