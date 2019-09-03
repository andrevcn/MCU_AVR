/*
 * Timer0.h
 *
 *  Created on: 15/08/2014
 *      Author: Ing Cristian Conejeros
 *      API para manejo del Timer 0 del Atmega128
 *      Se implenta una función para incializar el Timer0 "Timer0_init".
 *      Dicha función permite configurar el modo, el clock y el modo de la salida OC0.
 *      Además se definen macros para habilitar las interrupciones.
 *      La inicialización de las interrupciones no se agrega en la función de incio
 *      de forma que no haya que cambiar (o ratificar) la configuración del Timer0
 *      para habilitar/deshabilitar las interrupciones del mismo.
 *      Recordar que si se usan modos PWM o CTC se debe incilizar el registro OCR0
 *      con el valor adecuado antes de inicializar el timer.
 *      Los handler de interrupción, ya sea por comparación o por desborde están
 *      definidos en Timer0.c; agregar el código deseado en dichos handler.
 */

#ifndef TIMER0_H_
#define TIMER0_H_

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

typedef enum		//Modos de funcionamiento
{
	Normal,				// Modo Normal
	PWM_Phase_Correct,	// Modo PWM Fase Correcta
	CTC,				// Modo Clear To Compare Match
	Fast_PWM,			// Modo Fast PWM
}MODE_TIMER0;

typedef enum		//Modos de funcionamiento
{
	Disconected,			// Modo Normal con OC0 desconectado
	Toggle_On_Comp_Match,	// Toggle. No válido para PWM.
	PWM_No_Invert,			// Modo Clear on Compare Match. PWM no invertido
	PWM_Invert,				// Modo Set on Compare Match. PWM invertido.
}MODE_OC0;


typedef enum				//Prescaler para Timer 0
{
	Timer0_Stoped = 0,			//(1) -> Sin fuente de clock, Timer detenido
	Timer0_Prescaler_1,			//(1) -> Clock MCU sin prescaler
	Timer0_Prescaler_8,			//(3) -> Clock MCU /8	 (PARA 8MHz da 1MHz)
	Timer0_Prescaler_32,		//(5) -> Clock MCU /32	 (PARA 8MHz da 250KHz)
	Timer0_Prescaler_64,		//(6) -> Clock MCU /64	 (PARA 8MHz da 125KHz)
	Timer0_Prescaler_128,		//(7) -> Clock MCU /128	 (PARA 8MHz da 62.50KHz)
	Timer0_Prescaler_256,		//(7) -> Clock MCU /256	 (PARA 8MHz da 31.25KHz)
	Timer0_Prescaler_1024,		//(7) -> Clock MCU /1024 (PARA 8MHz da 7.8125KHz)
}PRES_TIMER0;

typedef enum		//Mensajes de error del TIMER
{
	TIMER0_ERR = 0,
	TIMER0_OK,
}TIMER0_ERROR;

/* *********************** MACROS para el control de las interrupciones ************************ */
//MACROS para activar y descativar la interrupción por desborde
#define TIMER0_OVF_Interrupt_ON TIMSK|=(1<<TOIE0)
#define TIMER0_OVF_Interrupt_OFF TIMSK&=~(1<<TOIE0)

//MACROS para activar y descativar la interrupción por comparación con OCR0
#define TIMER0_COMP_Interrupt_ON TIMSK|=(1<<OCIE0)
#define TIMER0_COMP_Interrupt_OFF TIMSK&=~(1<<OCIE0)


extern volatile MODE_TIMER0 Modo_Timer0;

/* *********************** Funciones para manejo del TIMER ************************ */
TIMER0_ERROR Timer0_init(MODE_TIMER0, PRES_TIMER0, MODE_OC0);			//Inicializa el Timer, MODO, Prescaler, Interrupciones.

ISR(TIMER0_OVF_vect);					//Rutina de Interrupcion por desborde del Timer0
ISR(TIMER0_COMP_vect);					//Rutina de Interrupcion por compare correcta del Timer0

#endif /* TIMER0_H_ */
