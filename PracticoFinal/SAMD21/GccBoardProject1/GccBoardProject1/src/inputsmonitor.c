/*
 * InputsMonitor.c
 *
 * Created: 14/06/2019 23:07:34
 *  Author: miguel
 */ 
#include "inc/inputsmonitor.h"
#include "inc/gpioboard.h"

#define MAX_INPUTS_TO_WATCH	4

typedef enum{WAITING_INPUT_CHANGE,WAITING_DELAY_40MS, CHECK_INPUT}MachineStatus_t;	//	Estados de la maquina de estados que detecta los cambioas en las entradas

typedef struct{
	uint8_t 			InputNumber;
	MachineStatus_t		Status;
	bool				CurrentValue;
	bool				OldValue;
	uint32_t			CountDelay;
	TOnInputChange		CallbackFunc;
} InputHandler_t;										//	Estructura usada para controlar las entradas

static uint32_t FIndex;
static bool FInputsMonitorEnable;						//	Habilitacion del modulo.
static InputHandler_t FInput[MAX_INPUTS_TO_WATCH];
static TOnInputChange FOnInputChange;					//	Variable que guarda la funcion de callback que se llama cuando una entrada cambia


/*
*	MachineError:
*		Si el estado de alguna entrada difiere de los definidos se ejecuta esta funcion.
*	Se resetea la entrada en cuestion (AIndexChannel) a valores conocidos.
*	Parametros
*	uint8_t AIndexChannel		Numero de entrada 
*/
static void MachineError(uint8_t AIndexChannel)			
{
	FInput[AIndexChannel].CountDelay = 0;
	FInput[AIndexChannel].CurrentValue = GpioGetPinState(FInput[FIndex].InputNumber);
	FInput[AIndexChannel].OldValue = FInput[AIndexChannel].CurrentValue;
	FInput[AIndexChannel].Status = WAITING_INPUT_CHANGE;
}

/*
*	CheckInputsStatus:
*		Esta rutina es ejecutada periodicamente cada 10ms y es la encargada de actualizar el estado de las entradas.
*		Genera un evento cuando detecta que alguna entrada cambio informando el numero de entrada y el valor. 
*/
static void CheckInputsStatus(void)
{
	for(FIndex = 0;FIndex < MAX_INPUTS_TO_WATCH;FIndex++){
		switch(FInput[FIndex].Status)
		{
			case	WAITING_INPUT_CHANGE:																// En este estado verifica si alguna entrada cambio 
				FInput[FIndex].CurrentValue  = GpioGetPinState(FInput[FIndex].InputNumber);
				if(FInput[FIndex].CurrentValue !=  FInput[FIndex].OldValue){
					FInput[FIndex].Status = WAITING_DELAY_40MS;
					FInput[FIndex].CountDelay = 0;
				}
				break;
			case	WAITING_DELAY_40MS:																	//	En este estado espera que pasen 40ms y cambia al estado CHECK_INPUT
				FInput[FIndex].CountDelay++;
				if(FInput[FIndex].CountDelay == (MAX_DELAY_DEBOUNCE/SAMPLER_PERIOD)){
					FInput[FIndex].CountDelay = 0;
					FInput[FIndex].Status = CHECK_INPUT;												
				}
				break;
			case	CHECK_INPUT:																		//	En este estado volvemos a verificar si la entrada cambio 
				FInput[FIndex].CurrentValue = GpioGetPinState(FInput[FIndex].InputNumber);				//	En realidad el debounce es de MAX_DELAY_DEBOUNCE + SAMPLER_PERIODO
				FInput[FIndex].OldValue = FInput[FIndex].CurrentValue;
				FInput[FIndex].Status = WAITING_INPUT_CHANGE;
				if(FOnInputChange)
					FOnInputChange(FInput[FIndex].InputNumber, FInput[FIndex].CurrentValue);			//	Evento avisando el cambio
				break;
			default:
				MachineError(FIndex);
			break;
		}
	}
}

/*
*	RegCallbackOnInputChange:
*		Registra la funcion que se llamara cuando se detecte un cambio en alguna entrada.
*	Parametros
*	TOnInputChange AOnInputChange		funcion callback
*/
void RegCallbackOnInputChange(TOnInputChange AOnInputChange)
{
	FOnInputChange = AOnInputChange;
}

/*
*	InputsMonitorInit:
*		Inicializa el modulo.
*/
void InputsMonitorInit(void)
{
	FInput[0].InputNumber = INPUT0;
	FInput[1].InputNumber = INPUT1;
	FInput[2].InputNumber = INPUT2;
	FInput[3].InputNumber = INPUT3;
	for(uint32_t i=0;i<MAX_INPUTS_TO_WATCH;i++){
		FInput[i].CountDelay = 0;
		FInput[i].CurrentValue = GpioGetPinState(FInput[FIndex].InputNumber);
		FInput[i].OldValue = FInput[i].CurrentValue;
		FInput[i].Status = WAITING_INPUT_CHANGE;
		FInput[FIndex].CallbackFunc = 0;
	}
}

/*
*	InputsMonitorEnable:
*		Habilita/Deshabilita el modulo en lo referente al monitoreo de las entradas. 	
*/
void InputsMonitorEnable(bool AEnable)
{
	if(FInputsMonitorEnable != AEnable){
		FInputsMonitorEnable = AEnable;
		if(FInputsMonitorEnable == false){
			InputsMonitorInit();
		}
	}
}		

/*
*	InputsMonitorUpdate:
*		Rutina usada para registrarse en el scheduler , con un periodo de 10ms. 
*	De aca se llama a la maquina de estados si esta habilitado el modulo.
*/
void InputsMonitorUpdate(void)
{
	if(FInputsMonitorEnable)
		CheckInputsStatus();
}