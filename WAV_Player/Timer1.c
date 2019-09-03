/*
 * Timer1.c
 *
 *  Created on: 24/08/2014
 *      Author: Ing. Cristian Conejeros 
 *		API para manejo del Timer 1 del Atmega128
 */
#include "Timer1.h"

#define TRUE 1

/************************ Variables Globales	 *******************************/
volatile MODE_TIMER1 modo_Timer1 = Normal_1;

/*************************************************************************
Funcion: 	Timer1_init()
Proposito:  Inicializa el Timer 1.
Parámetros: Modo: Normal, PWM_Phase_Correct_8b, PWM_Phase_Correct_9b, PWM_Phase_Correct_10b,
				CTC_OCR1A, Fast_PWM_8b, Fast_PWM_9b, Fast_PWM_10b,	PWM_Phase_Freq_Correct_ICR1,
				PWM_Phase_Freq_Correct_OCR1A, PWM_Phase_Correct_ICR1, PWM_Phase_Correct_OCR1,
				CTC_ICR1, Fast_PWM_ICR1, Fast_PWM_OCR1A, 
		  	Valor de Prescaler: Timer1_Stoped, Timer1_Prescaler_1, ...8, ...64, ...256, ...1024,
				Timer1_External_Falling, Timer1_External_Rising.
		  	Modo de la salida OC1A: Disconected, Toggle_On_Comp_Match, PWM_No_Invert, PWM_Invert.
			Modo de la salida OC1B: Disconected, Toggle_On_Comp_Match, PWM_No_Invert, PWM_Invert.
			Modo de la salida OC1C: Disconected, Toggle_On_Comp_Match, PWM_No_Invert, PWM_Invert.
Devuelve:  	TIMER1_ERR si algún parámetro es erróneo.
**************************************************************************/

