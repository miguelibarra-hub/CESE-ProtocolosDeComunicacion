/*
 * ds1307.c
 *
 * Created: 31/07/2019 20:47:10
 *  Author: miguel
 */ 

#include "asf.h"
#include "inc/ds1307.h"
#include "inc/I2CMasterChannel.h"

static uint8_t BinaryToBCD(uint8_t AData)
{
	uint8_t Temp = AData;
	uint8_t Result = 0;
	uint8_t Decenas = 0;
	if(Temp == 10)
		return 0x10;
	if(Temp < 10)
		return Temp;
	else{
		while(Temp > 10){
			Temp -= 10;
			Decenas++;
		}
		return ((Decenas << 4) & 0xF0) | (Temp & 0x0F);
	}	
	
}

/*  DS1307GetTime
*    Rellena la estructura ATime con la info del tiempo
*	Parametros:
*		*RTCTime ATime		puntero a la estructura que se actualizara con los datos del tiempo
*	Devuelve:
*		TIME_OK			Funcion exitosa.
*		DS1307_FAIL		Funcion falla el chip no responde.
*/
int32_t DS1307GetTime(RTCTime *ATime)
{
	int32_t Result = 0;
	uint8_t BufferWrite[8];
	uint8_t BufferRead[8];
	uint32_t timeout = 0;
	BufferWrite[0] = 0;
	struct i2c_master_packet Packet = {
		.address =  DS1307_ADDRESS,
		.data_length = 1,
		.data = &BufferWrite[0],
		.ten_bit_address = false,
		.high_speed = false,
		.hs_master_code = 0x0,
	};
	Result = I2CMasterWrite(&Packet);
	Packet.data = &BufferRead[0];
	Packet.data_length = 8;
	Result = I2CMasterRead(&Packet);
	if(Result == 0){
		ATime->Date = ((BufferRead[4] >> 4)*10) + (BufferRead[4] & 0x0F);
		ATime->Hours = ((BufferRead[2] & 0x3F) >> 4)*10 + (BufferRead[2] & 0x0F);
		ATime->Minutes = ((BufferRead[1] >> 4)*10) + (BufferRead[1] & 0x0F);
		ATime->Month = ((BufferRead[5] >> 4)*10) + (BufferRead[5] & 0x0F);
		ATime->Seconds = ((BufferRead[0] & 0x7F) >> 4)*10 + (BufferRead[0] & 0x0F);
		ATime->Year = (BufferRead[6] >> 4)*10 + (BufferRead[6] & 0x0F);
	}
	return Result;
}

static void EnableOscillator(void)
{
	uint8_t BufferWrite[8];
	uint8_t BufferRead[8];
	uint32_t timeout = 0;
	BufferWrite[0] = 0;
	BufferWrite[1] = 0;
	struct i2c_master_packet Packet = {
		.address =  DS1307_ADDRESS,
		.data_length = 2,
		.data = &BufferWrite[0],
		.ten_bit_address = false,
		.high_speed = false,
		.hs_master_code = 0x0,
	};
	I2CMasterWrite(&Packet);
}

/*  DS1307SetTime
*   Inicializa los registros del RTC con la info del tiempo apuntada por la estructura
*	Parametros:
*		RTCTime *ATime		puntero a la estructura de tiempo
*	Devuelve:
*		TIME_OK			Funcion exitosa.
*		DS1307_FAIL		Funcion falla el chip no responde.
*/
int32_t DS1307SetTime(RTCTime *ATime)
{
	uint8_t BufferWrite[8];
	uint8_t BufferRead[8];
	uint32_t timeout = 0;
	BufferWrite[0] = 0;
	BufferWrite[1] = BinaryToBCD(ATime->Seconds);
	BufferWrite[2] = BinaryToBCD(ATime->Minutes);
	BufferWrite[3] = BinaryToBCD(ATime->Hours);
	BufferWrite[4] = 0;
	BufferWrite[5] = BinaryToBCD(ATime->Date);
	BufferWrite[6] = BinaryToBCD(ATime->Month);
	BufferWrite[7] = BinaryToBCD(ATime->Year);
	struct i2c_master_packet Packet = {
		.address =  DS1307_ADDRESS,
		.data_length = 8,
		.data = &BufferWrite[0],
		.ten_bit_address = false,
		.high_speed = false,
		.hs_master_code = 0x0,
	};
	return I2CMasterWrite(&Packet);
}

/*  DS1307Init
*    Inicializa el módulo.
*/
void DS1307Init(void)
{
	RTCTime Time;
	EnableOscillator();
	Time.Date = 18;
	Time.Hours = 23;
	Time.Minutes = 35;
	Time.Month = 8;
	Time.Seconds = 0;
	Time.Year = 19; 
	//DS1307SetTime(&Time);		//	solo la uso cuando quiero setear por primera ves el tiempo
}