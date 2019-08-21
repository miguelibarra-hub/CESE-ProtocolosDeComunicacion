/*
 * RtcCalendar.c
 *
 * Created: 15/06/2019 22:01:13
 *  Author: miguel
 */ 
#include "inc/rtccalendar.h"

static struct rtc_module rtc_instance;
static struct rtc_calendar_time time;


static void configure_rtc_calendar(void)
{
	struct rtc_calendar_config config_rtc_calendar;
	rtc_calendar_get_config_defaults(&config_rtc_calendar);
	struct rtc_calendar_time alarm;
	rtc_calendar_get_time_defaults(&alarm);
	alarm.year = 2020;
	alarm.month = 1;
	alarm.day = 1;
	alarm.hour = 0;
	alarm.minute = 0;
	alarm.second = 4;
	config_rtc_calendar.clock_24h = true;
	config_rtc_calendar.alarm[0].time = alarm;
	config_rtc_calendar.alarm[0].mask = RTC_CALENDAR_ALARM_MASK_YEAR;
	rtc_calendar_init(&rtc_instance, RTC, &config_rtc_calendar);
	rtc_calendar_enable(&rtc_instance);
}

/*
*	RTCCalendarInit:
*		Inicializa el modulo
*/
void RTCCalendarInit(void)
{
	time.year = 2019;
	time.month = 06;
	time.day = 20;
	time.hour = 19;
	time.minute = 30;
	time.second = 59;
	configure_rtc_calendar();							//	Inicializa el periferico RTC
	rtc_calendar_set_time(&rtc_instance, &time);		//	Inicializa el timepo actual 
	rtc_calendar_swap_time_mode(&rtc_instance);			//	Cambia a modo de 24hs
}

/*
*	GetTime:
*		Obtiene la fecha y hora actuales, y las escribe en el puntero *ATimeString
*	Devuelve
*		La cantidad de caracteres escritos
*/
uint8_t GetTime(char *ATimeString)
{
	uint8_t CharCount = 0;
	rtc_calendar_get_time(&rtc_instance, &time);										//	Obtenemos el tiempo
	CharCount = sprintf(ATimeString,"%d",time.day);										
	CharCount = CharCount + sprintf((ATimeString + CharCount),"/");
	CharCount = CharCount + sprintf((ATimeString + CharCount),"%d",time.month);
	CharCount = CharCount + sprintf((ATimeString + CharCount),"/");
	CharCount = CharCount + sprintf((ATimeString + CharCount),"%d",time.year);
	CharCount = CharCount + sprintf((ATimeString + CharCount),":");
	CharCount = CharCount + sprintf((ATimeString + CharCount),"%d",time.hour);
	CharCount = CharCount + sprintf((ATimeString + CharCount),"/");
	CharCount = CharCount + sprintf((ATimeString + CharCount),"%d",time.minute);
	CharCount = CharCount + sprintf((ATimeString + CharCount),"/");
	CharCount = CharCount + sprintf((ATimeString + CharCount),"%d \n\r",time.second);
	return CharCount;
}