/*
 * spiflash.c
 *
 * Created: 31/07/2019 21:38:59
 *  Author: miguel
 */ 
#include "asf.h"
#include "inc/spiflash.h"

#define SPI_MISO_PIN	PIN_PA04
#define SPI_MOSI_PIN	PIN_PA06
#define SPI_CLK_PIN		PIN_PA07 
#define SPI_SS_PIN		PIN_PA05 
 
#define SET_MOSI()		port_pin_set_output_level(SPI_MOSI_PIN, 1)
#define CLEAR_MOSI()	port_pin_set_output_level(SPI_MOSI_PIN, 0)
#define GET_MISO()		port_pin_get_input_level(SPI_MISO_PIN)
#define SET_CLK()		port_pin_set_output_level(SPI_CLK_PIN, 1)
#define CLEAR_CLK()		port_pin_set_output_level(SPI_CLK_PIN, 0)
#define SET_SS()		port_pin_set_output_level(SPI_SS_PIN, 1)
#define CLEAR_SS()		port_pin_set_output_level(SPI_SS_PIN, 0)

/*****		Comandos del Chip *********/
#define BYTE_PROGRAM	0x02	//	Comando de escritura
#define	READ_BYTE		0x03	//	Comando de lectura
#define RDSR			0x05	//	Lectura del registro de estado
#define CMD_WREN		0x06	//	Habilitacion de escritura
#define WRDI			0x04	//	Deshabilitacion de escritura

/**
*	SendByte :
*		Envia un byte por el pin MOSI, MSB primero y en el flanco de subida. 
*	Parametros:
*		uint8_t ADataOut		Dato a enviar
*/
static void SendByte(uint8_t ADataOut)
{
	uint8_t i = 0;
	for (i = 0; i < 8; i++){
		if ((ADataOut & 0x80) == 0x80)		//	Verifica si MSB esta en alto
			SET_MOSI();						//	MOSI = 1
		else
			CLEAR_MOSI();					//	MOSI = 0
		SET_CLK();							//	SCK = 1			
		ADataOut = (ADataOut << 1);			//	Desplazamos para evaluar el proximo bit
		CLEAR_CLK();						//	SCK = 0			
	}
}

/**
*	GetByte:
*		Obtiene un byte desde la entrada MISO
*	Retorna:
*		El byte leido	
*/
static uint8_t GetByte(void)
{
	uint8_t i = 0, in = 0, temp = 0;
	for (i = 0; i < 8; i++){
		in = (in << 1);							
		temp = GET_MISO();					//	Obtenemos el nivel de la entrada MISO
		SET_CLK();							//	SCK = 1
		if (temp == 1)						//	Verificamos el valor
			in = in | 0x01;						
		CLEAR_CLK();						//	SCK = 0		
	}
	return in;
}

/**
*	ReadStatusRegister:
*		Lee el registro de estado del chip.
*	Retorna:
*		El valor del registro.	  
*/
static uint8_t ReadStatusRegister(void)
{
	unsigned char byte = 0;
	CLEAR_SS();					//	Habilitamos el dispositivo
	SendByte(RDSR);				//	Envia el comando RDSR 
	byte = GetByte();			
	SET_SS();					//	Deshabilitamos el dispositivo
	return byte;
}

/*
*	SendWRDI:
*		Comado de deshabilitacion de escritura
*/
static void SendWRDI(void)
{
	CLEAR_SS();					//	Habilitamos el dispositivo
	SendByte(WRDI);				//	Envia el comando WRDI 
	SET_SS();					//	Deshabilitamos el dispositivo
}

/*
*	SendWREN:
*		Comado de habilitacion de escritura
*/
static void SendWREN(void)
{
	CLEAR_SS();					//	Habilitamos el dispositivo
	SendByte(0x06);				//	Envia el comando WREN
	SET_SS();					//	Deshabilitamos el dispositivo
}

