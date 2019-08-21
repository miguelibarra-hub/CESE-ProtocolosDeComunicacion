/*
 * messagestore.c
 *
 * Created: 20/08/2019 0:15:42
 *  Author: miguel
 */ 

#include "inc/messagestore.h"
#include "inc/at24c32.h"

#define START_MESSAGE	0x23
#define END_MESSAGE		0x25


static uint16_t FAddressIn;
static uint16_t FAddressOut;
static uint32_t FPendingMessages;
static uint8_t FBufferMessage[PAGE_SIZE];
static uint32_t FFlagEepromEmpty;
static uint32_t FFlagEepromFull;
/*
 * 	GetAddressForInputMessage:
 * 		Busca en EEPROM una pagina valida para escribir un mensaje. Un mensaje guardado esta enmarcado de la siguiente forma
 * 		# Mensaje % . El simbolo # se escribe siempre en el primer byte de la pagina. Por lo que la funcion busca la primer
 * 		pagina que no tenga el simbolo grabado. Si no encuentra sobreescribe el inicio.
 * 	Retorna:
 * 		Direccion de la pagina donde se puede escribe el proximo mensaje.
 * */
static uint16_t GetAddressForInputMessage(void)
{
	uint8_t DataEeprom = 0;
	uint16_t AddressEeprom = 0;
	uint32_t I;
	for(I = 0;I < MAX_PAGES; I++){
		AT24C32ReadByte(&DataEeprom,AddressEeprom);			//	Lee el primer byte de cada pagina
		if(DataEeprom != START_MESSAGE)						//	Verifica si esta ocupada por otro mensaje
			return AddressEeprom;
		else
			AddressEeprom = AddressEeprom + PAGE_SIZE;		//	Proxima pagina	
	}
	//TODO avisar que esta lleno
	return 0;												//	Si no encuentra nada sobreescribe el primer mensaje 
}

/*
 * 	GetAddressForOutputMessage:
 * 		Busca en EEPROM la primer pagina que contiene un mensaje valido para ser extraido.
 * 	Retorna:
 * 		Direccion de la pagina que contiene un mensaje valido.
 * */
static uint16_t GetAddressForOutputMessage(void)
{
	uint8_t DataEeprom = 0; 
	uint16_t AddressEeprom = 0;
	uint32_t I;
	for(I = 0;I < MAX_PAGES;I++){						//	Explora todos los comienzos de pagina
		AT24C32ReadByte(&DataEeprom,AddressEeprom);
		if(DataEeprom == START_MESSAGE)					//	Buscando el caracter de inicio de mensaje
			return AddressEeprom;			
		else
			AddressEeprom = AddressEeprom + PAGE_SIZE;
	}
	//TODO avisar que no hay nada
	return 0;
}

/*
 * 	GetPendingMessages:
 *		Obtiene la cantidad total de mensajes grabados que no se transmitieron.
 *	Retorna:
 *		El numero de mensajes pendientes
 * */
static uint32_t GetPendingMessages(void)
{
	uint8_t DataEeprom = 0;
	uint16_t AddressEeprom = 0;
	uint32_t NumberMessages = 0;
	uint32_t I;
	for(I = 0;I < MAX_PAGES;I++){
		AT24C32ReadByte(&DataEeprom,AddressEeprom);
		if(DataEeprom == START_MESSAGE){
			NumberMessages++;
			AddressEeprom += PAGE_SIZE;
		}
	}
	return NumberMessages;	
}

/*
 * 	SetMessage:
 *		Guarda un mensaje en EEPROM. Le agrega con los caracteres de inicio y fin y lo almacena en
 *		la primer pagina disponible.
 *	Retorna:
 *		0	Operacion exitosa
 *		-1	Falla
 * */
int32_t SetMessage(uint8_t *AMessage, uint8_t ALenght)
{
	uint32_t I;
	int32_t Result = -1;
	if(ALenght < (PAGE_SIZE - 2)){
		FAddressIn = GetAddressForInputMessage();									//	Busca una pagina disponible
		FBufferMessage[0] = START_MESSAGE;											//	Inicializa el buffer con el caracter de inicio
		for(I = 1; I < ALenght; I++)
			FBufferMessage[I] = *(AMessage + I -1);									//	Carga los datos
		FBufferMessage[I] = END_MESSAGE;											//	Indica final
		Result = AT24C32WritePage(&FBufferMessage[0], ALenght + 2, FAddressIn);		//	Guarda en EEPROM
	}
	return Result;
}

/*
 * 	GetMessage:
 * 		Obtiene un mensaje desde EEPROM. Si encuentra un mensaje lo carga en el puntero *AMessage
 * 		una ves cargado, marca la pagina con 0xff para hacerla disponible.
 * 	Retorna:
 * 		0	Operacion exitosa
 *		-1	Falla
 * */
int32_t GetMessage(uint8_t *AMessage)
{
	uint32_t I;
	int32_t Result;
	uint8_t BufferMessage[PAGE_SIZE];
	FAddressOut = GetAddressForOutputMessage();						//	Busca una pagina con mensaje valido
	if(FAddressOut > MAX_ADDRESS)									//	Si no encuentra
		return -1;	
	Result = AT24C32ReadPage(&BufferMessage[0], FAddressOut);		//	Copia los datos a un buffer local
	if(Result == 0){
		for(I = 1; I < PAGE_SIZE; I++)
			*(AMessage + I -1) = BufferMessage[I];					//	Carga el buffer del puntero, sin el inicio
		Result = AT24C32WriteByte(0xff, FAddressOut);				//	Marcamos el mensaje como leido	
	}
	return Result;
}

/*
 * 	MessageStoreInit:
 * 		Inicializa el modulo
 * */
void MessageStoreInit(void)
{
	FAddressIn = GetAddressForInputMessage();		//	Obtiene la primer pagina disponible
	FPendingMessages = GetPendingMessages(); 		//	Obtiene los mensajes pendientes
	FAddressOut = 0;
	FFlagEepromEmpty = 0;
	FFlagEepromFull = 0;
}

/*
 * 	ClearAllMessages:
 * 		Marca todos los comienzos de pagina con 0xFF para que queden disponibles y eliminando de
 * 		esta forma los mensajes guardados.
 * */
void ClearAllMessages(void)
{
	uint32_t I;
	int32_t Result;
	uint16_t AddressEeprom = 0;
	for(I = 0;I < MAX_PAGES;I++){
		Result = AT24C32WriteByte(0xff, AddressEeprom);				//	Marcamos el mensaje como leido
		AddressEeprom += PAGE_SIZE;  	
	}
}
