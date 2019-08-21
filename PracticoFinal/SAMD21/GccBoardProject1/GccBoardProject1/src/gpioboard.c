/*
 * gpioboard.c
 *
 * Created: 14/06/2019 21:40:32
 *  Author: miguel
 */ 
#include "inc/gpioboard.h"

#define MAX_INPUTS_NUMBER	4		//	Numero maximo de entradas soportadas

const uint8_t GpioPinInputs[MAX_INPUTS_NUMBER] = {PIN_PB06,PIN_PB07,PIN_PA20,PIN_PA21};		//	Guarda el numero de gpio de cada entrada

/*
*	GpioGetPinState :
*		Devuelve el estado de la entrada AGpioPin, este valor internamente es mapeado
*	al nombre del pin.  
*	Parametros
*	Input_t AGpioPin	Tipo generico asignado para describir las entradas 
*/
bool GpioGetPinState(Input_t AGpioPin)			
{
	return port_pin_get_input_level(GpioPinInputs[AGpioPin]);
}

/*
*	GpioConfigure:
*		Inicializa los pines usados en la placa.
*/
void GpioConfigure(void)						
{
	struct port_config config_port_pin;
	port_get_config_defaults(&config_port_pin);						
	config_port_pin.direction = PORT_PIN_DIR_INPUT;
	config_port_pin.input_pull = PORT_PIN_PULL_UP;
	for(uint8_t i = 0;i<MAX_INPUTS_NUMBER;i++){
		port_pin_set_config(GpioPinInputs[i], &config_port_pin);
	}
	config_port_pin.direction = PORT_PIN_DIR_INPUT;
	config_port_pin.input_pull = PORT_PIN_PULL_NONE;
	port_pin_set_config(PIN_PA04, &config_port_pin);			//	MISO
	config_port_pin.direction = PORT_PIN_DIR_OUTPUT;
	config_port_pin.input_pull = PORT_PIN_PULL_NONE;
	port_pin_set_config(PIN_PA06, &config_port_pin);			//	MOS1	
	config_port_pin.direction = PORT_PIN_DIR_OUTPUT;
	config_port_pin.input_pull = PORT_PIN_PULL_NONE;
	port_pin_set_config(PIN_PA07, &config_port_pin);			//	CLK	
	config_port_pin.direction = PORT_PIN_DIR_OUTPUT;
	config_port_pin.input_pull = PORT_PIN_PULL_NONE;
	port_pin_set_config(PIN_PA05, &config_port_pin);			//	SS
}