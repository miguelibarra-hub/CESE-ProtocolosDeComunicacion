/*
 * I2CMasterChannel.h
 *
 * Created: 19/08/2019 0:11:19
 *  Author: miguel
 */ 


#ifndef I2CMASTERCHANNEL_H_
#define I2CMASTERCHANNEL_H_
/*
 * 	I2CMasterInit:
 * 		Inicializa el modulo I2C.
 * */
void I2CMasterInit(void);
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
int32_t I2CMasterWrite(struct i2c_master_packet *const APacket);
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
int32_t I2CMasterRead(struct i2c_master_packet *const APacket);



#endif /* I2CMASTERCHANNEL_H_ */
