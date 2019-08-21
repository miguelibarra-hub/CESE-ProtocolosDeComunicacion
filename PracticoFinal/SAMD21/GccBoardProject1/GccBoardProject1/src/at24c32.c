/*
 * at24c32.c
 *
 * Created: 19/08/2019 0:47:26
 *  Author: miguel
 */ 
#include "asf.h"
#include "inc/at24c32.h"
#include "inc/I2CMasterChannel.h"

#define AT24C32_ADDRESS		0x50

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
int32_t AT24C32WritePage(uint8_t *ABuffer, uint8_t ALenght, uint16_t AAddressPage)
{
	uint8_t i;
	uint8_t BufferWrite[32];
	BufferWrite[0] = AAddressPage >> 8;				//	Prepara la direccion
	BufferWrite[1] = AAddressPage & 0xFF;
	for(i = 2;i < ALenght;i++)						//	Carga el buffer local de escritura
		BufferWrite[i] = *(ABuffer + (i - 2));
	struct i2c_master_packet Packet = {				//	Prepara la estructura usada para la transferencia I2C
		.address =  AT24C32_ADDRESS,				//	Direccion del Dispositivo
		.data_length = 2 + ALenght,					//	Cantidad de datos
		.data = &BufferWrite[0],					//	direccion del buffer de escritura
		.ten_bit_address = false,					//	desabilita direccionamiento de 10 bits
		.high_speed = false,						//	velocidad 100Kkz
		.hs_master_code = 0x0,
	};
	return I2CMasterWrite(&Packet);					//	Envia al master I2C la estructura
}

/*	AT24C32ReadPage:
 * 		Lee una pagina de 32 bytes de la EEPROM.
 * 	Parametros:
 * 		uint8_t *ABuffer		Puntero al Buffer donde se guardaran los datos
 * 		uint16_t AAddressPage	Direccion EEPROM
 * 	Retorna:
 * 		0	Operacion exitosa
 * 		-1	Falla
 * */
int32_t AT24C32ReadPage(uint8_t *ABuffer, uint16_t AAddressPage)
{
	uint8_t i;
	uint8_t BufferWrite[8];
	uint8_t BufferRead[PAGE_SIZE];
	BufferWrite[0] = AAddressPage >> 8;
	BufferWrite[1] = AAddressPage & 0xFF;
	struct i2c_master_packet Packet = {			//	Primero hacemos que vamos a escribir para actualizar el registro de direccion
		.address =  AT24C32_ADDRESS,
		.data_length = 2,
		.data = &BufferWrite[0],
		.ten_bit_address = false,
		.high_speed = false,
		.hs_master_code = 0x0,
	};
	if(I2CMasterWrite(&Packet) != 0)			//	Setea el registro de direccion
		return -1;
	Packet.data = ABuffer;
	Packet.data_length = PAGE_SIZE;
	if(I2CMasterRead(&Packet) != 0)				//	Lectura de la pagina
		return -1;
	return 0;	
}


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
int32_t AT24C32ReadByte(uint8_t *ABuffer, uint16_t AAddress)
{
	uint8_t i;
	uint8_t BufferWrite[8];
	uint8_t BufferRead[8];
	BufferWrite[0] = AAddress >> 8;
	BufferWrite[1] = AAddress & 0xFF;
	struct i2c_master_packet Packet = {		//	Primero hacemos que vamos a escribir para actualizar el registro de direccion
		.address =  AT24C32_ADDRESS,
		.data_length = 2,
		.data = &BufferWrite[0],
		.ten_bit_address = false,
		.high_speed = false,
		.hs_master_code = 0x0,
	};
	if(I2CMasterWrite(&Packet) != 0)		//	Setea el registro de direccion
		return -1;
	Packet.data = ABuffer;
	Packet.data_length = 1;
	if(I2CMasterRead(&Packet) != 0)			//	Lectura del byte
		return -1;
	return 0;
}

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
int32_t AT24C32WriteByte(uint8_t AData, uint16_t AAddress)
{
	uint8_t i;
	uint8_t BufferWrite[8];
	uint8_t BufferRead[8];
	BufferWrite[0] = AAddress >> 8;
	BufferWrite[1] = AAddress & 0xFF;
	BufferWrite[2] = AData;
	struct i2c_master_packet Packet = {
		.address =  AT24C32_ADDRESS,
		.data_length = 3,
		.data = &BufferWrite[0],
		.ten_bit_address = false,
		.high_speed = false,
		.hs_master_code = 0x0,
	};
	if(I2CMasterWrite(&Packet) != 0)
		return -1;
	return 0;
}
