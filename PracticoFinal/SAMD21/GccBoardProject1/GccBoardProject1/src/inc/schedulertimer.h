/*
 * schedulertimer.h
 *
 * Created: 09/06/2019 20:57:57
 *  Author: miguel
 */ 


#ifndef SCHEDULERTIMER_H_
#define SCHEDULERTIMER_H_

#define SCH_MAX_TASKS 16

#define ERROR_SCH_TOO_MANY_TASKS     1
#define ERROR_SCH_CANNOT_DELETE_TASK 2

#define RETURN_ERROR  1
#define RETURN_NORMAL 0

typedef struct
{
  void (* pTask)(void);		//	Puntero a la tarea
  uint32_t Delay; 			//	Retardo en TICKS del TIMER, hasta que la tarea empezara a correr   
  uint32_t Period;			//	Intervalo entre ejecuciones  
  uint8_t RunMe;			//	Incrementado por el scheduler cuando la tarea esta lista para ejecutarse
}sTask;

/*
*	SchedulerDeleteTask
*		Elimina una tarea del arreglo de tareas
*	Parametros
*		uint8_t ATaskIndex		Handler de la tarea 
*/
uint8_t SchedulerDeleteTask(uint8_t ATaskIndex);

/*
*	SchedulerAddTask
*		Agrega una tarea al arreglo de tareas.
*	Parametros
*	void (* pFunction)(void)	Puntero a funcion
*	const uint32_t ADelay		Retardo en Ticks para que la tarea empiece e correr
*	const uint32_t APeriod		Periodo de una tarea periodica. 
*/
uint8_t SchedulerAddTask (void (* pFunction)(void),const uint32_t ADelay,const uint32_t APeriod);

/*
*	SchedulerDispatchTasks
*		Esta funcion es la encargada de ejecutar las tareas que estan listas para correr.
*/
void SchedulerDispatchTasks(void);

/*
*	SchedulerTimerInit
*		Inicializa el modulo
*/
void SchedulerTimerInit(void);

/*
*	SchedulerTimerStart
*		Inicia el scheduler al configurar el timer TC
*/
void SchedulerTimerStart(void);

#endif /* SCHEDULERTIMER_H_ */