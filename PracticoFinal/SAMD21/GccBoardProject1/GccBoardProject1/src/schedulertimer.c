/*
 * schedulertimer.c
 *
 * Created: 09/06/2019 20:57:32
 *  Author: miguel
 */ 
#include "asf.h"
#include "inc/schedulertimer.h"

struct tc_module tc_instance;

sTask SCH_tasks_G[SCH_MAX_TASKS];
static uint8_t I;
static uint8_t Error_code_G;
static uint8_t Last_error_code_G;
int32_t Error_tick_count_G;

void SchedulerEventInterrupt(struct tc_module *const module_inst);

void configure_tc(void)
{
	struct tc_config config_tc;
	tc_get_config_defaults(&config_tc);
	config_tc.counter_size = TC_COUNTER_SIZE_8BIT;
	config_tc.clock_source = GCLK_GENERATOR_1;
	config_tc.clock_prescaler = TC_CLOCK_PRESCALER_DIV1;
	config_tc.counter_8_bit.period = 32;
	config_tc.counter_8_bit.compare_capture_channel[0] = 100;
	config_tc.counter_8_bit.compare_capture_channel[1] = 54;
	tc_init(&tc_instance, TC3, &config_tc);
	tc_enable(&tc_instance);
}
void configure_tc_callbacks(void)
{
	tc_register_callback(&tc_instance, SchedulerEventInterrupt,TC_CALLBACK_OVERFLOW);
	tc_enable_callback(&tc_instance, TC_CALLBACK_OVERFLOW);
}

/*
*	SchedulerEventInterrupt
*		Esta funcion es el controlador de la irq del timer TC. Se ejecuta cad 1ms.
*/
void SchedulerEventInterrupt(struct tc_module *const module_inst)
{
	uint8_t Index;
	for (Index = 0; Index < SCH_MAX_TASKS; Index++){
		if (SCH_tasks_G[Index].pTask != 0){									// Chequeamos si hay alguna tarea
			if (SCH_tasks_G[Index].Delay == 0){								// La tarea esta lista para correr
				SCH_tasks_G[Index].RunMe ++;
				if (SCH_tasks_G[Index].Period){
					SCH_tasks_G[Index].Delay = SCH_tasks_G[Index].Period;   // Agendamos tareas periodicas para correr de nuevo
				}
				}else{
				SCH_tasks_G[Index].Delay -= 1;								// Todavia no esta lista para correr, solo decrementamos el Delay
			}
		}
	}
}

/*
*	SchedulerAddTask
*		Agrega una tarea al arreglo de tareas.
*	Parametros
*	void (* pFunction)(void)	Puntero a funcion
*	const uint32_t ADelay		Retardo en Ticks para que la tarea empiece e correr
*	const uint32_t APeriod		Periodo de una tarea periodica.
*/
uint8_t SchedulerAddTask (void (* pFunction)(void),const uint32_t ADelay,const uint32_t APeriod)
{
	uint8_t Index = 0;
	while ((SCH_tasks_G[Index].pTask != 0) && (Index < SCH_MAX_TASKS)){		//	Vemos si hay lugar en el arreglo de tareas
		Index++;
	}
	if(Index == SCH_MAX_TASKS){												//	Vemos si estamos en el final de la lista
		Error_code_G = ERROR_SCH_TOO_MANY_TASKS;							//	Actualizamos la variable de error global
		return SCH_MAX_TASKS;												//	Retornamos el codigo de error
	}
	SCH_tasks_G[Index].pTask = pFunction;
	SCH_tasks_G[Index].Delay = ADelay;
	SCH_tasks_G[Index].Period = APeriod;
	SCH_tasks_G[Index].RunMe = 0;
	return Index; 															//	Retornamos la posicion de la tarea para poder eliminarla luego.
}
/*
*	SchedulerDeleteTask
*		Elimina una tarea del arreglo de tareas
*	Parametros
*		uint8_t ATaskIndex		Handler de la tarea
*/
unsigned char SchedulerDeleteTask(const unsigned char TASK_INDEX)
{
	static unsigned char Return_code;
	if (SCH_tasks_G[TASK_INDEX].pTask == 0){
		Error_code_G = ERROR_SCH_CANNOT_DELETE_TASK;
		Return_code = RETURN_ERROR;
	}else{
		Return_code = RETURN_NORMAL;
	}
	SCH_tasks_G[TASK_INDEX].pTask = 0x0000;
	SCH_tasks_G[TASK_INDEX].Delay = 0;
	SCH_tasks_G[TASK_INDEX].Period = 0;
	SCH_tasks_G[TASK_INDEX].RunMe = 0;
	return Return_code;							
}

/*
*	SchedulerDispatchTasks
*		Esta funcion es la encargada de ejecutar las tareas que estan listas para correr.
*/
void SchedulerDispatchTasks(void)
{
	unsigned char Index;
	for (Index = 0; Index < SCH_MAX_TASKS; Index++){
		if (SCH_tasks_G[Index].RunMe > 0){
			(*SCH_tasks_G[Index].pTask)();							//	Ejecuta la tarea
			SCH_tasks_G[Index].RunMe -= 1;							//	RESET / REDUCE RunMe flag
			if (SCH_tasks_G[Index].Period == 0){
				SchedulerDeleteTask(Index);
			}
		}
	}
}

/*
*	SchedulerTimerInit
*		Inicializa el modulo
*/
void SchedulerTimerInit(void)
{
	uint8_t Index;
	for (Index = 0; Index < SCH_MAX_TASKS; Index++){
		SCH_tasks_G[Index].Delay = 0;
		SCH_tasks_G[Index].Period = 0;
		SCH_tasks_G[Index].pTask = 0;
		SCH_tasks_G[Index].RunMe = 0;
	}
	
}

/*
*	SchedulerTimerStart
*		Inicia el scheduler al configurar el timer TC
*/
void SchedulerTimerStart(void)
{
	configure_tc();
	configure_tc_callbacks();
}