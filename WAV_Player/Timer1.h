/*
 * Timer1.h
 *
 *  Created on: 24/09/2014
 *      Author: Ing Cristian Conejeros
 *      API para manejo del Timer 1 del Atmega128
 *      Se implenta una función para incializar el Timer1 "Timer1_init".
 *      Dicha función permite configurar el modo, el clock y el modo de 
 *      las salidas OC1A, OC1B y OC1C.
 *      Además se definen macros para habilitar las interrupciones y también
 *		macros para forzar las salidaas OC1A, OC1B y OC1C.
 *      La inicialización de las interrupciones no se agrega en la función de incio
 *      de forma que no haya que cambiar (o ratificar) la configuración del Timer1
 *      para habilitar/deshabilitar las interrupciones del mismo.
 *      Recordar que si se usan modos PWM o CTC se debe incilizar el registro de comparación
 *		(en caso de ser necesario) con el valor adecuado antes de inicializar el timer.
 *      Los handler de interrupción, ya sea por comparación, por captura o por desborde están
 *      definidos en Timer1.c; agregar el código deseado en dichos handler.
 */

#ifndef TIMER1_H_
#define TIMER1_H_

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

typedef enum					//Modos de funcionamiento
{
	Normal_1,							// Modo Normal
	PWM_Phase_Correct_8b,			// Modo PWM Fase Correcta en 8 bits (0-255)
	PWM_Phase_Correct_9b,			// Modo PWM Fase Correcta en 9 bits (0-511)
	PWM_Phase_Correct_10b,			// Modo PWM Fase Correcta en 10 bits (0-1023)
	CTC_OCR1A, 						// Modo Clear To Compare Match OCR1A
	Fast_PWM_8b,					// Modo Fast PWM Correcta en 8 bits (0-255)
	Fast_PWM_9b,					// Modo Fast PWM Correcta en 9 bits (0-511)
	Fast_PWM_10b,					// Modo Fast PWM Correcta en 10 bits (0-1023)
	PWM_Phase_Freq_Correct_ICR1,	// Modo PWM Fase Y Frecuencia Correcta
	PWM_Phase_Freq_Correct_OCR1A,	// Modo PWM Fase Y Frecuencia Correcta
	PWM_Phase_Correct_ICR1,			// Modo PWM Fase Correcta en 16 bits ICR
	PWM_Phase_Correct_OCR1,			// Modo PWM Fase Correcta en 16 bits OCR
	CTC_ICR1, 						// Modo Clear To Compare Match OCR1A
	Reservado,						// Reservado. No utilizar.
	Fast_PWM_ICR1,					// Modo Fast PWM Correcta en 16 bits ICR1
	Fast_PWM_OCR1A,					// Modo Fast PWM Correcta en 16 bits OCR1A
}MODE_TIMER1;

typedef enum		//Modos de funcionamiento
{
	Timer1_Disconected,			// Modo Normal con OC0 desconectado
	Timer1_Toggle_On_Comp_Match,	// Toggle. No válido para PWM.
	Timer1_PWM_No_Invert,			// Modo Clear on Compare Match. PWM no invertido
	Timer1_PWM_Invert,				// Modo Set on Compare Match. PWM invertido.
}MODE_OC1X;


typedef enum				//Prescaler para Timer 0
{
	Timer1_Stoped = 0,			//(1) -> Sin fuente de clock, Timer detenido
	Timer1_Prescaler_1,			//(1) -> Clock MCU sin prescaler
	Timer1_Prescaler_8,			//(3) -> Clock MCU /8	 (PARA 8MHz da 1MHz)
	Timer1_Prescaler_64,		//(6) -> Clock MCU /64	 (PARA 8MHz da 125KHz)
	Timer1_Prescaler_256,		//(7) -> Clock MCU /256	 (PARA 8MHz da 31.25KHz)
	Timer1_Prescaler_1024,		//(7) -> Clock MCU /1024 (PARA 8MHz da 7.8125KHz)
	Timer1_External_Falling,	// Clock externo a través del pin T1. Flanco descendente.
	Timer1_External_Rising,		// Clock externo a través del pin T1. Flanco ascendente.
}PRES_TIMER1;

typedef enum		//Mensajes de error del TIMER
{
	TIMER1_ERR = 0,
	TIMER1_OK,
}TIMER1_ERROR;

/* *********************** MACROS para el control de las interrupciones ************************ */
//MACROS para activar y descativar la interrupción por desborde
#define TIMER1_OVF_Interrupt_ON TIMSK|=(1<<TOIE1)
#define TIMER1_OVF_Interrupt_OFF TIMSK&=~(1<<TOIE1)

//MACROS para activar y descativar la interrupción por comparación con OCR1A, OCR1B y OCR1C
#define TIMER1_COMPA_Interrupt_ON TIMSK|=(1<<OCIE1A)
#define TIMER1_COMPA_Interrupt_OFF TIMSK&=~(1<<OCIE1A)
#define TIMER1_COMPB_Interrupt_ON TIMSK|=(1<<OCIE1B)
#define TIMER1_COMPB_Interrupt_OFF TIMSK&=~(1<<OCIE1B)
#define TIMER1_COMPC_Interrupt_ON ETIMSK|=(1<<OCIE1C)
#define TIMER1_COMPC_Interrupt_OFF ETIMSK&=~(1<<OCIE1C)

//MACROS para activar y descativar la interrupción por comparación con OCR1A, OCR1B y OCR1C
#define TIMER1_CAPT_Interrupt_ON TIMSK|=(1<<TICIE1)
#define TIMER1_CAPT_Interrupt_OFF TIMSK&=~(1<<TICIE1)

extern volatile MODE_TIMER1 Modo_Timer1;

/* *********************** Funciones para manejo del TIMER ************************ */
TIMER1_ERROR Timer1_init(MODE_TIMER1, PRES_TIMER1, MODE_OC1X, MODE_OC1X, MODE_OC1X);			//Inicializa el Timer, MODO, Prescaler, Interrupciones.

ISR(TIMER1_OVF_vect);					//Rutina de Interrupcion por desborde del Timer1
ISR(TIMER1_CAPT_vect);					//Rutina de Interrupcion por captura del Timer1
ISR(TIMER1_COMPA_vect);					//Rutina de Interrupcion por comparación correcta con OCR1A del Timer1
ISR(TIMER1_COMPB_vect);					//Rutina de Interrupcion por comparación correcta con OCR1B del Timer1
ISR(TIMER1_COMPC_vect);					//Rutina de Interrupcion por comparación correcta con OCR1C del Timer1
#endif /* TIMER1_H_ */
