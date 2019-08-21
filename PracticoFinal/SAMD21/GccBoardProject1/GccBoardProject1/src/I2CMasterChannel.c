/*
 * I2CMasterChannel.c
 *
 * Created: 19/08/2019 0:11:56
 *  Author: miguel
 */ 
#include "asf.h"
#include "inc/I2CMasterChannel.h"

#define TIMEOUT 10000														//	Timeout de espera maxima de una transferencia

static struct i2c_master_module I2CMasterInstance;							//	Estructura de control del master

/*
 * 	I2CMasterInit:
 * 		Inicializa el modulo I2C.
 * */
void I2CMasterInit(void)
{
	struct i2c_master_config ConfigI2CMaster;								//	Estructura de configuracion del modulo I2C. */
	i2c_master_get_config_defaults(&ConfigI2CMaster);
	ConfigI2CMaster.buffer_timeout = TIMEOUT;								//	Timeout de respuesta del modulo
	ConfigI2CMaster.pinmux_pad0 = EXT1_I2C_SERCOM_PINMUX_PAD0;				//	SDA pinmux
	ConfigI2CMaster.pinmux_pad1 = EXT1_I2C_SERCOM_PINMUX_PAD1;				//	SCL pinmux
	i2c_master_init(&I2CMasterInstance, SERCOM2, &ConfigI2CMaster);			//	Inicializa el master con ConfigI2CMaster
	i2c_master_enable(&I2CMasterInstance);									//	Habilita el modulo
}

/*
 * 	I2CMasterWrite:
 * 		Operacion de escritura en el Bus I2C.
 * 	Parametros:
 * 		struct i2c_master_packet *const APacket		Puntero a la estructura que contienen la info para la transferencia, Direccion del esclavo,
 * 													direccion del buffer de escritura y los parametros de comunicacion.
 * 	Retorna:
 * 		0	Operacion exitosa
 * 		-1 	Falla
 * */
int32_t I2CMasterWrite(struct i2c_master_packet *const APacket)
{
	uint32_t timeout = 0;
	int32_t Result = 0;
	while (i2c_master_write_packet_wait(&I2CMasterInstance, APacket) !=	STATUS_OK) {
		if (timeout++ == TIMEOUT) {
			Result = -1;
			break;
		}
	}
	return Result;
}
/*
 * 	I2CMasterWrite:
 * 		Operacion de lectura del Bus I2C.
 * 	Parametros:
 * 		struct i2c_master_packet *const APacket		Puntero a la estructura que contienen la info para la transferencia, Direccion del esclavo,
 * 													direccion del buffer de escritura y los parametros de comunicacion.
 * 	Retorna:
 * 		0	Operacion exitosa
 * 		-1 	Falla
 * */
int32_t I2CMasterRead(struct i2c_master_packet *const APacket)
{
	uint32_t timeout = 0;
	int32_t Result = 0;
	while (i2c_master_read_packet_wait(&I2CMasterInstance, APacket) !=	STATUS_OK) {
		if (timeout++ == TIMEOUT) {
			Result = -1;
			break;
		}
	}
	return Result;
}
