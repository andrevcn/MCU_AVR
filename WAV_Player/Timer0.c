/*
 * Timer0.c
 *
 *  Created on: 15/08/2014
 *      Author: Ing. Cristian Conejeros 
 *		API para manejo del Timer 0 del Atmega128
 */
#include "Timer0.h"

/************************ Variables Globales	 *******************************/
volatile MODE_TIMER0 modo_Timer0 = Normal;

/*************************************************************************
Funcion: 	Timer0_init()
Proposito:  Inicializa el Timer 0.
Parámetros: Modo: Normal, PWM_Phase_Correct, CTC, Fast_PWM.
		  	Valor de Prescaler: Timer0_Stoped, Timer0_PrescaleR_1, 8, 32, 64, 128,256, 1024.
		  	Modo de la salida OC0: Disconected,	Toggle_On_Comp_Match, PWM_No_Invert, PWM_Invert.
Devuelve:  	TIMER0_ERR si algún parámetro es erróneo.
**************************************************************************/

TIMER0_ERROR Timer0_init(MODE_TIMER0 modo, PRES_TIMER0 fr_div, MODE_OC0 salida)			//Inicializa el Timer, MODO, Prescaler, Interrupciones
{
	if ((modo > Fast_PWM) | (fr_div > Timer0_Prescaler_128) | (salida > PWM_Invert))										//Si el prescales seleccionado es mayor que el maximo posible devuelve error
		return TIMER0_ERR;
	
	TCCR0 = 0;										// Reinicio la configuración totalmente
	TIMSK &= ~((1<OCIE0)|(1<<TOIE0));				// Deshabilito las interrupciones del Timer0
	
	switch (modo) 
	{
		case Normal:
			PORTB |=(1<<6);
			TCCR0 |= 0;
			TCCR0 |= (salida << 4);
			break;
		case PWM_Phase_Correct:
			if(salida == Toggle_On_Comp_Match)
				return TIMER0_ERR;
			TCCR0 |= (1<<WGM00);
			TCCR0 |= (salida << 4);
			break;
		case CTC:
			PORTB |=(1<<6);
			TCCR0 |= (1<<WGM01);
			TCCR0 |= (salida << 4);
			break;
		case Fast_PWM:
			if(salida == Toggle_On_Comp_Match)
				return TIMER0_ERR;
			TCCR0 |= (1<<WGM10)|(1<<WGM00);
			TCCR0 |= (salida << 4);
			break;
		default:
			return TIMER0_ERR;		//Caso de error de configuracion
			break;
	}	
	TCCR0 |= fr_div;				// Configuro el Prescaler
	return TIMER0_OK;
}

// **************************************************************************************
// Rutina de interrupción por overflow del Timer0
// **************************************************************************************
ISR(TIMER0_OVF_vect)
{

}

// **************************************************************************************
//Rutina de interrupcion por compare correcta del Timer0
// **************************************************************************************
ISR(TIMER0_COMP_vect)
{

}