/**
* ReadByte:
*	Lee un byte del dispositivo en la direccion indicada	
* Parametros:
* 	AADdress		Direccion 0 --- 0x1FFFFF					
**/
static uint8_t ReadByte(uint32_t AAddress)
{
	uint8_t byte = 0;

	CLEAR_SS();					//	Habilitamos el dispositivo
	SendByte(READ_BYTE); 			//	Comando de lectura
	SendByte(((AAddress & 0xFFFFFF) >> 16));	
	SendByte(((AAddress & 0xFFFF) >> 8));
	SendByte(AAddress & 0xFF);
	byte = GetByte();
	SET_SS();					//	Deshabilitamos el dispositivo
	return byte;				//	retornamos el byte leido
}

/**
* ByteProgram:						
*	Programa una direccion del dispositivo								
*	Asume que la direccion est borrada y no esta protegida por escritura				
*	Parametros:
*		uint32_t AAddress		Direccion a programar 
*		uint8_t AByte			Dato a programar
*/
void ByteProgram(uint32_t AAddress, uint8_t AByte)
{
	CLEAR_SS();					//	Habilitamos el dispositivo
	SendByte(BYTE_PROGRAM);		//	Comando de programacion
	SendByte(((AAddress & 0xFFFFFF) >> 16));	
	SendByte(((AAddress & 0xFFFF) >> 8));
	SendByte(AAddress & 0xFF);
	SendByte(AByte);			//	Byte a ser programado
	SET_SS();					//	Deshabilitamos el dispositivo
}


/* SPIFlashInit
*  Inicializa el módulo
*/
void SPIFlashInit(void)
{
	struct port_config config_port_pin;
	port_get_config_defaults(&config_port_pin);
	config_port_pin.direction = PORT_PIN_DIR_INPUT;
	config_port_pin.input_pull = PORT_PIN_PULL_NONE;
	port_pin_set_config(SPI_MISO_PIN, &config_port_pin);		//	MISO
	config_port_pin.direction = PORT_PIN_DIR_OUTPUT;
	config_port_pin.input_pull = PORT_PIN_PULL_NONE;
	port_pin_set_config(SPI_MOSI_PIN, &config_port_pin);		//	MOS1
	config_port_pin.direction = PORT_PIN_DIR_OUTPUT;
	config_port_pin.input_pull = PORT_PIN_PULL_NONE;
	port_pin_set_config(SPI_CLK_PIN, &config_port_pin);			//	CLK
	config_port_pin.direction = PORT_PIN_DIR_OUTPUT;
	config_port_pin.input_pull = PORT_PIN_PULL_NONE;
	port_pin_set_config(SPI_SS_PIN, &config_port_pin);			//	SS
	SET_SS();
	CLEAR_CLK();
	CLEAR_MOSI();
}

/* SPIFlashWriteMessage
*  Guarda un mensaje en memoria serial.
*  Parámetros
*		AMessage 			puntero a los datos
*		AMessageLenght  	longitud de los datos
*  Devuelve:
*		0				OK
*		-1				E_QUEUE_FULL
*/
int32_t SPIFlashWriteMessage(uint8_t *AMessage, uint8_t AMessageLenght )
{
	
}

/* SPIFlashGetMessage
*  Obtiene un mensaje de la cola.
*  Parámetros:
*		AMessage 		puntero al buffer donde se guardará el mensaje
*		ADataRead		puntero donde se guarda la cantidad de datos leidos
*  Devuelve:
*		0				OK
*		-1				E_QUEUE_EMPTY
*/
int32_t SPIFlashGetMessage(uint8_t *AMessage, uint8_t *ADataRead)
{
	int Result;
	uint32_t Delay = 0xff;
	uint8_t TxBuffer[8];
	uint8_t RxBuffer[8];
	//SendWREN();
	while(Delay--);
	//ByteProgram(0x00, 0x99);
	
	RxBuffer[0] = ReadByte(0x00);
	RxBuffer[1] = 0;
	return 0;
}