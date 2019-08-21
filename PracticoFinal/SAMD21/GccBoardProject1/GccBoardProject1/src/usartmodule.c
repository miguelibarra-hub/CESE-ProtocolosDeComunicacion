/*
 * usart.c
 *
 * Created: 16/06/2019 20:39:43
 *  Author: miguel
 */ 
#include "inc/usartmodule.h"

static void UsartWriteCallback(const struct usart_module *const usart_module);
static void UsartReadCallback(const struct usart_module *const usart_module);

static struct usart_module usart_instance;
volatile uint8_t RxBuffer[MAX_RX_BUFFER_LENGTH];
volatile uint8_t TxBuffer[MAX_TX_BUFFER_LENGTH];
static bool FUsartTxBusyFlag;
static TOnDataReceived OnDataReceived; 


/*
 * 	UsartReadCallback:
 * 		Funcion que se llama cuando se recibe un byte
 * */
static void UsartReadCallback(const struct usart_module *const usart_module)
{
	if(OnDataReceived)
		OnDataReceived(RxBuffer[0]);					//	llama al callback con el dato que llego
}

/*
 * 	UsartWriteCallback:
 * 		Funcion que se llama cuando se termina de transmitir un buffer
 * */
static void UsartWriteCallback(const struct usart_module *const usart_module)
{
	FUsartTxBusyFlag = false;			//	Avisa que esta disponible
}

/*
*	UsartInit:
*		Inicializa y habilita el puerto serie con los parametros de configuracion.
*
*/
void UsartInit(uint32_t ABaudRate, uint32_t AParity, uint32_t AStopBits, uint8_t ADataSize)
{
	struct usart_config config_usart;
	usart_get_config_defaults(&config_usart);							//	obtiene configuracion por defecto
	/**** Configura los parametros de comunicacion ****/
	config_usart.baudrate = ABaudRate;
	config_usart.character_size = ADataSize;
	config_usart.parity = AParity;
	config_usart.stopbits = AStopBits;
	/***** Configura los pines usados ****/
	config_usart.mux_setting = EXT2_UART_SERCOM_MUX_SETTING;
	config_usart.pinmux_pad0 = EXT2_UART_SERCOM_PINMUX_PAD0;			//	PB12 TX
	config_usart.pinmux_pad1 = EXT2_UART_SERCOM_PINMUX_PAD1;			//	PB13 RX
	config_usart.pinmux_pad2 = EXT2_UART_SERCOM_PINMUX_PAD2;
	config_usart.pinmux_pad3 = EXT2_UART_SERCOM_PINMUX_PAD3;
	while (usart_init(&usart_instance, EXT2_UART_MODULE, &config_usart) != STATUS_OK) {}
	usart_enable(&usart_instance);
	usart_register_callback(&usart_instance, UsartWriteCallback, USART_CALLBACK_BUFFER_TRANSMITTED);
	usart_register_callback(&usart_instance, UsartReadCallback, USART_CALLBACK_BUFFER_RECEIVED);
	usart_enable_callback(&usart_instance, USART_CALLBACK_BUFFER_TRANSMITTED);
	usart_enable_callback(&usart_instance, USART_CALLBACK_BUFFER_RECEIVED);
	FUsartTxBusyFlag = false;
	OnDataReceived = 0;
}

/*
*	SendBuffer:
*		Envia un buffer de datos por el puerto serie.
*	Parametros
*		uint8_t *ABuffer		Puntero a los datos a transmitir
*		uint32_t ALenght		Longitud de los datos
*	Devuelve
*		0		Funcion exitosa
*		-1		Falla
*/
int32_t SendBuffer(uint8_t *ABuffer, uint32_t ALenght)
{
	if(FUsartTxBusyFlag == false){
		if(ALenght < MAX_TX_BUFFER_LENGTH){
			for(uint32_t i = 0; i< ALenght;i++)
				TxBuffer[i] = *(ABuffer + i);
			if(usart_write_buffer_job(&usart_instance,&TxBuffer[0],ALenght) == STATUS_OK){
				FUsartTxBusyFlag = true;
				return 0;
			}else
				return -1;	
		}else
			return -1;
	}else
		return -1;
}

/*
 * 	CheckRx:
 * 		Esta funcion debe ser llamada asincronamente desde el main para darle funcionalidad al driver
 * 		de recepcion por callback esto es propio de la biblioteca de Atmel
 * */
void CheckRx(void)
{
	usart_read_buffer_job(&usart_instance,	(uint8_t *)RxBuffer, MAX_RX_BUFFER_LENGTH);
}

/*
 * 	RegOnDataReceived:
 * 		Funcion que registra el callback que se va a llamar cuando llegue un byte al puerto serie
 * */
void RegOnDataReceived(TOnDataReceived AOnDataReceived)
{
	OnDataReceived = AOnDataReceived;
}