TIMER1_ERROR Timer1_init(MODE_TIMER1 modo, PRES_TIMER1 fr_div, MODE_OC1X salida_A, MODE_OC1X salida_B, MODE_OC1X salida_C)			//Inicializa el Timer, MODO, Prescaler, Interrupciones
{
	if ((modo > Fast_PWM_OCR1A) | (fr_div > Timer1_External_Rising) | (salida_A > Timer1_PWM_Invert) | (salida_C > Timer1_PWM_Invert) | (salida_C > Timer1_PWM_Invert))										//Si el prescales seleccionado es mayor que el maximo posible devuelve error
		return TIMER1_ERR;
	
	TCCR1A = 0;										// Reinicio la configuración totalmente
	TCCR1B = 0;										
	TCCR1C = 0;										
	TIMSK &= ~((1<<TICIE1)|(1<<OCIE1A)|(1<<OCIE1B)|(1<<TOIE1));	// Deshabilito las interrupciones del Timer1
	ETIMSK &= ~(1<OCIE1C);
	
	switch (modo) 
	{
		case Normal_1:
			TCCR1A |= 0;
			TCCR1B |= 0;
			break;
		case PWM_Phase_Correct_8b:
			if((salida_A == Timer1_Toggle_On_Comp_Match) | (salida_B == Timer1_Toggle_On_Comp_Match) | (salida_C == Timer1_Toggle_On_Comp_Match) )
				return TIMER1_ERR;
			TCCR1A |= (1<<WGM10);
			TCCR1B |= 0;
			break;
		case PWM_Phase_Correct_9b:
			if((salida_A == Timer1_Toggle_On_Comp_Match) | (salida_B == Timer1_Toggle_On_Comp_Match) | (salida_C == Timer1_Toggle_On_Comp_Match) )
				return TIMER1_ERR;
			TCCR1A |= (1<<WGM11);
			TCCR1B |= 0;
			break;
		case PWM_Phase_Correct_10b:
			if((salida_A == Timer1_Toggle_On_Comp_Match) | (salida_B == Timer1_Toggle_On_Comp_Match) | (salida_C == Timer1_Toggle_On_Comp_Match) )
				return TIMER1_ERR;
			TCCR1A |= (1<<WGM11)|(1<<WGM10);
			TCCR1B |= 0;
			break;
		case CTC_OCR1A:
			TCCR1A |= 0;
			TCCR1B |= (1<<WGM12);
			break;
		case Fast_PWM_8b:
			if((salida_A == Timer1_Toggle_On_Comp_Match) | (salida_B == Timer1_Toggle_On_Comp_Match) | (salida_C == Timer1_Toggle_On_Comp_Match) )
				return TIMER1_ERR;
			TCCR1A |= (1<<WGM10);
			TCCR1B |= (1<<WGM12);
			break;
		case Fast_PWM_9b:
			if((salida_A == Timer1_Toggle_On_Comp_Match) | (salida_B == Timer1_Toggle_On_Comp_Match) | (salida_C == Timer1_Toggle_On_Comp_Match) )
				return TIMER1_ERR;
			TCCR1A |= (1<<WGM11);
			TCCR1B |= (1<<WGM12);
			break;
		case Fast_PWM_10b:
			if((salida_A == Timer1_Toggle_On_Comp_Match) | (salida_B == Timer1_Toggle_On_Comp_Match) | (salida_C == Timer1_Toggle_On_Comp_Match) )
				return TIMER1_ERR;
			TCCR1A |= (1<<WGM11)|(1<<WGM10);
			TCCR1B |= (1<<WGM12);
			break;
		case PWM_Phase_Freq_Correct_ICR1:
			if((salida_A == Timer1_Toggle_On_Comp_Match) | (salida_B == Timer1_Toggle_On_Comp_Match) | (salida_C == Timer1_Toggle_On_Comp_Match) )
				return TIMER1_ERR;
			TCCR1A |= 0;
			TCCR1B |= (1<<WGM13);
			break;
		case PWM_Phase_Freq_Correct_OCR1A:
			if((salida_A == Timer1_Toggle_On_Comp_Match) | (salida_B == Timer1_Toggle_On_Comp_Match) | (salida_C == Timer1_Toggle_On_Comp_Match) )
				return TIMER1_ERR;
			TCCR1A |= (1<<WGM10);
			TCCR1B |= (1<<WGM13);
			break;
		case PWM_Phase_Correct_ICR1:
			if((salida_A == Timer1_Toggle_On_Comp_Match) | (salida_B == Timer1_Toggle_On_Comp_Match) | (salida_C == Timer1_Toggle_On_Comp_Match) )
				return TIMER1_ERR;
			TCCR1A |= (1<<WGM11);
			TCCR1B |= (1<<WGM13);
			break;
		case PWM_Phase_Correct_OCR1:
			if((salida_A == Timer1_Toggle_On_Comp_Match) | (salida_B == Timer1_Toggle_On_Comp_Match) | (salida_C == Timer1_Toggle_On_Comp_Match) )
				return TIMER1_ERR;
			TCCR1A |= (1<<WGM11)|(1<<WGM10);
			TCCR1B |= (1<<WGM13);
			break;
		case CTC_ICR1:if((salida_A == Timer1_Toggle_On_Comp_Match) | (salida_B == Timer1_Toggle_On_Comp_Match) | (salida_C == Timer1_Toggle_On_Comp_Match) )
			TCCR1A |= 0;
			TCCR1B |= (1<<WGM13)|(1<<WGM12);
			break;
		case Fast_PWM_ICR1:
			if((salida_A == Timer1_Toggle_On_Comp_Match) | (salida_B == Timer1_Toggle_On_Comp_Match) | (salida_C == Timer1_Toggle_On_Comp_Match) )
				return TIMER1_ERR;
			TCCR1A |= (1<<WGM11);
			TCCR1B |= (1<<WGM13)|(1<<WGM12);
			break;
		case Fast_PWM_OCR1A:
			if((salida_A == Timer1_Toggle_On_Comp_Match) | (salida_B == Timer1_Toggle_On_Comp_Match) | (salida_C == Timer1_Toggle_On_Comp_Match) )
				return TIMER1_ERR;
			TCCR1A |= (1<<WGM11)|(1<<WGM10);
			TCCR1B |= (1<<WGM13)|(1<<WGM12);
			break;
		default:
			return TIMER1_ERR;		//Caso de error de configuracion
			break;
	}	
	TCCR1B |= fr_div;											// Configuro el Prescaler
	TCCR1A |= (salida_A << 6)|(salida_B << 4)|(salida_C << 2);	// Configuro las salidas
	if(salida_A!=Timer1_Disconected)
		DDRB|=(1<<5);
	else
		DDRB&=~(1<<5);
	if(salida_B!=Timer1_Disconected)
		DDRB|=(1<<6);
	if(salida_C!=Timer1_Disconected)
		DDRB|=(1<<7);
	return TIMER1_OK;
}

// **************************************************************************************
// Rutina de interrupción del Timer1
// **************************************************************************************
ISR(TIMER1_OVF_vect)					//Rutina de Interrupcion por desborde del Timer1
{
	
}
ISR(TIMER1_CAPT_vect)					//Rutina de Interrupcion por captura del Timer1
{
	
}
/* ISR(TIMER1_COMPA_vect)					//Rutina de Interrupcion por comparación correcta con OCR1A del Timer1
{
	DDRB|=(1<<7);
	PORTB^=0x80;
}*/
ISR(TIMER1_COMPB_vect)					//Rutina de Interrupcion por comparación correcta con OCR1B del Timer1
{
	
}
ISR(TIMER1_COMPC_vect)					//Rutina de Interrupcion por comparación correcta con OCR1C del Timer1
{
	
}
